#include "Quizduell_View_Private.h"


void qd_view_user_data_set(char *name, Evas_Object *icon)
{

    elm_object_part_text_set(view.user_ind.name, "default", name);
    //view.user_ic = icon;
}

static void qd_view_games_list_reload_clicked_cb(void *data, Evas_Object* obj, void *ev)
{
    qd_ctrl_games_list_update();
    printf("clicked reload\n");
}

static void qd_view_games_list_pref_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    qd_view_preferences_page_show();
}

static void qd_view_games_list_new_game_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    qd_view_new_game_page_show(NULL);
}

int qd_view_games_list_page_add(void)
{
    Evas_Object *frame, *scroller, *box, *ic;

    view.games_list.layout = elm_box_add(view.win);
    evas_object_size_hint_align_set(view.games_list.layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(view.games_list.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(view.games_list.layout);

    // 1.layout element: user indicator
    elm_box_pack_end(view.games_list.layout, view.user_ind.layout);
    //qd_view_obj_show_hide_with(view.games_list.layout, view.user_ind.layout);
    //evas_object_show(view.user_ind.layout);

    // box inside scroller
    // box for different list
    box = elm_box_add(view.games_list.layout);
    evas_object_show(box);
    evas_object_size_hint_align_set(box, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0);

    // add start new game button
    view.games_list.new_btn = elm_button_add(view.games_list.layout);
    ic = elm_icon_add(view.games_list.layout);
    elm_icon_standard_set(ic, "add");
    elm_object_part_content_set(view.games_list.new_btn, "icon", ic);
    elm_object_part_text_set(view.games_list.new_btn, "default", "Start a new game");
    evas_object_size_hint_weight_set(view.games_list.new_btn, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.games_list.new_btn, EVAS_HINT_FILL, 0.0);
    evas_object_smart_callback_add(view.games_list.new_btn, "clicked", qd_view_games_list_new_game_clicked_cb, NULL);
    elm_box_pack_end(box, view.games_list.new_btn);
    evas_object_show(view.games_list.new_btn);

    // add preferences button
    view.games_list.pref_btn = elm_button_add(view.games_list.layout);
    ic = elm_icon_add(view.games_list.layout);
    elm_icon_standard_set(ic, "preferences-system");
    elm_object_part_content_set(view.games_list.pref_btn, "icon", ic);
    elm_object_part_text_set(view.games_list.pref_btn, "default", "Pref.");
    evas_object_size_hint_weight_set(view.games_list.pref_btn, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.games_list.pref_btn, EVAS_HINT_FILL, 0.0);
    evas_object_smart_callback_add(view.games_list.pref_btn, "clicked", qd_view_games_list_pref_clicked_cb, NULL);

    // add reload button
    view.games_list.reload_btn = elm_button_add(view.games_list.layout);
    ic = elm_icon_add(view.games_list.layout);
    elm_icon_standard_set(ic, "view-refresh");
    elm_object_part_content_set(view.games_list.reload_btn, "icon", ic);
    elm_object_part_text_set(view.games_list.reload_btn, "default", "reload");
    evas_object_size_hint_weight_set(view.games_list.reload_btn, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.games_list.reload_btn, EVAS_HINT_FILL, 0.0);
    evas_object_smart_callback_add(view.games_list.reload_btn, "clicked", qd_view_games_list_reload_clicked_cb, NULL);

    // add a scroller for all the things (2. layout element)
    scroller = elm_scroller_add(view.win);
    elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_ON);
    evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(scroller);
    elm_box_pack_end(view.games_list.layout, scroller);
    elm_object_part_content_set(scroller, "default", box);

    
#define GAMES_LIST_ADD(_list_var, _label_text) \
    frame = elm_frame_add(view.games_list.layout); \
    evas_object_size_hint_align_set(frame, EVAS_HINT_FILL, EVAS_HINT_FILL); \
    evas_object_size_hint_weight_set(frame, EVAS_HINT_EXPAND, 0.0); \
    elm_box_pack_end(box, frame); \
    evas_object_show(frame); \
    _list_var = elm_list_add(view.games_list.layout); \
    elm_list_mode_set(_list_var, ELM_LIST_EXPAND); \
    elm_object_part_text_set(frame, "default", _label_text); \
    elm_object_part_content_set(frame, "default", _list_var); \
    elm_list_select_mode_set(_list_var, ELM_OBJECT_SELECT_MODE_ALWAYS); \
    elm_list_go(_list_var)

    GAMES_LIST_ADD(view.games_list.active_list, "Your turn");
    //elm_list_select_mode_set(view.games_list.active_list, ELM_OBJECT_SELECT_MODE_ALWAYS);
    GAMES_LIST_ADD(view.games_list.inactive_list, "Waiting for opponent");
    //elm_list_select_mode_set(view.games_list.inactive_list, ELM_OBJECT_SELECT_MODE_NONE);
    GAMES_LIST_ADD(view.games_list.done_list, "Finished games");
    //elm_list_select_mode_set(view.games_list.done_list, ELM_OBJECT_SELECT_MODE_NONE);

    evas_object_show(view.user_ind.layout);
    evas_object_hide(view.games_list.layout);
    return 0;
}

void qd_view_games_list_clear(void)
{
    elm_list_clear(view.games_list.active_list);
    elm_list_clear(view.games_list.inactive_list);
    elm_list_clear(view.games_list.done_list);
}

void qd_view_games_list_play_cb(void *data, Evas_Object *obj, void *ev)
{
    printf("wanna play?\n");
    qd_ctrl_game_details((Qd_Game_Info *) data);
}

void qd_view_games_list_active_item_add(Qd_Game_Info *game)
{
    elm_list_item_append(view.games_list.active_list, game->opponent->name, NULL, NULL, qd_view_games_list_play_cb, (void *) game);
    elm_list_go(view.games_list.active_list);
}

void qd_view_games_list_inactive_item_add(Qd_Game_Info *game)
{
    elm_list_item_append(view.games_list.inactive_list, game->opponent->name, NULL, NULL, qd_view_games_list_play_cb, (void *) game);
    elm_list_go(view.games_list.inactive_list);
}

void qd_view_games_list_done_item_add(Qd_Game_Info *game)
{
    elm_list_item_append(view.games_list.done_list, game->opponent->name, NULL, NULL, qd_view_games_list_play_cb, (void *) game);
    elm_list_go(view.games_list.done_list);
}
