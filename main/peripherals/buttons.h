#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#include "gel/keypad/keypad.h"

typedef enum {
    BUTTON_UP=0,BUTTON_DOWN,BUTTON_FAN,BUTTON_LIGHT
} button_t;

void buttons_init();
keypad_update_t button_manage(unsigned long ts);

#endif