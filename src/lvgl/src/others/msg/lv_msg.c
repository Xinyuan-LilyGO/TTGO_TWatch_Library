/**
 * @file lv_msg.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_msg.h"
#if LV_USE_MSG

#include "../../misc/lv_assert.h"
#include "../../misc/lv_ll.h"
#include "../../misc/lv_gc.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    lv_msg_id_t msg_id;
    lv_msg_subscribe_cb_t callback;
    void * user_data;
    void * _priv_data;      /*Internal: used only store 'obj' in lv_obj_subscribe*/
} sub_dsc_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void notify(lv_msg_t * m);
static void obj_notify_cb(lv_msg_t * m);
static void obj_delete_event_cb(lv_event_t * e);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_msg_init(void)
{
    _lv_ll_init(&LV_GC_ROOT(_subs_ll), sizeof(sub_dsc_t));
}

void * lv_msg_subscribe(lv_msg_id_t msg_id, lv_msg_subscribe_cb_t cb, void * user_data)
{
    sub_dsc_t * s = _lv_ll_ins_tail(&LV_GC_ROOT(_subs_ll));
    LV_ASSERT_MALLOC(s);
    if(s == NULL) return NULL;

    lv_memzero(s, sizeof(*s));

    s->msg_id = msg_id;
    s->callback = cb;
    s->user_data = user_data;
    return s;
}

void * lv_msg_subscribe_obj(lv_msg_id_t msg_id, lv_obj_t * obj, void * user_data)
{
    sub_dsc_t * s = lv_msg_subscribe(msg_id, obj_notify_cb, user_data);
    if(s == NULL) return NULL;
    s->_priv_data = obj;

    /*If not added yet, add a delete event cb which automatically unsubcribes the object*/
    sub_dsc_t * s_first = lv_obj_get_event_user_data(obj, obj_delete_event_cb);
    if(s_first == NULL) {
        lv_obj_add_event_cb(obj, obj_delete_event_cb, LV_EVENT_DELETE, s);
    }
    return s;
}

void lv_msg_unsubscribe(void * s)
{
    LV_ASSERT_NULL(s);
    _lv_ll_remove(&LV_GC_ROOT(_subs_ll), s);
    lv_free(s);
}

void lv_msg_send(lv_msg_id_t msg_id, const void * payload)
{
    lv_msg_t m;
    lv_memzero(&m, sizeof(m));
    m.id = msg_id;
    m.payload = payload;
    notify(&m);
}

void lv_msg_update_value(void * v)
{
    lv_msg_send((lv_msg_id_t)v, v);
}

lv_msg_id_t lv_msg_get_id(lv_msg_t * m)
{
    return m->id;
}

const void * lv_msg_get_payload(lv_msg_t * m)
{
    return m->payload;
}

void * lv_msg_get_user_data(lv_msg_t * m)
{
    return m->user_data;
}

lv_msg_t * lv_event_get_msg(lv_event_t * e)
{
    if(e->code == LV_EVENT_MSG_RECEIVED) {
        return lv_event_get_param(e);
    }
    else {
        LV_LOG_WARN("Not interpreted with this event code");
        return NULL;
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void notify(lv_msg_t * m)
{
    sub_dsc_t * s;
    _LV_LL_READ(&LV_GC_ROOT(_subs_ll), s) {
        if(s->msg_id == m->id && s->callback) {
            m->user_data = s->user_data;
            m->_priv_data = s->_priv_data;
            s->callback(m);
        }
    }
}

static void obj_notify_cb(lv_msg_t * m)
{
    lv_event_send(m->_priv_data, LV_EVENT_MSG_RECEIVED, m);
}

static void obj_delete_event_cb(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

    sub_dsc_t * s = _lv_ll_get_head(&LV_GC_ROOT(_subs_ll));
    sub_dsc_t * s_next;
    while(s) {
        /*On unsubscribe the list changes s becomes invalid so get next item while it's surely valid*/
        s_next = _lv_ll_get_next(&LV_GC_ROOT(_subs_ll), s);
        if(s->_priv_data == obj) {
            lv_msg_unsubscribe(s);
        }
        s = s_next;
    }
}

#endif /*LV_USE_MSG*/
