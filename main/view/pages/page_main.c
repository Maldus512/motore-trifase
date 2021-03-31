#include "view/view.h"
#include "model/model.h"
#include "gel/pagemanager/page_manager.h"
#include "buttons.h"


static void *create_page(model_t *model, void *extra) {
    return NULL;
}


static view_message_t process_page_event(model_t *model, void *arg, pman_event_t update){
    view_message_t msg = {.vmsg = {VIEW_PAGE_COMMAND_CODE_NOTHING}};
    
    if (update.event==KEY_CLICK) {
        switch (update.code) {
            case BUTTON_UP:
                msg.cmsg.code = VIEW_CONTROLLER_COMMAND_CODE_UPDATE_SPEED_UP;
                msg.vmsg.code = VIEW_PAGE_COMMAND_CODE_UPDATE;
                break;
            case BUTTON_DOWN:
                msg.cmsg.code = VIEW_CONTROLLER_COMMAND_CODE_UPDATE_SPEED_DOWN;
                msg.vmsg.code = VIEW_PAGE_COMMAND_CODE_UPDATE;
                break;
            case BUTTON_FAN:
                msg.cmsg.code = VIEW_CONTROLLER_COMMAND_CODE_UPDATE_ONOFF;
                msg.vmsg.code = VIEW_PAGE_COMMAND_CODE_UPDATE;
                break;
        }
    }
    return msg;
}

static view_t update_page(model_t *model, void *arg){
    size_t i = 0;
    view_t view = 0;
    
    for (i = 0; i < model->velocita + 1; i++) {
        view |= 1 << i;
    }
    
    return view;
}

const pman_page_t page_main = {
    .create = create_page,
    .update        = update_page,
    .process_event = process_page_event,
};