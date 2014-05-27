#include "Quizduell_View_Private.h"

typedef struct
{
    Qd_Question *quest;
    int *your_answer;
    int *opponent_answer;
    Eina_Stringshare *opp_name;
    Evas_Object *parent;
} Qd_Question_Full;

Evas_Object *qd_view_game_stat_points_ind_add(Evas_Object *parent, Qd_Game_Info *game)
{
    Evas_Object *layout;
    char buf[32];
    int i,j, yp = 0, op = 0;

    layout = elm_label_add(parent);

    for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
    {
        for (j = 0; j < NO_QUESTIONS_PER_ROUND; j++)
        {
            if (game->your_answers[i][j] == 0)
                yp++;
            if ((game->your_answers[i][j] != QD_INVALID_VALUE) &&
                    (game->opponent_answers[i][j] == 0))
                op++;
        }
    }
    snprintf(buf, 31, "<style font_size=30><b>%i - %i</b>", yp, op);
    elm_object_part_text_set(layout, "default", buf);
    EXPAND_AND_FILL(layout);
    evas_object_show(layout);

    return layout;
}

Evas_Object *qd_view_game_stat_ind_add(Evas_Object *parent, Qd_Player *pl)
{
    Evas_Object *label, *frame;
    frame = elm_frame_add(parent);
    evas_object_size_hint_align_set(frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(frame, EVAS_HINT_EXPAND, 0.0);
    evas_object_show(frame);

    label = elm_label_add(frame);
    elm_object_part_content_set(frame, "default", label);
    if (pl)
        elm_object_part_text_set(label, "default", pl->name);
    evas_object_show(label);

    return frame;
}

static void _qd_view_game_stat_question_popup_ok_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    Evas_Object *p = (Evas_Object *) data;
    evas_object_hide(p);
    evas_object_del(p);
}
static void _qd_view_game_stat_icon_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    Qd_Question_Full *qf = (Qd_Question_Full *) data;
    Evas_Object *popup, *btn;
#define LEN 1024
    char textbuf[LEN];
    int cur = 0;
    Eina_Stringshare *ans[4];
    ans[0] = qf->quest->correct;
    ans[1] = qf->quest->wrong[0];
    ans[2] = qf->quest->wrong[1];
    ans[3] = qf->quest->wrong[2];

    popup = elm_popup_add(qf->parent);

    //cur = snprintf(textbuf, LEN, "Category: %s<br>", qf->quest->cat_name);
    elm_object_part_text_set(popup, "title,text", qf->quest->cat_name);
    cur += snprintf(textbuf + cur, LEN - cur, "<b>%s</b><br>", qf->quest->question);
    cur += snprintf(textbuf + cur, LEN - cur, "Your answer: %s<br>", ans[*qf->your_answer]);

    if (*(qf->opponent_answer) != QD_INVALID_VALUE)
        cur += snprintf(textbuf + cur, LEN - cur, "Opponent answer: %s<br>", ans[*qf->opponent_answer]);
    else
        cur += snprintf(textbuf + cur, LEN - cur, "Opponent answer:<br>");

    cur += snprintf(textbuf + cur, LEN - cur, "Correct answer: %s<br>", ans[0]);
    elm_object_part_text_set(popup, "default", textbuf);
    btn = elm_button_add(popup);
    elm_object_part_content_set(popup, "button1", btn);
    elm_object_part_text_set(btn, "default", "OK");
    evas_object_smart_callback_add(btn, "clicked", _qd_view_game_stat_question_popup_ok_clicked_cb, popup);

    evas_object_show(popup);
#undef LEN
}

static void _qd_view_game_stat_info_free_cb(void *data, Evas *e, Evas_Object* obj, void *ev)
{
    free(data);
}

Evas_Object *qd_view_game_stat_res_add(Evas_Object *parent, int ans[3], Qd_Game_Info *game, int round)
{
    Evas_Object *box, *ic;
    int i, cat_choice;
    Qd_Question_Full *qf;
    box = elm_box_add(parent);
    elm_box_horizontal_set(box, EINA_TRUE);
    EXPAND_AND_FILL(box);
    cat_choice = game->cat_choices[round];


    for (i = 0; i < 3; i++)
    {
        ic = elm_icon_add(box);
        // did opponent play this round, but we didn't?
        if ((game->your_answers[round][i] == QD_INVALID_VALUE) &&
                (game->opponent_answers[round][i] != QD_INVALID_VALUE) &&
                (ans == game->opponent_answers[round]))
        {
            elm_icon_standard_set(ic, "dialog-question");
            game->round = round;
        }
        else
        {
            // there is no valid answer for this round
            if (ans[i] != QD_INVALID_VALUE)
            {
                if (ans[i] == 0)
                {
                    elm_icon_standard_set(ic, "info");
                }
                else
                {
                    elm_icon_standard_set(ic, "error");
                }
                if (cat_choice != QD_INVALID_VALUE)
                {
                    qf = calloc(1, sizeof(Qd_Question_Full));
                    qf->quest = game->questions[round][cat_choice][i];
                    qf->your_answer = &game->your_answers[round][i];
                    qf->opponent_answer = &game->opponent_answers[round][i];
                    qf->parent = parent;
                    evas_object_smart_callback_add(ic, "clicked", _qd_view_game_stat_icon_clicked_cb, (void *) qf);
                    evas_object_event_callback_add(ic, EVAS_CALLBACK_DEL, _qd_view_game_stat_info_free_cb, qf);
                }
            }
        }
        EXPAND_AND_FILL(ic);
        evas_object_show(ic);
        elm_box_pack_end(box, ic);
    }


    evas_object_show(box);
    return box;
}

