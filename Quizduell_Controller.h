#ifndef __QUIZDUELL_CONTROLLER_H__
#define __QUIZDUELL_CONTROLLER_H__

Eina_Bool qd_ctrl_init(void);
void qd_ctrl_shutdown(void);
void qd_ctrl_run(void);

void qd_ctrl_user_login(char *name, char* pw);

#endif