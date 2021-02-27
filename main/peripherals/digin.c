/*
 * File:   digin.c
 * Author: Virginia
 *
 * Created on 20 febbraio 2021, 15.33
 */

#include "peripherals/digin.h"
#include "peripherals/hardwareprofile.h"
#include "gel/debounce/debounce.h"

static debounce_filter_t filter;

void digin_init() {
    IN1_TRIS=1;
    IN2_TRIS=1;
    IN3_TRIS=1;
    
    debounce_filter_init(&filter);
}

int digin_get(digin_t digin) {
   return debounce_read(&filter, digin);
}

void digin_take_reading() {
    unsigned int input=0;
    input|=!IN1;
    input|=(!IN2)<<1;
    input|=(!IN3)<<2;
    debounce_filter(&filter, input, 10);
}
