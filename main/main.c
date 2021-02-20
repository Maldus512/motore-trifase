/*
 * File:   main.c
 * Author: Virginia
 *
 * Created on 20 febbraio 2021, 13.15
 */

#include "peripherals/hardwareprofile.h"
#include "peripherals/system.h"
#include "peripherals/timer.h"
#include "peripherals/digout.h"
#include "peripherals/digin.h"



int main(void) {
    
    unsigned long ts=0;
    int blink=0;
    system_init();
    timer_init();
    LED_RUN_TRIS=0;
    digout_init();
    digin_init();
 
    for (;;) {
        digout_set(DIGOUT_RELE1, digin_get(DIGIN_IN1));
        digout_set(DIGOUT_RELE2, digin_get(DIGIN_IN2));
        digout_set(DIGOUT_RELE3, digin_get(DIGIN_IN3));
        if (get_millis()-ts>1000) {
            LED_RUN=blink;
            blink=!blink;
            ts=get_millis();
        }
    }
    return 0;
}
