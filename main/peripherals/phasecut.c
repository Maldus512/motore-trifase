/*
 * File:   phasecut.c
 * Author: Virginia
 *
 * Created on 20 febbraio 2021, 16.37
 */
#include <xc.h>
#include "phasecut.h"
#include "hardwareprofile.h"
#include "timer.h"

#define TRIAC_HOLD_US 500
#define MINIMUM_PERCENTAGE 40
#define MAXIMUM_PERCENTAGE 70


//interrupt sfasato di 2.8 ms circa
static int percentages[5]   = {50,51,52,54,100};
static int speed_index = 0;
static int onoff       = 0;


void phasecut_set_speed(int index) {
    speed_index = index;
}


void phasecut_init(void) {
    INT0_TRIS=1;
    INT1_TRIS=1;
    INT2_TRIS=1;
    CNENBbits.CNIEB7 = 1;
    CNENBbits.CNIEB13 = 1;
    CNENBbits.CNIEB12 = 1;
    CNPUB = 0;
    CNPDB = 0;
    IFS1bits.CNIF = 0;
    IPC4bits.CNIP = 5;
    IEC1bits.CNIE = 1;
    PWM1_TRIS=0;
    PWM2_TRIS=0;
    PWM3_TRIS=0;
}


void turn_off_triac1(void) {
    PWM1_LAT=0;
}

void turn_on_triac1(void) {
    PWM1_LAT = 1;
    timer_register_cb(TRIAC_HOLD_US, turn_off_triac1);
}

void turn_off_triac2(void) {
    PWM2_LAT=0;
}

void turn_on_triac2(void) {
    PWM2_LAT = 1;
    timer_register_cb(TRIAC_HOLD_US, turn_off_triac2);
}

void turn_off_triac3(void) {
    PWM3_LAT=0;
}

void turn_on_triac3(void) {
    PWM3_LAT = 1;
    timer_register_cb(TRIAC_HOLD_US, turn_off_triac3);
}


void phasecut_set(int value) {
    onoff=value;
}


void __attribute__((interrupt, no_auto_psv)) _CNInterrupt()
{
    static int old_int0  = 0;
    static int old_int1  = 0;
    static int old_int2  = 0;
    static int slope     = 0;
    static int oldstatus = 0;
    
    static unsigned long counter   = 0;
    
    int int0=0,int1=0,int2=0;
    int0=INT0;
    int1=INT1;
    int2=INT2;
    
    if (onoff){
        // Se mi sono acceso fai una rampa
        if (oldstatus == 0) {
           slope = 1;
        }
        
        int speed = percentages[speed_index];
        int calculated_slope = MINIMUM_PERCENTAGE + counter/10;
        
        // Incrementa di 10% al secondo
        if (slope && calculated_slope < speed) {
            speed = calculated_slope;
        }
        
        int usdelay = (100 - speed) * 100;
        
        if (speed > MAXIMUM_PERCENTAGE) {
            PWM1_LAT=1;
            PWM2_LAT=1;
            PWM3_LAT=1;
            disable_timer3();
        }
        else if(int0!=old_int0 && int1==old_int1 &&int2==old_int2) {
            if (slope)
                counter++;
            
            timer_register_cb(usdelay, turn_on_triac1);
        } 
        else if(int0==old_int0 && int1!=old_int1 &&int2==old_int2) {
            timer_register_cb(usdelay, turn_on_triac2);
        }
        else if(int0==old_int0 && int1==old_int1 &&int2!=old_int2) {
            timer_register_cb(usdelay, turn_on_triac3);
        }
    }
    else {
        counter   = 0;
        slope     = 0;
        PWM1_LAT  = 0;
        PWM2_LAT  = 0;
        PWM3_LAT  = 0;
    }
    
    oldstatus = onoff;
    old_int1  = int1;
    old_int2  = int2;
    old_int0  = int0;
    IFS1bits.CNIF = 0;
}
