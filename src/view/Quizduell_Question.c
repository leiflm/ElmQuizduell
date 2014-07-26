#include "Quizduell_View_Private.h"

#define SEC_PER_QUESTION 10.0
typedef struct
{
    Evas_Object *btns[4];
    Evas_Object *bar;
    Evas_Object *question;
    Evas_Object *frame;
    Evas_Object *flip;
    Ecore_Timer *timer;
    Evas_Object *score_ic[NO_QUESTIONS_PER_ROUND];
    Qd_Game_Info *game;
    int no;
} Qd_Question_Elem;

void qd_view_question_answer_clicked_cb(void *data, Evas_Object* obj, void *ev);
void qd_view_question_next_clicked_cb(void *data, Evas *e, Evas_Object *obj, void *ev);
void qd_view_question_questions_text_set(Qd_Question_Elem *qqe);
void qd_view_question_button_random_repack(Evas_Object *btns[4]);

Evas_Object *qd_view_question_title_score_ind_add(Evas_Object* parent, Qd_Game_Info *game)
{
    Evas_Object *layout, *ic;
    int i;
    layout = elm_box_add(parent);
    elm_box_horizontal_set(layout, EINA_TRUE);
    for (i = 0; i < NO_QUESTIONS_PER_ROUND; i++)
    {
       ic = elm_icon_add(layout);
       EXPAND_AND_FILL(ic);
       elm_box_pack_end(layout, ic);
       evas_object_show(ic);
    }
    evas_object_show(layout);
    return layout;
}
void qd_view_question_question_finished(Qd_Question_Elem *qqe)
{
    int i;
    // del clicked callbacks
    for (i = 0; i < 4; i++)
    {
        evas_object_smart_callback_del(qqe->btns[i], "clicked", qd_view_question_answer_clicked_cb);
    }
    evas_object_event_callback_add(qqe->frame, EVAS_CALLBACK_MOUSE_UP, qd_view_question_next_clicked_cb, qqe);
    qqe->no++;
}

Eina_Bool _animator_cb(void *data)
{
    double val, new_val, freq;
    Qd_Question_Elem *qqe = (Qd_Question_Elem *) data;
    val = elm_progressbar_value_get(qqe->bar);
    freq = ecore_animator_frametime_get();
    // should use gettimeofday etc....
    new_val = (SEC_PER_QUESTION * val - freq)/SEC_PER_QUESTION;

    if (new_val <= 0)
    {
        qd_view_question_question_finished(qqe);
        return EINA_FALSE;
    }
    elm_progressbar_value_set(qqe->bar, new_val);
    return EINA_TRUE;
}

void qd_view_question_next_clicked_cb(void *data, Evas *e, Evas_Object *obj, void *ev)
{
    Qd_Question_Elem *qqe = (Qd_Question_Elem *) data;
    int i;

    // check if last question
    if (qqe->no == 3)
    {
        // cheeck if next turn
        if ((qqe->game->opponent_answers[qqe->game->round][0] == QD_INVALID_VALUE)
            || (qqe->game->round == (NO_ROUNDS_PER_GAME - 1)))
        {
            qqe->game->your_turn = EINA_FALSE;
            // send answers to ctrl
            qd_ctrl_answers_send(qqe->game);
        }
        else
        {
            qqe->game->round++;
        }
        qd_view_game_stat_page_refresh_and_pop_to(qqe->game);

        return;
    }
    // hide question
    elm_flip_go(qqe->flip, ELM_FLIP_ROTATE_Y_CENTER_AXIS);
    for (i = 0; i < 4; i++)
    {
        evas_object_hide(qqe->btns[i]);
        evas_object_color_set(qqe->btns[i], 255,255,255, 255);
    }
    evas_object_hide(qqe->bar);
    evas_object_event_callback_del(qqe->frame, EVAS_CALLBACK_MOUSE_UP, qd_view_question_next_clicked_cb);
}

void qd_view_question_page_del_cb(void *data, Evas *e, Evas_Object *obj, void *ev)
{
    free(data);
}

void qd_view_question_answer_clicked_cb(void *data, Evas_Object* obj, void *ev)
{
    int i, ans;
    Qd_Question_Elem *qqe = (Qd_Question_Elem *) data;
    // check which was clicked
    for (i = 0; i < 4; i++)
    {
        if (obj == qqe->btns[i])
            ans = i;
    }
    ecore_animator_del(qqe->timer);

    printf("answer %i, question %i\n", ans, qqe->no);
    // check answer here or ...
    if (ans == 0)
    {
        evas_object_color_set(obj, 0, 255, 0, 255);
        elm_icon_standard_set(qqe->score_ic[qqe->no], "info");
    }
    else
    {
        evas_object_color_set(obj, 255, 0, 0, 255);
        elm_icon_standard_set(qqe->score_ic[qqe->no], "error");
    }

    qqe->game->your_answers[qqe->game->round][qqe->no] = ans;

    // show corrent answer
    evas_object_color_set(qqe->btns[0], 0, 255, 0, 255);
    qd_view_question_question_finished(qqe);
}

