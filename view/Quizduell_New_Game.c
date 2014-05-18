#include "Quizduell_View_Private.h"

static void qd_view_new_game_clicked_rand_cb(void *data, Evas_Object *btn, void *event_info)
{
    qd_ctrl_game_new_random_player();
}

static void qd_view_new_game_clicked_search_cb(void *data, Evas_Object *btn, void *event_info)
{
    printf("Search player\n");
    qd_view_info_message_show("Not implemented", "Search Player");
}

int qd_view_new_game_page_add(void)
{
    Evas_Object *user_ind;
    view.new_game.layout = elm_box_add(view.win);
    evas_object_size_hint_align_set(view.new_game.layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(view.new_game.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

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

    evas_object_hide(view.new_game.layout);
    return 0;
}
