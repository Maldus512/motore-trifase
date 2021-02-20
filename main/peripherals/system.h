#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED


#include <xc.h>
#define FOSC 120223125
#define FCY (FOSC/2)
#include <libpic30.h>

void system_init();

#endif