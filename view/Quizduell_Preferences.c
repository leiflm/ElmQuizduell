
#include "Quizduell_View_Private.h"


void qd_view_preferences_clicked_login_cb(void *data, Evas_Object *obj, void *ev)
{
    qd_view_login_page_show();
}


int qd_view_preferences_page_add(void)
{
    Evas_Object *btn, *user_ind;
    view.preferences.layout = elm_box_add(view.win);
    evas_object_size_hint_weight_set(view.preferences.layout, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.preferences.layout, EVAS_HINT_FILL, 0.0);

    user_ind = qd_view_user_indicator_add(view.preferences.layout);
    elm_box_pack_end(view.preferences.layout, user_ind);
    evas_object_show(user_ind);

    // add login button
    btn = elm_button_add(view.win);
    elm_object_part_text_set(btn, "default", "Login");
    evas_object_size_hint_weight_set(btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, 0.0);
    evas_object_show(btn);
    elm_box_pack_end(view.preferences.layout, btn);
    evas_object_smart_callback_add(btn, "clicked", qd_view_preferences_clicked_login_cb, NULL);

    return 0;
}

