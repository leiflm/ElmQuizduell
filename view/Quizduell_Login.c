#include "Quizduell_View_Private.h"
#include "Quizduell_Controller.h"

static void qd_view_user_data_from_entry_get(char **un, char **pw)
{
    const char *_un, *_pw;
    size_t len;

    _un = elm_entry_entry_get(view.login.en_user);
    len = strlen(_un);
    *un = malloc(len + 1);
    snprintf(*un, len + 1, "%s", _un); 

    _pw = elm_entry_entry_get(view.login.en_pw);
    len = strlen(_pw);
    *pw = malloc(len + 1);
    snprintf(*pw, len + 1, "%s", _pw); 
}

static void qd_view_login_page_clicked_ok_cb(void *data, Evas_Object *btn, void *event_info)
{
    char *un, *pw;
    qd_view_user_data_from_entry_get(&un, &pw);
    qd_ctrl_user_login(un, pw);
    qd_view_games_list_page_show();
}

int qd_view_login_page_add(void)
{
    Evas_Object *frame;
    // box as layout
    view.login.layout = elm_box_add(view.win);
    evas_object_size_hint_weight_set(view.login.layout, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.login.layout, EVAS_HINT_FILL, 0.0);

    // User name entry
    frame = elm_frame_add(view.login.layout);
    view.login.en_user = elm_entry_add(view.login.layout);
    elm_entry_single_line_set(view.login.en_user, EINA_TRUE);
    evas_object_size_hint_weight_set(frame, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(view.login.en_user);
    elm_object_part_content_set(frame, "default", view.login.en_user);
    elm_object_part_text_set(frame, "default", "User name");
    evas_object_show(frame);
    elm_box_pack_end(view.login.layout, frame);

    // pw entry
    frame = elm_frame_add(view.login.layout);
    view.login.en_pw = elm_entry_add(view.login.layout);
    elm_entry_single_line_set(view.login.en_pw, EINA_TRUE);
    elm_entry_password_set(view.login.en_pw, EINA_TRUE);
    evas_object_size_hint_weight_set(frame, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(view.login.en_pw);
    elm_object_part_content_set(frame, "default", view.login.en_pw);
    elm_object_part_text_set(frame, "default", "Password");
    evas_object_show(frame);
    elm_box_pack_end(view.login.layout, frame);

    // OK button
    view.login.ok_btn = elm_button_add(view.login.layout);
    elm_object_part_text_set(view.login.ok_btn, "default", "login");
    evas_object_size_hint_weight_set(view.login.ok_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view.login.ok_btn, EVAS_HINT_FILL, 0.0);
    elm_box_pack_end(view.login.layout, view.login.ok_btn);
    evas_object_show(view.login.ok_btn);
    evas_object_smart_callback_add(view.login.ok_btn, "clicked", qd_view_login_page_clicked_ok_cb, NULL);

    evas_object_hide(view.login.layout);

    return 0;
}
