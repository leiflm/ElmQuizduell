#ifndef __QUIZDUELL_VIEW_H__
#define __QUIZDUELL_VIEW_H__

#include <Evas.h>

int qd_view_init(int argc, char **argv);
void qd_view_shutdown(void);

void qd_view_user_name_set(char *name); 
void qd_view_games_list_page_show(void);
void qd_view_login_page_show(void);
void qd_view_new_game_page_show(void);
void qd_view_preferences_page_show(void);
void qd_view_question_page_show(void);
//void qd_view_game_stat_page_show(void);
void qd_view_game_stat_page_show(void *data);
void qd_view_info_message_show(const char *title, const char *msg);
Evas_Object *qd_view_game_stat_page_add(void *data);

void qd_view_games_list_active_item_add(char *user_name, void *data);

#endif
