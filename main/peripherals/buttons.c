
#include "peripherals/buttons.h"
#include "peripherals/hardwareprofile.h"
#include "gel/debounce/debounce.h"
#include "gel/keypad/keypad.h"

static debounce_filter_t filter;

static keypad_key_t keyboard[] = {
    KEYPAD_KEY(0x01, BUTTON_UP),
    KEYPAD_KEY(0x02, BUTTON_DOWN),
    KEYPAD_KEY(0x04, BUTTON_FAN),
    KEYPAD_KEY(0x08, BUTTON_LIGHT),
  KEYPAD_NULL_KEY,  
};

void buttons_init() {
    BUTTON1_TRIS=1;
    BUTTON2_TRIS=1;
    BUTTON3_TRIS=1;
    BUTTON4_TRIS=1;
    
    debounce_filter_init(&filter);
}

keypad_update_t button_manage(unsigned long ts) {
    unsigned int input=0;
    
    input|=!BUTTON1_PORT;
    input|=(!BUTTON2_PORT)<<1;
    input|=(!BUTTON3_PORT)<<2;
    input|=(!BUTTON4_PORT)<<3;
    debounce_filter(&filter, input, 5);
    
    unsigned int keymap = debounce_value(&filter);
    return keypad_routine(keyboard, 30, 1500, 100, ts, keymap);
}
