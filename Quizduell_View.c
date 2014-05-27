#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>

#include "Quizduell_Config.h"
#include "view/Quizduell_View_Private.h"


static void _qd_view_stringshare_free_cb(void *user_data, void *func_data)
{
    eina_stringshare_del(user_data);
}

void qd_view_user_name_set(Eina_Stringshare *name)
{
    name = eina_stringshare_add(name);
    ecore_event_add(QD_EVENT_USER_NAME_CHANGED, (void*)name, _qd_view_stringshare_free_cb, (void*)name);
}

Eina_Bool qd_view_set_user_name_ev_hd_cb(void *data, int type, void *ev)
{
    Evas_Object *tp = (Evas_Object *) data;
    char *text = (char *) ev;
    //printf("setting new names\n");

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
    if (player)
        elm_object_part_text_set(name_obj, "default", player->name);
    else
        elm_object_part_text_set(name_obj, "default", "nobody");

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
    if (eina_list_count(elm_naviframe_items_get(view.layout)) > 1)
    {
        bot_it = elm_naviframe_bottom_item_get(view.layout);
        elm_naviframe_item_pop_to (bot_it);
    }
}

void qd_view_preferences_page_show(void)
{
    qd_view_preferences_page_add();
    elm_naviframe_item_push(view.layout, "Preferences", NULL, NULL, view.preferences.layout, NULL);
}

void qd_view_game_stat_page_show(Qd_Game_Info *game)
{
    Evas_Object* page_layout;
    // should get opponent name
    page_layout = qd_view_game_stat_page_add(view.win, game);
    elm_naviframe_item_push(view.layout, "game against", NULL, NULL, page_layout, NULL);
}

void qd_view_category_page_show(Qd_Game_Info *game)
{
    Evas_Object* page_layout;
    Elm_Object_Item *it;
    char *title;
    page_layout = qd_view_category_page_add(view.win, game);
    title = qd_view_category_title_get(game);
    
    elm_naviframe_prev_btn_auto_pushed_set(view.layout, EINA_FALSE);
    it = elm_naviframe_item_push(view.layout, title, NULL, NULL, page_layout, NULL);
    elm_naviframe_prev_btn_auto_pushed_set(view.layout, EINA_TRUE);
    free(title);
    //elm_naviframe_item_title_enabled_set(it, EINA_FALSE, EINA_FALSE);
}

void qd_view_question_page_show(Qd_Game_Info *game)
{
    Evas_Object* page_layout, *title_obj_l;
    Elm_Object_Item *it;
    char *title;
    // should get data 
    title_obj_l = qd_view_question_title_score_ind_add(view.win, game);
    page_layout = qd_view_question_page_add(view.win, game, title_obj_l);
    title = qd_view_category_title_get(game);
    elm_naviframe_prev_btn_auto_pushed_set(view.layout, EINA_FALSE);
    it = elm_naviframe_item_push(view.layout, title, title_obj_l, NULL, page_layout, NULL);
    elm_naviframe_prev_btn_auto_pushed_set(view.layout, EINA_TRUE);
    free(title);
    //elm_naviframe_item_title_enabled_set(it, EINA_FALSE, EINA_FALSE);
}

void qd_view_login_page_show(void)
{
    qd_view_login_page_add();
    elm_naviframe_item_push(view.layout, "login", NULL, NULL, view.login.layout, NULL);
}

void qd_view_new_game_page_show(Eina_List *friends)
{
    Evas_Object *layout;
    layout = qd_view_new_game_page_add(view.win, friends);
    elm_naviframe_item_push(view.layout, "new game", NULL, NULL, layout, NULL);
}

void qd_view_search_player_page_show(void)
{
    Evas_Object *layout;
    layout = qd_view_search_player_page_add(view.win);
    elm_naviframe_item_push(view.layout, "Search player", NULL, NULL, layout, NULL);
}

void qd_view_message_ok_btn_clicked_cb(void *data, Evas_Object* obj, void *event)
{
    Evas_Object *popup = (Evas_Object *) data;
    evas_object_hide(popup);
    evas_object_del(popup);
}

void qd_view_info_message_show(const char *title, const char *msg)
{
    Evas_Object *popup, *button;
    popup = elm_popup_add(view.layout);
    elm_object_part_text_set(popup, "title,text", title);
    elm_object_part_text_set(popup, "default", msg);
    button = elm_button_add(popup);
    elm_object_part_text_set(button, "default", "OK");
    evas_object_smart_callback_add(button, "clicked", qd_view_message_ok_btn_clicked_cb, popup);
    elm_object_part_content_set(popup, "button1", button);
    evas_object_show(popup);
}


void _on_exit_cb(void *data, Evas_Object *obj, void *event_info)
{
    elm_exit();
}

int qd_view_main_win_add()
{
    Evas_Object *ic;
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

    ic = elm_icon_add(view.layout);
    elm_icon_standard_set(ic, "process-working");
    evas_object_show(ic);

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

void qd_view_simple_evas_free_cb(void *data, Evas *e, Evas_Object *obj, void *ev)
{
    free(data);
}
