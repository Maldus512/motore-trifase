#ifndef PHASECUT_H_INCLUDED
#define PHASECUT_H_INCLUDED

void phasecut_init(void);
void phasecut_set_speed(int index);
void turn_off_triac1(void);
void turn_on_triac1(void);
void turn_off_triac2(void);
void turn_on_triac2(void);
void turn_off_triac3(void);
void turn_on_triac3(void);
void phasecut_set(int value);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt();

#endif