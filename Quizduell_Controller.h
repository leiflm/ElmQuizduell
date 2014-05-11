#ifndef __QUIZDUELL_CONTROLLER_H__
#define __QUIZDUELL_CONTROLLER_H__

#include <Quizduell_Structures.h>

extern Qd_Player player;
extern Eina_List *games; // list of Qd_Game_Info*

Eina_Bool qd_ctrl_init(void);
void qd_ctrl_shutdown(void);
void qd_ctrl_run(void);

void qd_ctrl_user_login(char *name, char* pw);

#endif