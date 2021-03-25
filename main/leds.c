#include "leds.h"
#include "hardwareprofile.h"

void leds_init() {
    LED1_TRIS=0;
    LED2_TRIS=0;
    LED3_TRIS=0;
    LED4_TRIS=0;
    LED5_TRIS=0;
}

void leds_set(led_t led, int val) {
    val = val > 0;
    switch(led) {
        case LED_1 :
            LED1=val;
            break;
        case LED_2 :
            LED2=val;
            break; 
        case LED_3 :
           LED3=val;
            break;
        case LED_4 :
            LED4=val;
            break;
        case LED_5 :
            LED5=val;
            break;
    }
}