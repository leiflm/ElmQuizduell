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
    int no;
} Qd_Question_Elem;

void qd_view_question_answer_clicked_cb(void *data, Evas_Object* obj, void *ev);
void qd_view_question_questions_text_set(Qd_Question_Elem *qqe);

void qd_view_question_question_finished(Qd_Question_Elem *qqe)
{
    int i;
    // del clicked callbacks
    for (i = 0; i < 4; i++)
    {
        evas_object_smart_callback_del(qqe->btns[i], "clicked", qd_view_question_answer_clicked_cb);
    }
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

    // check if last question
    if (qqe->no == 3)
    {
        // send answers to ctrl
        // qd_ctrl_question_answers();
        elm_naviframe_item_pop(view.layout);
        return;
    }
    // hide question
    elm_flip_go(qqe->flip, ELM_FLIP_ROTATE_Y_CENTER_AXIS);
    // set new question / answers and hide
    qd_view_question_questions_text_set(qqe);
    // reveal
}

void qd_view_question_page_del_cb(void *data, Evas *e, Evas_Object *obj, void *ev)
{
    printf("free\n");
    free(data);
}

void qd_view_question_answer_clicked_cb(void *data, Evas_Object* obj, void *ev)
{
    Qd_Question_Elem *qqe = (Qd_Question_Elem *) data;
    int i, ans;
    // check which was clicked
    for (i = 0; i < 4; i++)
    {
        if (obj == qqe->btns[i])
            ans = i;
    }
    ecore_animator_del(qqe->timer);
    qd_view_question_question_finished(qqe);

    printf("answer %i\n", ans);
    // check answer here or ...
    if (ans == 0)
        evas_object_color_set(obj, 0, 255, 0, 255);
    else
        evas_object_color_set(obj, 255, 0, 0, 255);
}

void qd_view_question_questions_text_set(Qd_Question_Elem *qqe)
{
    int i;

    elm_object_part_text_set(qqe->question, "default", "question");
    for (i = 0; i < 4; i++)
    {
        evas_object_hide(qqe->btns[i]);
        evas_object_color_set(qqe->btns[i], 255,255,255, 255);
        elm_object_part_text_set(qqe->btns[i], "default", "answer");
        evas_object_smart_callback_add(qqe->btns[i], "clicked", qd_view_question_answer_clicked_cb, qqe);
    }
    evas_object_hide(qqe->bar);
    elm_progressbar_value_set(qqe->bar, 1.0);
    qqe->no++;
}

void qd_view_question_reveal_clicked_cb(void *data, Evas_Object* obj, void *ev)
{
    Qd_Question_Elem *qqe = (Qd_Question_Elem *) data;
    int i;
    printf("clicked: %p\n", obj);
    // set text content of objects/questions/answers
    qd_view_question_questions_text_set(qqe);
    elm_flip_go(qqe->flip, ELM_FLIP_ROTATE_Y_CENTER_AXIS);
    for (i = 0; i < 4; i++)
    {
        evas_object_show(qqe->btns[i]);
    }
    evas_object_show(qqe->bar);
    qqe->timer = ecore_animator_add(_animator_cb, qqe);
}

//Evas_Object *qd_view_question_page_add(Evas_Object *parent, Qd_Question_Info_Set *qqt)
Evas_Object *qd_view_question_page_add(Evas_Object *parent)
{
    Evas_Object* layout;
    Evas_Object* front_label;
    Qd_Question_Elem *qqe;
    qqe = malloc(sizeof(Qd_Question_Elem));
    qqe->no = -1;

    layout = elm_table_add(parent);
    evas_object_event_callback_add(layout, EVAS_CALLBACK_DEL, qd_view_question_page_del_cb, qqe);

    evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);


    qqe->frame = elm_frame_add(layout);
    front_label = elm_image_add(layout);
    elm_image_file_set(front_label, "/tmp/undo.svg", NULL);

    qqe->question = elm_label_add(qqe->frame);

    elm_object_part_content_set(qqe->frame, "default", qqe->question);
    evas_object_show(qqe->question);
    evas_object_size_hint_align_set(qqe->frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(qqe->frame, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(qqe->frame);
    evas_object_event_callback_add(qqe->frame, EVAS_CALLBACK_MOUSE_UP, qd_view_question_next_clicked_cb, qqe);

    evas_object_smart_callback_add(front_label, "clicked", qd_view_question_reveal_clicked_cb, qqe);
    evas_object_show(front_label);

    // add a flip for the question, activated after
    qqe->flip = elm_flip_add(layout);

    evas_object_size_hint_align_set(qqe->flip, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(qqe->flip, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_part_content_set(qqe->flip, "front", front_label);
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
    elm_object_part_text_set(qqe->frame, "default", "category");

    return layout;
}


