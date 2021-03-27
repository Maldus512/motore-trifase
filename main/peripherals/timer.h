#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

unsigned long get_millis(void);
void timer_init();

void timer_register_cb(unsigned long us, void(*cb) (void));
void timer3_setperiod_us(unsigned long us);
void timer3_setperiod(unsigned long period);
void disable_timer3(void);

#endif
        