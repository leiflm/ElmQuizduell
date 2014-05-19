#include "Quizduell_View_Private.h"

void qd_view_category_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    qd_view_question_page_show((Qd_Game_Info *) data);
    printf("selected: %s\n", elm_object_part_text_get(obj, "default"));
}

Evas_Object *qd_view_category_page_add(Evas_Object *parent, Qd_Game_Info *game)
{
    Evas_Object *layout, *box, *btn1, *btn2, *btn3;

    layout = elm_frame_add(parent);
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    box = elm_box_add(layout);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);

    elm_object_part_content_set(layout, "default", box);
    elm_object_part_text_set(layout, "default", "Select a category");

#define ADD_BUTTON_TO_WITH(_btn, _cb_func, _data, _btn_text) \
    _btn = elm_button_add(layout); \
    evas_object_size_hint_align_set(_btn, EVAS_HINT_FILL, EVAS_HINT_FILL); \
    evas_object_size_hint_weight_set(_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
    evas_object_show(_btn); \
    elm_object_part_text_set(_btn, "default", _btn_text); \
    evas_object_smart_callback_add(_btn, "clicked", _cb_func, _data); \
    elm_box_pack_end(box, _btn)

    ADD_BUTTON_TO_WITH(btn1, qd_view_category_clicked_cb, game, "t1");
    ADD_BUTTON_TO_WITH(btn2, qd_view_category_clicked_cb, game, "t2");
    ADD_BUTTON_TO_WITH(btn3, qd_view_category_clicked_cb, game, "t3");
    
    evas_object_show(layout);

#undef ADD_BUTTON_TO_WITH
    return layout;
}
