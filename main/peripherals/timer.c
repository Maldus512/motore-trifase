/*
 * File:   timer.c
 * Author: Virginia
 *
 * Created on 20 febbraio 2021, 14.58
 */

#include "system.h"
#include "hardwareprofile.h"
#include <stddef.h>

static unsigned long millis = 0;
static void(*callback) (void)=NULL;

unsigned long get_millis(void)
{
    unsigned long res;
    IEC0bits.T1IE   = 0;
    res = millis;
    IEC0bits.T1IE   = 1;
    return res;
}

void timer_init() {
    TMR1            = 0;
    T1CONbits.TCS   = 0;            // Internal Clock
    T1CONbits.TCKPS = 0;            // Clock source prescaler
    PR1             = 60111 - 1;     // Period, interrupt every 1ms at 120MHz
    T1CONbits.TON   = 1;            // Enable timer 1
    IPC0bits.T1IP   = 5;            // timer 1 interrupt priority
    IFS0bits.T1IF   = 0;
    IEC0bits.T1IE   = 1;             // Enable timer 1 interrupt
    
    TMR2            = 0;
    TMR3            = 0;
    T2CONbits.TCS   = 0;            // Internal Clock
    T2CONbits.TCKPS = 0;            // Clock source prescaler
    T2CONbits.T32 = 1;
    IPC2bits.T3IP   = 6;            // timer 2 interrupt priority
    IFS0bits.T3IF   = 0;
    IEC0bits.T3IE   = 1;             // Enable timer 2 interrupt
    T2CONbits.TON   = 0;            // Enable timer 2

    
}

void timer_register_cb(unsigned long hus, void(*cb) (void)) {
    IEC0bits.T3IE = 0;
    T2CONbits.TON = 0;
    
    uint32_t period = 6011*hus - 1;
    PR3 = (period >> 16)&0xFFFF;
    PR2=            period & 0xFFFF;
    TMR2            = 0;
    TMR3            = 0;
    
    callback=cb;
    
    IFS0bits.T3IF   = 0;
    IEC0bits.T3IE = 1;
    T2CONbits.TON = 1;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    millis++;
    IFS0bits.T1IF = 0;
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void) {
    
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 0;
    T2CONbits.TON = 0;
    if (callback!=NULL) callback();
   
}
