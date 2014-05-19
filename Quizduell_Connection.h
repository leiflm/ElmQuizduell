#ifndef __QUIZDUELL_CONNECTION_H__
#define __QUIZDUELL_CONNECTION_H__

#include <Eina.h>
#include <Ecore.h>
#include <Ecore_Con.h>
#include "Quizduell_Connection.h"

extern int QD_CON_USERS_CREATE;
extern int QD_CON_USERS_UPDATE;
extern int QD_CON_USERS_LOGIN;
extern int QD_CON_USERS_LOGOUT;
extern int QD_CON_USERS_CURRENT_GAME_INFO;
extern int QD_CON_GAMES_SPECIFIC_GAME_INFO;
extern int QD_CON_GAMES_UPLOAD_ROUND_ANSWERS;

Eina_Bool qd_con_init(void);
void qd_con_shutdown(void);

/**
 @brief Create and send a request with given parameters
 @param api_url The API url starting with a '/'
 @param params_hashlist a hashset that will translated to post params and eventually hashed for authorization
 @param use_post Determines whether the request is done via post or get
 @return EINA_TRUE on sucess, EINA_FALSE on failure
 */
Eina_Bool qd_con_request_with_params(const char *api_url, const Eina_Hash *params_hashlist, int type, Eina_Bool use_post);

#endif