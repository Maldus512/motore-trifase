#include <stdlib.h>

#include "gel/collections/queue.h"

#include "model/model.h"
#include "view/view.h"


QUEUE_DECLARATION(event_queue, view_event_t, 32);
QUEUE_DEFINITION(event_queue, view_event_t);
static struct event_queue q;


static page_manager_t pman;

view_t view_init(model_t *model) {
    pman_init(&pman);
    event_queue_init(&q);
    return view_change_page(model, &page_main);
}


view_t view_change_page_extra(model_t *model, const pman_page_t *page, void *extra) {
    event_queue_init(&q);     // Butta tutti gli eventi precedenti quando cambi la pagina
    return pman_change_page_extra(&pman, model, *page, extra);
}


view_t view_change_page(model_t *model, const pman_page_t *page) {
   return view_change_page_extra(model, page, NULL);
}


view_t view_rebase_page(model_t *model, const pman_page_t *page) {
    event_queue_init(&q);
    return pman_rebase_page(&pman, model, *(pman_page_t *)page);
}


int view_get_next_msg(model_t *model, view_message_t *msg, view_event_t *eventcopy) {
    view_event_t event;
    int          found = 0;

    if (!event_queue_is_empty(&q)) {
        event_queue_dequeue(&q, &event);
        found = 1;
    }

    if (found) {
        *msg = pman.current_page.process_event(model, pman.current_page.data, event);
        if (eventcopy)
            *eventcopy = event;
    }

    return found;
}


int view_process_msg(view_page_command_t vmsg, model_t *model, view_t *view) {
    if (vmsg.code == VIEW_PAGE_COMMAND_CODE_CHANGE_PAGE) {
        view_change_page(model, vmsg.page);
    } else if (vmsg.code == VIEW_PAGE_COMMAND_CODE_CHANGE_PAGE_EXTRA) {
        view_change_page_extra(model, vmsg.page, vmsg.extra);
    } else if (vmsg.code == VIEW_PAGE_COMMAND_CODE_BACK) {
        pman_back(&pman, model);
        event_queue_init(&q);
       // view_event((view_event_t){.code = VIEW_EVENT_CODE_OPEN});
    } else if (vmsg.code == VIEW_PAGE_COMMAND_CODE_REBASE) {
        view_rebase_page(model, vmsg.page);
    } else if (vmsg.code == VIEW_PAGE_COMMAND_CODE_UPDATE) {
        *view = pman_page_update(&pman, model);
        return 1;
    }
    return 0;
}


void view_event(view_event_t event) {
    event_queue_enqueue(&q, &event);
}