void qd_view_question_questions_text_set(Qd_Question_Elem *qqe)
{
    int i;
    Qd_Question *q;
    Eina_Stringshare *ans[4];

    q = qqe->game->questions[qqe->game->round][qqe->game->cat_choices[qqe->game->round]][qqe->no];

    ans[0] = q->correct;
    ans[1] = q->wrong[0];
    ans[2] = q->wrong[1];
    ans[3] = q->wrong[2];
    elm_object_part_text_set(qqe->frame, "default", q->cat_name);
    elm_object_part_text_set(qqe->question, "default", q->question);
    for (i = 0; i < 4; i++)
    {
        elm_object_part_text_set(qqe->btns[i], "default", ans[i]);
        evas_object_smart_callback_add(qqe->btns[i], "clicked", qd_view_question_answer_clicked_cb, qqe);
    }
    elm_progressbar_value_set(qqe->bar, 1.0);
    //qqe->no++;
}

void qd_view_question_reveal_clicked_cb(void *data, Evas_Object* obj, void *ev)
{
    Qd_Question_Elem *qqe = (Qd_Question_Elem *) data;
    int i;
    // set text content of objects/questions/answers
    qd_view_question_questions_text_set(qqe);
    qd_view_question_button_random_repack(qqe->btns);
    elm_flip_go(qqe->flip, ELM_FLIP_ROTATE_Y_CENTER_AXIS);
    for (i = 0; i < 4; i++)
    {
        evas_object_show(qqe->btns[i]);
    }
    evas_object_show(qqe->bar);
    qqe->timer = ecore_animator_add(_animator_cb, qqe);
}

Evas_Object *qd_view_question_page_add(Evas_Object *parent, Qd_Game_Info *game, Evas_Object *score_ic_box)
{
    Evas_Object* layout;
    Evas_Object *cat_icon, *ic;
    Qd_Question_Elem *qqe;
    Eina_List *ic_l;
    qqe = malloc(sizeof(Qd_Question_Elem));
    qqe->no = 0;
    int i = 0;
    qqe->game = game;
    ic_l = elm_box_children_get(score_ic_box);
    EINA_LIST_FREE(ic_l, ic)
    {
        qqe->score_ic[i++] = ic;
    }

    layout = elm_table_add(parent);
    evas_object_event_callback_add(layout, EVAS_CALLBACK_DEL, qd_view_simple_evas_free_cb, qqe);

    evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    qqe->frame = elm_frame_add(layout);
    cat_icon = elm_icon_add(layout);
    elm_icon_standard_set(cat_icon, "dialog-question");
    evas_object_show(cat_icon);

    qqe->question = elm_label_add(qqe->frame);
    elm_label_line_wrap_set(qqe->question, ELM_WRAP_WORD);

    elm_object_part_content_set(qqe->frame, "default", qqe->question);
    evas_object_show(qqe->question);
    evas_object_size_hint_align_set(qqe->frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(qqe->frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(qqe->frame);

    evas_object_smart_callback_add(cat_icon, "clicked", qd_view_question_reveal_clicked_cb, qqe);
    evas_object_show(cat_icon);

    // add a flip for the question, activated after
    qqe->flip = elm_flip_add(layout);

    evas_object_size_hint_align_set(qqe->flip, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(qqe->flip, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_part_content_set(qqe->flip, "front", cat_icon);
    elm_object_part_content_set(qqe->flip, "back", qqe->frame);
    evas_object_show(qqe->flip);

    elm_table_pack(layout, qqe->flip, 0, 0, 2, 1);

#define ADD_BUTTON_TO_WITH(_btn, _pos_x, _pos_y) \
    _btn = elm_button_add(layout); \
    evas_object_size_hint_align_set(_btn, EVAS_HINT_FILL, EVAS_HINT_FILL); \
    evas_object_size_hint_weight_set(_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
    elm_table_pack(layout, _btn, (_pos_x ), (_pos_y + 1), 1, 1);

    ADD_BUTTON_TO_WITH(qqe->btns[0], 0, 0);
    ADD_BUTTON_TO_WITH(qqe->btns[1], 1, 0);
    ADD_BUTTON_TO_WITH(qqe->btns[2], 0, 1);
    ADD_BUTTON_TO_WITH(qqe->btns[3], 1, 1);

#undef ADD_BUTTON_TO_WITH

    qqe->bar = elm_progressbar_add(layout);
    elm_progressbar_unit_format_set(qqe->bar, NULL);
    evas_object_size_hint_align_set(qqe->bar, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(qqe->bar, EVAS_HINT_EXPAND, 0.0);

    elm_table_pack(layout, qqe->bar, 0, 3, 2, 1);

    return layout;
}

void qd_view_question_button_random_repack(Evas_Object *btns[4])
{
    int rand_x = 0, rand_y = 0;

    rand_x = rand() % 2;
    rand_y = rand() % 2;

    elm_table_pack_set(btns[0], rand_x, rand_y + 1, 1, 1);
    elm_table_pack_set(btns[1], rand_x, abs(rand_y - 1) + 1, 1, 1);
    elm_table_pack_set(btns[2], abs(rand_x - 1), rand_y + 1, 1, 1);
    elm_table_pack_set(btns[3], abs(rand_x - 1), abs(rand_y - 1) + 1, 1, 1);
}

