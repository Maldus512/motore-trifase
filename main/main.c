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



int main(void) {
    
    unsigned long ts=0;
    unsigned long tskp=0;
    unsigned long tsin=0;
    int blink=0;
    system_init();
    Init_I2C();
    timer_init();
    LED_RUN_TRIS=0;
    int level=0;
    int fan=0;
    
    
    digout_init();
    digin_init();
    phasecut_init();
    
    buttons_init();
    leds_init();
    
    
    for (;;) {
        digout_set(DIGOUT_RELE1, digin_get(DIGIN_IN1));
        digout_set(DIGOUT_RELE2, digin_get(DIGIN_IN2));
        digout_set(DIGOUT_RELE3, digin_get(DIGIN_IN3));
        if (is_expired(ts,get_millis(), fan ? 200 : 1000)) {
            LED_RUN=blink;
            blink=!blink;
            ts=get_millis();
        }
         if (is_expired(tsin,get_millis(), 2)) {
            tsin=get_millis();
            digin_take_reading();
        }
        if (is_expired(tskp,get_millis(), 2)) {
            tskp=get_millis();
            keypad_update_t update = button_manage(get_millis());
            
            if (update.code==BUTTON_UP && update.event==KEY_CLICK && level<4) {
                level++;
                phasecut_set_speed(level);
            }
            if (update.code==BUTTON_DOWN && update.event==KEY_CLICK && level>0) {
                level--;
                phasecut_set_speed(level);
            }
            int i;
            for (i=0; i<NUM_LEDS;i++) {
                
                leds_set(i,i<=level);
                
            }
            
            if (update.code==BUTTON_FAN && update.event==KEY_CLICK) {
                fan=!fan;
                phasecut_set(fan);
            }
        }
        
    }
    return 0;
}
