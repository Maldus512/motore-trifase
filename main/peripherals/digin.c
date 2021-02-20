/*
 * File:   digin.c
 * Author: Virginia
 *
 * Created on 20 febbraio 2021, 15.33
 */

#include "peripherals/digin.h"
#include "peripherals/hardwareprofile.h"

void digin_init() {
    IN1_TRIS=1;
    IN2_TRIS=1;
    IN3_TRIS=1;
}

int digin_get(digin_t digin) {
    switch (digin) {
        case DIGIN_IN1:
            return IN1;
            break;
        case DIGIN_IN2:
            return IN2;
            break;
        case DIGIN_IN3:
            return IN3;
            break;
    }
    return 0;
}

