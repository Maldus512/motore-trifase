#ifndef VIEW_TYPES_H_INCLUDED
#define VIEW_TYPES_H_INCLUDED

#include <stdint.h>
#include "gel/keypad/keypad.h"

typedef enum {
    VIEW_PAGE_COMMAND_CODE_NOTHING = 0,
    VIEW_PAGE_COMMAND_CODE_REBASE,
    VIEW_PAGE_COMMAND_CODE_CHANGE_PAGE,
    VIEW_PAGE_COMMAND_CODE_CHANGE_PAGE_EXTRA,
    VIEW_PAGE_COMMAND_CODE_BACK,
    VIEW_PAGE_COMMAND_CODE_UPDATE,
} view_page_command_code_t;

typedef struct {
    view_page_command_code_t code;

    union {
        struct {
            const void *page;
            void *      extra;
        };
    };
} view_page_command_t;


typedef enum {
    VIEW_CONTROLLER_COMMAND_CODE_NOTHING=0,
    VIEW_CONTROLLER_COMMAND_CODE_UPDATE_ONOFF,
    VIEW_CONTROLLER_COMMAND_CODE_UPDATE_SPEED_UP,
    VIEW_CONTROLLER_COMMAND_CODE_UPDATE_SPEED_DOWN,
} view_controller_command_code_t;


typedef struct {
    view_controller_command_code_t code;
} view_controller_command_t;


typedef struct {
    view_page_command_t       vmsg;
    view_controller_command_t cmsg;
} view_message_t;

typedef uint8_t view_t;
typedef keypad_update_t view_event_t;

#endif
