#ifndef LEDS_H_INCLUDED
#define LEDS_H_INCLUDED

typedef enum {
    LED_1=0, LED_2, LED_3, LED_4, LED_5, NUM_LEDS
} led_t;

void leds_init();
void leds_set(led_t led, int val);


#endif