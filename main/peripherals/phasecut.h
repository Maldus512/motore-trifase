#ifndef PHASECUT_H_INCLUDED
#define PHASECUT_H_INCLUDED

void phasecut_init(void);
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt();

#endif