#include "Quizduell_View_Private.h"

char *qd_view_category_title_get(Qd_Game_Info *game)
{
    char *text = calloc(128, sizeof(char));
    snprintf(text, 127, "Round %i against %s", (game->round + 1), game->opponent->name);
    return text;
}

void qd_view_category_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    int i;
    const char *text;
    Qd_Game_Info *game = (Qd_Game_Info *) data;
    text =  elm_object_part_text_get(obj, "default");
    for (i = 0; i < NO_CAT_CHOICES; i++)
    {
        if (!strcmp(text, game->questions[game->round][i][0]->cat_name))
            break;
    }
    game->cat_choices[game->round] = i;
    printf("selected: %s\n",text);
    qd_view_question_page_show((Qd_Game_Info *) data);
}

Evas_Object *qd_view_category_page_add(Evas_Object *parent, Qd_Game_Info *game)
{
    Evas_Object *layout, *box, *btn, *opn;
    int i;

    layout = elm_frame_add(parent);
    EXPAND_AND_FILL(layout);
    box = elm_box_add(layout);
    EXPAND_AND_FILL(box);

    elm_object_part_content_set(layout, "default", box);
    elm_object_part_text_set(layout, "default", "Select a category against");

    opn = elm_label_add(layout);
    evas_object_size_hint_align_set(opn, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(opn, EVAS_HINT_EXPAND, 0);
    evas_object_show(opn);
    elm_box_pack_end(box, opn);

    for (i = 0; i < NO_CAT_CHOICES; i++)
    {
        btn = elm_button_add(layout);
        EXPAND_AND_FILL(btn);
        evas_object_show(btn);
        elm_object_part_text_set(btn, "default", game->questions[game->round][i][0]->cat_name);
        evas_object_smart_callback_add(btn, "clicked", qd_view_category_clicked_cb, game);
        elm_box_pack_end(box, btn);
    }
    
    evas_object_show(layout);

    return layout;
}
