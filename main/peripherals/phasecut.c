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


//interrupt sfasato di 2.8 ms circa
static int speeds[5]={58,55,50,38,0};
static int speed_index=0;
static int onoff=0;
static int old_int0=0;
static int old_int1=0;
static int old_int2=0;


void phasecut_set_speed(int index) {
    speed_index=index;
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
    timer_register_cb(200, turn_off_triac1);
}

void turn_off_triac2(void) {
    PWM2_LAT=0;
}

void turn_on_triac2(void) {
    PWM2_LAT = 1;
    timer_register_cb(200, turn_off_triac2);
}

void turn_off_triac3(void) {
    PWM3_LAT=0;
}

void turn_on_triac3(void) {
    PWM3_LAT = 1;
    timer_register_cb(200, turn_off_triac3);
}

void phasecut_set(int value) {
    onoff=value;
}

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt()
{
    int int0=0,int1=0,int2=0;
    int0=INT0;
    int1=INT1;
    int2=INT2;
    
    if (onoff){ 
        if (speeds[speed_index]==0) {
                PWM1_LAT=1;
                PWM2_LAT=1;
                PWM3_LAT=1;
                disable_timer3();
        }
        else if(int0!=old_int0 && int1==old_int1 &&int2==old_int2) {
            timer_register_cb(100*speeds[speed_index], turn_on_triac1);
            Nop();
            Nop();
            Nop();
        } 
        else if(int0==old_int0 && int1!=old_int1 &&int2==old_int2) {
            timer_register_cb(100*speeds[speed_index], turn_on_triac2);
            Nop();
            Nop();
            Nop();
        }
        else if(int0==old_int0 && int1==old_int1 &&int2!=old_int2) {
            timer_register_cb(100*speeds[speed_index], turn_on_triac3);
            Nop();
            Nop();
            Nop();
        }
        else {
            Nop();
            Nop();
            Nop();
        }
    }
    else {
        PWM1_LAT=0;
        PWM2_LAT=0;
        PWM3_LAT=0;
    }
    old_int1=int1;
    old_int2=int2;
    old_int0=int0;
    IFS1bits.CNIF = 0;
}
