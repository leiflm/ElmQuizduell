#include <Eina.h>
#include "Quizduell_Structures.h"

const int QD_INVALID_VALUE = -1;
const Qd_User_Id QD_USER_ID_UNDEFINED = 0;

void qd_message_free(Qd_Message *m) {
    eina_stringshare_del(m->text);
    eina_stringshare_del(m->id);
    free(m);
}

void qd_player_free(Qd_Player *p) {
    if (!p)
    {
        return;
    }
    eina_stringshare_del(p->name);
    free(p);
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
    int i, j, k;

    if (!game)
    {
        return;
    }

    EINA_LIST_FREE(game->messages, m)
    {
        qd_message_free(m);
    }
    qd_player_free(game->opponent);

    for (i = 0; i < NO_ROUNDS_PER_GAME; ++i)
    {
        for (j = 0; j < NO_CAT_CHOICES; ++j)
        {
            for (k = 0; k < NO_QUESTIONS_PER_ROUND; ++k)
            {
                qd_question_free(game->questions[i][j][k]);
            }
        }
    }

    free(game);
}

void qd_server_message_free(Qd_Server_Message *msg)
{
    if (!msg)
        return;
    if (msg->title)
    {
        eina_stringshare_del(msg->title);
        msg->title = NULL;
    }
    if (msg->msg)
    {
        eina_stringshare_del(msg->msg);
        msg->msg = NULL;
    }
    free (msg);
}