Evas_Object * qd_view_game_stat_cat_add(Evas_Object *parent, const char *cat_name, int round)
{
    Evas_Object *layout, *lb_r, *lb_c;
    char r[32];
    snprintf(r, 31, "<b>Round %i</b>", (round + 1));

    layout = elm_box_add(parent);
    lb_r = elm_label_add(layout);
    elm_object_part_text_set(lb_r, "default", r);
    evas_object_show(lb_r);
    EXPAND_AND_FILL(lb_r);
    elm_box_pack_end(layout, lb_r);
    lb_c = elm_label_add(layout);
    elm_object_part_text_set(lb_c, "default", cat_name);
    evas_object_show(lb_c);
    EXPAND_AND_FILL(lb_c);
    elm_box_pack_end(layout, lb_c);

    evas_object_show(layout);

    return layout;
}


void qd_view_game_stat_play_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    Qd_Game_Info *game = (Qd_Game_Info *) data;

    if (game->state > QD_GAME_STATE_PLAYING)
    {
        qd_ctrl_games_game_create(game->opponent->user_id);
    }
    else
    {
        printf("play round %i \n", game->round);
        if (game->your_turn && (game->opponent_answers[game->round][0] == QD_INVALID_VALUE))
        {
            qd_view_category_page_show(game);
        }
        else
        {
            qd_view_question_page_show(game);
        }
    }
}

void qd_view_game_stat_retire_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    printf("cant give up yet\n");
}

void qd_view_game_stat_calc_round(Qd_Game_Info *game)
{
    int i;
    for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
    {
        if ((game->your_answers[i][0] == QD_INVALID_VALUE) ||
            (game->opponent_answers[i][0] == QD_INVALID_VALUE))
            break;
    }
    game->round = i;
}
Evas_Object *qd_view_game_stat_page_add(Evas_Object *parent, Qd_Game_Info *game)
{
    Evas_Object *layout, *user_ind, *points, *opp_ind, *game_res, *game_cat, *retire_btn, *pl_btn;
    int i;
    layout = elm_table_add(parent);

    user_ind = qd_view_game_stat_ind_add(layout, player);
    EXPAND_AND_FILL(user_ind);
    elm_table_pack(layout, user_ind, 0, 0, 1, 1);

    points = qd_view_game_stat_points_ind_add(layout, game);
    elm_table_pack(layout, points, 1, 0, 1, 1);

    opp_ind = qd_view_game_stat_ind_add(layout, game->opponent);
    EXPAND_AND_FILL(opp_ind);
    elm_table_pack(layout, opp_ind, 2, 0, 1, 1);

    for (i = 0; i < NO_ROUNDS_PER_GAME; i++)
    {
        game_res = qd_view_game_stat_res_add(layout, game->your_answers[i], game, i);
        EXPAND_AND_FILL(game_res);
        elm_table_pack(layout, game_res, 0, (i+1), 1, 1);
        if (game->cat_choices[i] != QD_INVALID_VALUE)
            game_cat = qd_view_game_stat_cat_add(layout, game->questions[i][game->cat_choices[i]][0]->cat_name, i);
        else
            game_cat = qd_view_game_stat_cat_add(layout, NULL, i);

        EXPAND_AND_FILL(game_cat);
        elm_table_pack(layout, game_cat, 1, (i+1), 1, 1);
        game_res = qd_view_game_stat_res_add(layout, game->opponent_answers[i], game, i);
        EXPAND_AND_FILL(game_res);
        elm_table_pack(layout, game_res, 2, (i+1), 1, 1);
        
    }
    
    qd_view_game_stat_calc_round(game);

    // add play button
    pl_btn = elm_button_add(layout);
    evas_object_size_hint_align_set(pl_btn, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(pl_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_table_pack(layout, pl_btn, 1, NO_ROUNDS_PER_GAME + 2, 1, 1);
    evas_object_smart_callback_add(pl_btn, "clicked", qd_view_game_stat_play_clicked_cb, (void *) game);
    if (game->state <= QD_GAME_STATE_PLAYING)
    {
        elm_object_part_text_set(pl_btn, "default", "Play");
        if (!game->your_turn)
            elm_object_disabled_set(pl_btn, EINA_TRUE);
    }
    else
        elm_object_part_text_set(pl_btn, "default", "Again");

    evas_object_show(pl_btn);

    // add play button
    retire_btn = elm_button_add(layout);
    evas_object_size_hint_align_set(retire_btn, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(retire_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_table_pack(layout, retire_btn, 0, NO_ROUNDS_PER_GAME + 2, 1, 1);
    evas_object_smart_callback_add(retire_btn, "clicked", qd_view_game_stat_retire_clicked_cb, NULL);
    elm_object_part_text_set(retire_btn, "default", "Give up");
    if (game->state > QD_GAME_STATE_PLAYING)
        elm_object_disabled_set(retire_btn, EINA_TRUE);
    evas_object_show(retire_btn);



    return layout;
}

void qd_view_game_stat_page_refresh_and_pop_to(Qd_Game_Info *game)
{
    Evas_Object* layout;
    Eina_List *items;
    Elm_Object_Item *it;
    layout = qd_view_game_stat_page_add(view.layout, game);
    items = elm_naviframe_items_get(view.layout);

    it = eina_list_nth(items, 1);
    elm_object_item_part_content_set(it, "default", layout);
    elm_naviframe_item_pop_to(it);
}
