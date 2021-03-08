/*
 * File:   timer.c
 * Author: Virginia
 *
 * Created on 20 febbraio 2021, 14.58
 */

#include "system.h"
#include "hardwareprofile.h"
#include <stddef.h>
#include <stdlib.h>


#define MAX_TIMERS 6
#define ELAPSED_US(tmr) ((tmr * 100)/6011 + 1)
#define US_TO_TICKS(us) ((us/100)*6011 - 1)


typedef struct {
    void(*callback) (void);
    unsigned long time;
} timer_callback_t ;


static int is_another_timer_scheduled(void);
static size_t get_next_timer(void);
static void update_timers(unsigned long ticks
);


static unsigned long millis = 0;
static size_t current_timer = 0;
static timer_callback_t scheduled_timers[MAX_TIMERS];

unsigned long get_millis(void)
{
    unsigned long res;
    IEC0bits.T1IE   = 0;
    res = millis;
    IEC0bits.T1IE   = 1;
    return res;
}

void timer_init() {
    size_t i;
    for (i = 0; i < MAX_TIMERS; i++)
        scheduled_timers[i] = (timer_callback_t){NULL, 0};
    
    TMR1            = 0;
    T1CONbits.TCS   = 0;            // Internal Clock
    T1CONbits.TCKPS = 0;            // Clock source prescaler
    PR1             = 60111 - 1;     // Period, interrupt every 1ms at 120MHz
    T1CONbits.TON   = 1;            // Enable timer 1
    IPC0bits.T1IP   = 4;            // timer 1 interrupt priority
    IFS0bits.T1IF   = 0;
    IEC0bits.T1IE   = 1;             // Enable timer 1 interrupt
    
    TMR3HLD            = 0;
    TMR2            = 0;
    T2CONbits.TCS   = 0;            // Internal Clock
    T2CONbits.TCKPS = 0;            // Clock source prescaler
    T2CONbits.T32 = 1;
    IPC2bits.T3IP   = 6;            // timer 2 interrupt priority
    IFS0bits.T3IF   = 0;
    IEC0bits.T3IE   = 1;             // Enable timer 2 interrupt
    T2CONbits.TON   = 0;            // Enable timer 2
  
}

void timer3_setperiod_us(unsigned long us) {
    unsigned long hus=us/100;
    uint32_t period = 6011*hus - 1;
    PR3 = (period >> 16)&0xFFFF;
    PR2 =            period & 0xFFFF;
    TMR3HLD            = 0;
    TMR2            = 0;
}

void timer3_setperiod(unsigned long period) {
    PR3 = (period >> 16)&0xFFFF;
    PR2 =            period & 0xFFFF;
    TMR3HLD            = 0;
    TMR2            = 0;
}

void timer_register_cb(unsigned long us, void(*cb) (void)) {
    IEC0bits.T3IE = 0;
  
    //if (T2CONbits.TON) {
        // Il timer stava già girando
        uint16_t tmr2 = TMR2;
        uint32_t tmr3 =(((unsigned long)TMR3HLD) << 16);
        uint32_t total = tmr3 | tmr2;
        update_timers(total);
        
        size_t i = 0;
        for (i = 0; i < MAX_TIMERS; i++) {
            if (scheduled_timers[i].callback == NULL) {
                scheduled_timers[i].callback = cb;
                scheduled_timers[i].time = US_TO_TICKS(us);
                break;
            }
        }
        
        if (i < MAX_TIMERS) {
            size_t next = get_next_timer();
            T2CONbits.TON = 0;
            timer3_setperiod(scheduled_timers[next].time);
            current_timer = next;
            IFS0bits.T3IF   = 0;
            T2CONbits.TON = 1;
        } 
    /*} else {
        scheduled_timers[0].callback = cb;
        scheduled_timers[0].time = us;
        
        // Non stava andando, devo far partire il timer
        timer3_setperiod(scheduled_timers[0].time);
        current_timer = 0;

        IFS0bits.T3IF   = 0;
        T2CONbits.TON = 1;
    }*/
    IEC0bits.T3IE = 1;
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    millis++;
    IFS0bits.T1IF = 0;
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void) {
    
    IEC0bits.T3IE = 0;
    T2CONbits.TON = 0;
    if (scheduled_timers[current_timer].callback!=NULL) {
//        uint16_t tmr2 = PR2;
//        uint32_t tmr3 = ((uint32_t)PR3) << 16;
//        uint32_t total = tmr3 | tmr2;
        //update_timers( ELAPSED_US(total));
        update_timers(scheduled_timers[current_timer].time);
        
        if (is_another_timer_scheduled()) {
            size_t next = get_next_timer();
            current_timer = next;
            
            timer3_setperiod(scheduled_timers[current_timer].time);
            T2CONbits.TON = 1;
            IEC0bits.T3IE = 1;
        }
    }
    IFS0bits.T3IF = 0;
}

void disable_timer3(void) {
    size_t i=0;
    T2CONbits.TON = 0;
    IEC0bits.T3IE = 0;
    for (i=0;i<MAX_TIMERS;i++)
        scheduled_timers[i].callback=NULL;
}


static void update_timers(unsigned long ticks) {
    size_t i;
    timer_callback_t tofire[MAX_TIMERS];
    size_t j=0;
    for (i = 0; i < MAX_TIMERS; i++){
        if (scheduled_timers[i].callback != NULL ){
            if (scheduled_timers[i].time > ticks) {
                scheduled_timers[i].time -= ticks;
            } else {
                tofire[j].callback=scheduled_timers[i].callback;
                j++;
                scheduled_timers[i].callback = NULL;
            }
        }
    }
    for (i=0; i<j; i++) {
        tofire[i].callback();
    }
}


static int is_another_timer_scheduled(void) {
    size_t i;
     for (i = 0; i < MAX_TIMERS; i++){
        if (scheduled_timers[i].callback != NULL )
            return 1;
    }
    
    return 0;
}


static size_t get_next_timer(void) {
    unsigned long min = 0xFFFFFFFF;
    size_t found = 0, i;
    
    for (i = 0; i < MAX_TIMERS; i++){
        if (scheduled_timers[i].callback != NULL && scheduled_timers[i].time < min) {
            found = i;
            min = scheduled_timers[i].time;
        }
    }
    
    return found;
}