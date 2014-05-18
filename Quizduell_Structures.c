#include <Eina.h>
#include "Quizduell_Structures.h"

int QD_INVALID_VALUE = -1;

void qd_message_free(Qd_Message *m) {
    eina_stringshare_del(m->text);
    eina_stringshare_del(m->id);
    free(m);
}

void qd_player_free(Qd_Player *p) {
    eina_stringshare_del(p->name);
}

void qd_question_free(Qd_Question *q)
{
    if (!q)
    {
        return;
    }
    eina_stringshare_del(q->cat_name);
    eina_stringshare_del(q->correct);
    eina_stringshare_del(q->question);
    eina_stringshare_del(q->wrong[0]);
    eina_stringshare_del(q->wrong[1]);
    eina_stringshare_del(q->wrong[2]);
    free(q);
}

void qd_game_info_free(Qd_Game_Info *game)
{
    Qd_Message *m;

    EINA_LIST_FREE(game->messages, m)
    {
        qd_message_free(m);
    }
    qd_player_free(&(game->opponent));
}