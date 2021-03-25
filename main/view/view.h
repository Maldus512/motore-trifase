#ifndef VIEW_H_INCLUDED
#define VIEW_H_INCLUDED

#include <stdint.h>
#include "model/model.h"
#include "gel/keypad/keypad.h"

#include "gel/pagemanager/page_manager.h"


view_t view_init(model_t *model);
view_t view_change_page_extra(model_t *model,
        const pman_page_t *page,
        void *extra);
view_t view_change_page(model_t *model, const pman_page_t *page);
view_t view_rebase_page(model_t *model, const pman_page_t *page);
int view_get_next_msg(model_t *model, view_message_t *msg, view_event_t *eventcopy);
int view_process_msg(view_page_command_t vmsg, model_t *model, view_t *view);
void view_event(view_event_t event);



extern const pman_page_t page_main;

#endif