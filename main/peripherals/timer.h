#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

unsigned long get_millis(void);
void timer_init();

void timer_register_cb(unsigned long hus, void(*cb) (void));

#endif
        