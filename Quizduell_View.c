#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>

#include "Quizduell_Config.h"
#include "view/Quizduell_View_Private.h"

void qd_view_user_name_set(char *name)
{
    ecore_event_add(QD_EVENT_USER_NAME_CHANGED, name, NULL, NULL);
}

Eina_Bool qd_view_set_user_name_ev_hd_cb(void *data, int type, void *ev)
{
    Evas_Object *tp = (Evas_Object *) data;
    char *text = (char *) ev;
    printf("setting new names\n");

    elm_object_part_text_set(tp, "default", text);
    return ECORE_CALLBACK_PASS_ON;
}

void qd_view_set_user_ev_handler_del_cb(void *data, Evas *e, Evas_Object *obj, void *ev)
{
    Ecore_Event_Handler *eh = (Ecore_Event_Handler *) data;
    ecore_event_handler_del(eh);
}


Evas_Object *qd_view_user_indicator_add(Evas_Object *parent)
{
    Evas_Object *box, *layout, *name_obj;
    Eina_Stringshare *name;
    Ecore_Event_Handler *hd;

    // use a fram as main layout obj
    layout = elm_frame_add(parent);
    elm_object_part_text_set(layout, "default", "logged in as");

    // put box into frame
    box = elm_box_add(layout);
    elm_box_horizontal_set(box, EINA_TRUE);
    evas_object_show(box);

    // Label for user name
    name_obj = elm_label_add(layout);
    name = eina_stringshare_add(qd_config.username);
    elm_object_part_text_set(name_obj, "default", name);
    evas_object_size_hint_align_set(name_obj, 0.0, 0.0);
    evas_object_size_hint_weight_set(name_obj, EVAS_HINT_EXPAND, 0.0); 
    evas_object_show(name_obj);
    elm_box_pack_end(box, name_obj);

    elm_object_part_content_set(layout, "default", box);
    evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, 0.0);

    hd = ecore_event_handler_add(QD_EVENT_USER_NAME_CHANGED, qd_view_set_user_name_ev_hd_cb, name_obj);
    evas_object_event_callback_add(name_obj, EVAS_CALLBACK_DEL, qd_view_set_user_ev_handler_del_cb, hd);

    return layout;
}


void qd_view_games_list_page_show(void)
{
    Elm_Object_Item *bot_it;
    bot_it = elm_naviframe_bottom_item_get(view.layout);
    elm_naviframe_item_pop_to (bot_it);
}

void qd_view_preferences_page_show(void)
{
    qd_view_preferences_page_add();
    elm_naviframe_item_push(view.layout, "Preferences", NULL, NULL, view.preferences.layout, NULL);
}

void qd_view_game_stat_page_show(void *data)
{
    Evas_Object* page_layout;
    // should get opponent name
    page_layout = qd_view_game_stat_page_add(data);
    elm_naviframe_item_push(view.layout, "game against", NULL, NULL, page_layout, NULL);
}

void qd_view_category_page_show(void)
{
    
    elm_naviframe_item_push(view.layout, "Select a category", NULL, NULL, view.category.layout, NULL);
}

void qd_view_question_page_show(void)
{
    elm_naviframe_item_push(view.layout, "", NULL, NULL, view.category.layout, NULL);
}

void qd_view_login_page_show(void)
{
    qd_view_login_page_add();
    elm_naviframe_item_push(view.layout, "login", NULL, NULL, view.login.layout, NULL);
}

void qd_view_new_game_page_show(void)
{
    qd_view_new_game_page_add();
    elm_naviframe_item_push(view.layout, "new_game", NULL, NULL, view.new_game.layout, NULL);
}


void _on_exit_cb(void *data, Evas_Object *obj, void *event_info)
{
    elm_exit();
}

int qd_view_main_win_add()
{
    view.win = elm_win_util_standard_add("Quizduell Window", "Quizduell");
    elm_win_focus_highlight_enabled_set(view.win, EINA_TRUE);
    evas_object_smart_callback_add(view.win, "delete,request", _on_exit_cb, NULL);
    
    evas_object_size_hint_weight_set(view.win, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view.win, EVAS_HINT_FILL, EVAS_HINT_FILL);

    view.layout = elm_naviframe_add(view.win);
    //elm_naviframe_content_preserve_on_pop_set(view.layout, EINA_TRUE);
    evas_object_show(view.layout);

    evas_object_size_hint_weight_set(view.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view.layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(view.win, view.layout);

    evas_object_resize(view.win, 480, 640);
    evas_object_show(view.win);

    return 0;
}

int qd_view_init(int argc, char **argv)
{
    eina_init();
    ecore_init();
    evas_init();
    elm_init(argc, argv);
    qd_view_main_win_add();
    QD_EVENT_USER_NAME_CHANGED = ecore_event_type_new();
    view.user_ind.layout = qd_view_user_indicator_add(view.win);
    qd_view_games_list_page_add();
    elm_naviframe_item_push(view.layout, "Quizduell", view.games_list.reload_btn,
                            view.games_list.pref_btn, view.games_list.layout, NULL);
    return 0;
}

void qd_view_shutdown(void)
{
    elm_shutdown();
    evas_shutdown();
    ecore_shutdown();
    eina_shutdown();
}