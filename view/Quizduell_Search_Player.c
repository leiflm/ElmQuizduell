#include "Quizduell_View_Private.h"
#include "Quizduell_Controller.h"

typedef struct
{
    Evas_Object *en;
    Evas_Object *add_friend_btn;
    Evas_Object *new_game_btn;
} Qd_Search_Info;

static Qd_Search_Info *si = NULL;

static void qd_view_search_player_clicked_add_friend_cb(void *data, Evas_Object *btn, void *ev)
{
    Qd_Player *pl = (Qd_Player *) data;
    qd_ctrl_users_add_friend(pl->user_id);
}

static void qd_view_search_player_clicked_new_game_cb(void *data, Evas_Object *btn, void *ev)
{
    Qd_Player *pl = (Qd_Player *) data;
    qd_ctrl_games_game_create(pl->user_id);
}

static void qd_view_search_player_clicked_search_cb(void *data, Evas_Object *btn, void *event_info)
{
    const char *text;

    text = elm_entry_entry_get(si->en);
    if (text)
    {
       // name = eina_stringshare_add(text);
        qd_ctrl_users_find_user(text);
    }
    evas_object_hide(si->add_friend_btn);
    evas_object_smart_callback_del(si->add_friend_btn, "clicked", qd_view_search_player_clicked_add_friend_cb);
    evas_object_hide(si->new_game_btn);
    evas_object_smart_callback_del(si->new_game_btn, "clicked", qd_view_search_player_clicked_new_game_cb);
}

void qd_view_search_player_completed_cb(Qd_Player *pl)
{
    char text_buf[128];

    if (pl)
    {
        snprintf(text_buf, 127, "Add %s as friend", pl->name);
        elm_object_part_text_set(si->add_friend_btn, NULL, text_buf);
        evas_object_smart_callback_add(si->add_friend_btn, "clicked", qd_view_search_player_clicked_add_friend_cb, pl);
        evas_object_show(si->add_friend_btn);
        snprintf(text_buf, 127, "New game against %s", pl->name);
        elm_object_part_text_set(si->new_game_btn, NULL, text_buf);
        evas_object_smart_callback_add(si->new_game_btn, "clicked", qd_view_search_player_clicked_new_game_cb, pl);
        evas_object_show(si->new_game_btn);
    }
    else
    {
        qd_view_info_message_show("Search", "Not found");
    }
}

Evas_Object *qd_view_search_player_page_add(Evas_Object *parent)
{
    Evas_Object *frame, *layout, *search_btn, *ic;
    // box as layout
    layout = elm_box_add(parent);
    evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_FILL);

    si = calloc(1, sizeof(Qd_Search_Info));
    evas_object_event_callback_add(layout, EVAS_CALLBACK_DEL, qd_view_simple_evas_free_cb, si);

    // User name entry
    frame = elm_frame_add(layout);
    EXPAND_HORI_AND_FILL(frame);
    si->en = elm_entry_add(layout);
    elm_entry_single_line_set(si->en, EINA_TRUE);
    EXPAND_HORI_AND_FILL(si->en);
    evas_object_show(si->en);
    elm_object_part_content_set(frame, "default", si->en);
    elm_object_part_text_set(frame, "default", "User name");
    evas_object_show(frame);
    elm_box_pack_end(layout, frame);

    // Search button
    search_btn = elm_button_add(layout);
    elm_object_part_text_set(search_btn, "default", "search");
    ic = elm_icon_add(layout);
    elm_icon_standard_set(ic, "edit-find");
    elm_object_part_content_set(search_btn, "icon", ic);
    EXPAND_HORI_AND_FILL(search_btn);
    elm_box_pack_end(layout, search_btn);
    evas_object_show(search_btn);
    evas_object_smart_callback_add(search_btn, "clicked", qd_view_search_player_clicked_search_cb, si);

    // Add friend button
    si->add_friend_btn = elm_button_add(layout);
    EXPAND_HORI_AND_FILL(si->add_friend_btn);
    elm_box_pack_end(layout, si->add_friend_btn);
    
    // new game button
    si->new_game_btn = elm_button_add(layout);
    EXPAND_HORI_AND_FILL(si->new_game_btn);
    elm_box_pack_end(layout, si->new_game_btn);

    return layout;
}
