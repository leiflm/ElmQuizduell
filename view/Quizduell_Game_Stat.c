#include "Quizduell_View_Private.h"

Evas_Object *qd_view_game_stat_ind_add(Evas_Object *parent, Qd_Player *pl)
{
    Evas_Object *label, *frame;
    frame = elm_frame_add(parent);
    evas_object_size_hint_align_set(frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(frame, EVAS_HINT_EXPAND, 0.0);
    evas_object_show(frame);


    label = elm_label_add(frame);
    elm_object_part_content_set(frame, "default", label);
    elm_object_part_text_set(label, "default", pl->name);
    evas_object_show(label);


    return frame;

}

Evas_Object *qd_view_game_stat_res_add(Evas_Object *parent)
{
    Evas_Object *lb;
    lb = elm_label_add(parent);
    elm_object_part_text_set(lb, "default", "text");
    evas_object_size_hint_weight_set(lb, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(lb, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(lb);
    return lb;
}


void qd_view_game_stat_play_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    printf("play\n");
    //qd_view_category_page_show();
    qd_view_question_page_show();
}

void qd_view_game_stat_retire_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    printf("cant retire\n");
}

Evas_Object *qd_view_game_stat_page_add(Evas_Object *parent, Qd_Game_Info *game)
{
    Evas_Object *layout, *user_ind, *opp_ind, *game_res, *retire_btn, *pl_btn;
    int i;
    layout = elm_table_add(parent);

    user_ind = qd_view_game_stat_ind_add(layout, &player);
    evas_object_size_hint_weight_set(user_ind, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(user_ind, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_table_pack(layout, user_ind, 0, 0, 1, 1);
    opp_ind = qd_view_game_stat_ind_add(layout, &game->opponent);
    evas_object_size_hint_weight_set(opp_ind, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(opp_ind, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_table_pack(layout, opp_ind, 2, 0, 1, 1);

    for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
    {
        game_res = qd_view_game_stat_res_add(layout);
        elm_table_pack(layout, game_res, 0, (i+1), 1, 1);
        game_res = qd_view_game_stat_res_add(layout);
        elm_table_pack(layout, game_res, 2, (i+1), 1, 1);
        
    }

    // add play button
    pl_btn = elm_button_add(layout);
    evas_object_size_hint_align_set(pl_btn, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(pl_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_part_text_set(pl_btn, "default", "Play");
    elm_table_pack(layout, pl_btn, 1, NO_ROUNDS_PER_GAME + 2, 1, 1);
    evas_object_smart_callback_add(pl_btn, "clicked", qd_view_game_stat_play_clicked_cb, NULL);
    evas_object_show(pl_btn);

    // add play button
    retire_btn = elm_button_add(layout);
    evas_object_size_hint_align_set(retire_btn, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(retire_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_table_pack(layout, retire_btn, 0, NO_ROUNDS_PER_GAME + 2, 1, 1);
    evas_object_smart_callback_add(retire_btn, "clicked", qd_view_game_stat_retire_clicked_cb, NULL);
    elm_object_part_text_set(retire_btn, "default", "Retire");
    evas_object_show(retire_btn);



    return layout;
}
