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

void qd_game_info_free(Qd_Game_Info *game)
{
    Qd_Message *m;

    EINA_LIST_FREE(game->messages, m)
    {
        qd_message_free(m);
    }
    qd_player_free(&(game->opponent));
}