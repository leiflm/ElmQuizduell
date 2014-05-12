#include "Quizduell_View_Private.h"

void qd_view_category_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    qd_view_question_page_show();
    printf("selected: %s\n", elm_object_part_text_get(obj, "default"));
}

Evas_Object *qd_view_category_page_add(Evas_Object *parent)
{
    Evas_Object *layout, *frame, *btn1, *btn2, *btn3;

    layout = elm_box_add(parent);
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

#define ADD_BUTTON_TO_WITH(_btn, _cb_func, _data, _btn_text) \
    _btn = elm_button_add(layout); \
    evas_object_size_hint_align_set(_btn, EVAS_HINT_FILL, EVAS_HINT_FILL); \
    evas_object_size_hint_weight_set(_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
    evas_object_show(_btn); \
    elm_object_part_text_set(_btn, "default", _btn_text); \
    evas_object_smart_callback_add(_btn, "clicked", _cb_func, _data); \
    elm_box_pack_end(layout, _btn)

    ADD_BUTTON_TO_WITH(btn1, qd_view_category_clicked_cb, NULL, "t1");
    ADD_BUTTON_TO_WITH(btn2, qd_view_category_clicked_cb, NULL, "t2");
    ADD_BUTTON_TO_WITH(btn3, qd_view_category_clicked_cb, NULL, "t3");
    
    evas_object_show(layout);

#undef ADD_BUTTON_TO_WITH
    return layout;
}
