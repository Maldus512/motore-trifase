/*
 * File:   main.c
 * Author: Virginia
 *
 * Created on 20 febbraio 2021, 13.15
 */

#include <xc.h>
#include "peripherals/hardwareprofile.h"
#include "peripherals/system.h"
#include "peripherals/timer.h"
#include "peripherals/digout.h"
#include "peripherals/digin.h"
#include "peripherals/phasecut.h"
#include "i2c_ports/PIC/i2c_driver.h"
#include "i2c_ports/PIC/i2c_bitbang.h"
#include "i2c_common/i2c_common.h"
#include "i2c_devices/eeprom/24LC16.h"
#include "i2c_devices.h"
#include "buttons.h"
#include "leds.h"
#include "gel/timer/timecheck.h"
#include <stddef.h>
#include "uart_driver.h"
#include "adc.h"
#include "model/model.h"
#include "view/view.h"
#include "controller/modbus_server.h"
#include <stdio.h>
#include <string.h>

void update_gui(view_t view) {
    int i = 0;
    for (i=0; i<NUM_LEDS;i++)      
        leds_set(i,view&(1<<i));
}


void manage_gui(model_t *model) {
    view_message_t umsg;
    view_event_t   event;
    view_t view;

    while (view_get_next_msg(model, &umsg, &event)) {
        if (umsg.cmsg.code==VIEW_CONTROLLER_COMMAND_CODE_UPDATE_SPEED_UP && model->velocita<4) {
            model->velocita++;
            phasecut_set_speed(model->velocita);
        }
        if (umsg.cmsg.code==VIEW_CONTROLLER_COMMAND_CODE_UPDATE_SPEED_DOWN && model->velocita>0) {
            model->velocita--;
            phasecut_set_speed(model->velocita);
        }
        if (umsg.cmsg.code==VIEW_CONTROLLER_COMMAND_CODE_UPDATE_ONOFF) {
            model->fan_on=!model->fan_on;
            phasecut_set(model->fan_on);
        }
        if (view_process_msg(umsg.vmsg, model, &view)) {
            update_gui(view);
        }
    }
}


int main(void) {
    model_t model = {0};
    unsigned long ts=0;
    unsigned long tskp=0;
    unsigned long tsin=0;
    unsigned long ts_rx=0;
    int blink=0;
    system_init();
    Init_I2C();
    timer_init();
    LED_RUN_TRIS=0;
 
    digout_init();
    digin_init();
    phasecut_init();
    buttons_init();
    leds_init();
    init_uart();
    init_adc();
    modbus_server_init();
    update_gui(view_init(&model));
       
    for (;;) {
        digout_set(DIGOUT_RELE1, digin_get(DIGIN_IN1));
        digout_set(DIGOUT_RELE2, digin_get(DIGIN_IN2));
        digout_set(DIGOUT_RELE3, digin_get(DIGIN_IN3));
        manage_gui(&model);
        if (is_expired(ts,get_millis(), model.fan_on ? 250 : 1000)) {
            LED_RUN=blink;
            blink=!blink;
            ts=get_millis();
        }
        
        if (is_expired(ts_rx,get_millis(), 10))  {
            modbus_server_manage();
            ts_rx=get_millis();
        }
        
         if (is_expired(tsin,get_millis(), 2)) {
            tsin=get_millis();
            digin_take_reading();
        }
        
        if (is_expired(tskp,get_millis(), 2)) {
            tskp=get_millis();
            keypad_update_t update = button_manage(get_millis());
            view_event(update);
        }
        
    }
    return 0;
}
