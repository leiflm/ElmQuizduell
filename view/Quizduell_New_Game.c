#include "Quizduell_View_Private.h"

static void qd_view_new_game_clicked_rand_cb(void *data, Evas_Object *btn, void *event_info)
{
    qd_ctrl_game_new_random_player();
}

static void qd_view_new_game_clicked_search_cb(void *data, Evas_Object *btn, void *event_info)
{
    printf("Search player\n");
    qd_view_search_player_page_show();
}

static void qd_view_new_game_friend_selected_cb(void *data, Evas_Object *obj, void *ev)
{
    Qd_Player *pl = (Qd_Player *) data;
    qd_ctrl_games_game_create(pl->user_id);
}

Evas_Object *qd_view_new_game_page_add(Evas_Object *parent, Eina_List *friends)
{
    Evas_Object *user_ind, *list, *frame, *ic;
    Eina_List *l;
    Qd_Player *pl;
    view.new_game.layout = elm_box_add(view.win);
    evas_object_size_hint_align_set(view.new_game.layout, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(view.new_game.layout, EVAS_HINT_EXPAND, 0.0);

    user_ind = qd_view_user_indicator_add(view.new_game.layout);
    elm_box_pack_end(view.new_game.layout, user_ind);
    evas_object_show(user_ind);

    // button random player 
    view.new_game.rand_btn = elm_button_add(view.new_game.layout);
    elm_object_part_text_set(view.new_game.rand_btn, "default", "Random player");
    evas_object_size_hint_weight_set(view.new_game.rand_btn, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.new_game.rand_btn, EVAS_HINT_FILL, 0.0);
    evas_object_show(view.new_game.rand_btn);
    elm_box_pack_end(view.new_game.layout, view.new_game.rand_btn);
    evas_object_smart_callback_add(view.new_game.rand_btn, "clicked", qd_view_new_game_clicked_rand_cb, NULL);

    // search player
    view.new_game.search_btn = elm_button_add(view.new_game.layout);
    elm_object_part_text_set(view.new_game.search_btn, "default", "search for player");
    evas_object_size_hint_weight_set(view.new_game.search_btn, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.new_game.search_btn, EVAS_HINT_FILL, 0.0);
    evas_object_smart_callback_add(view.new_game.search_btn, "clicked", qd_view_new_game_clicked_search_cb, NULL);
    evas_object_show(view.new_game.search_btn);
    elm_box_pack_end(view.new_game.layout, view.new_game.search_btn);
    evas_object_show(view.new_game.rand_btn);

    // friends list
    frame = elm_frame_add(view.new_game.layout);
    EXPAND_AND_FILL(frame);
    evas_object_show(frame);
    elm_object_part_text_set(frame, NULL, "Friends");
    list = elm_list_add(view.new_game.layout);
    EXPAND_AND_FILL(list);
    evas_object_show(list);

    EINA_LIST_FOREACH(friends, l, pl)
    {
        ic = elm_icon_add(list);
        elm_icon_standard_set(ic, "Add");
        elm_list_item_append(list, pl->name, ic, NULL, qd_view_new_game_friend_selected_cb, pl);
    }

    elm_object_part_content_set(frame, NULL, list);
    elm_list_go(list);
    elm_box_pack_end(view.new_game.layout, frame);


    return view.new_game.layout;
}

typedef struct
{
    Evas_Object *popup;
    Qd_Game_Info *game;
} Qd_Challenge_Popup;

static void qd_view_new_game_clicked_accept_cb(void *data, Evas_Object* obj, void *event)
{
    Qd_Challenge_Popup* cp = (Qd_Challenge_Popup *) data;
    cp->game->state = QD_GAME_STATE_PLAYING;
    cp->game->your_turn = EINA_TRUE;
    qd_view_games_list_active_item_add(cp->game);

    evas_object_hide(cp->popup);
    evas_object_del(cp->popup);
}

static void qd_view_new_game_clicked_decline_cb(void *data, Evas_Object* obj, void *event)
{
    Qd_Challenge_Popup* cp = (Qd_Challenge_Popup *) data;

    evas_object_hide(cp->popup);
    evas_object_del(cp->popup);
}

void qd_view_new_game_challenge_popup(Qd_Game_Info *game)
{
    Evas_Object *accept_btn, *decline_btn;
    char msg[128];
    Qd_Challenge_Popup *cp = calloc(1, sizeof(Qd_Challenge_Popup *));

    cp->game = game;
    snprintf(msg, 127, "%s wants to challenge you!", game->opponent->name);

    cp->popup = elm_popup_add(view.games_list.layout);
    elm_object_part_text_set(cp->popup, "title,text", "Challenge");
    elm_object_part_text_set(cp->popup, "default", msg);
    accept_btn = elm_button_add(cp->popup);
    elm_object_part_text_set(accept_btn, "default", "Accept");
    evas_object_smart_callback_add(accept_btn, "clicked", qd_view_new_game_clicked_accept_cb, cp);
    elm_object_part_content_set(cp->popup, "button1", accept_btn);

    decline_btn = elm_button_add(cp->popup);
    elm_object_part_text_set(decline_btn, "default", "Decline");
    evas_object_smart_callback_add(decline_btn, "clicked", qd_view_new_game_clicked_decline_cb, cp);
    elm_object_part_content_set(cp->popup, "button2", decline_btn);

    evas_object_event_callback_add(cp->popup, EVAS_CALLBACK_DEL, qd_view_simple_evas_free_cb, cp);

    evas_object_show(cp->popup);
}
