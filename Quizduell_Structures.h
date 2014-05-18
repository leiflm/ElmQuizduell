#ifndef _QUIZDUELL_STRUCTURES_H_
#define _QUIZDUELL_STRUCTURES_H_
#include <time.h>
#include <Eina.h>

#define NO_ROUNDS_PER_GAME 6
extern int QD_INVALID_VALUE;

typedef unsigned long Qd_Game_Id;
typedef unsigned long Qd_User_Id;
typedef unsigned long Qd_Question_Id;

typedef enum
{
    QD_CATEGORY_BLA = 0
} Qd_Category;

typedef struct
{
    Qd_User_Id user_id;
    Eina_Stringshare *name;
    unsigned int avatar;
} Qd_Player;

typedef enum
{
    QD_GAME_STATE_NONE = 0,
    QD_GAME_STATE_PLAYING,
    QD_GAME_STATE_FINISHED
} Qd_Game_State;

typedef struct
{
    Eina_Stringshare *text;
    //time_t created_at; not used yet
    Qd_User_Id from;
    Eina_Stringshare *id;
    Qd_User_Id to;
} Qd_Message;

typedef struct
{
    Qd_Category cat_id;
    Eina_Stringshare *cat_name;
    Eina_Stringshare *correct;
    Qd_Question_Id q_id;
    Eina_Stringshare *question;
    //time_t timestamp; not used yet
    Eina_Stringshare* wrong[3];
} Qd_Question;

typedef struct
{
    Qd_Category cat_choices[NO_ROUNDS_PER_GAME];
    int elapsed_min;
    Qd_Game_Id game_id;
    Eina_List *messages;
    Qd_Player opponent;
    int opponent_answers[NO_ROUNDS_PER_GAME][3];
    Qd_Question* questions[NO_ROUNDS_PER_GAME][3];
    int rating_bonus;
    Qd_Game_State state;
    int your_answers[NO_ROUNDS_PER_GAME][3];
    Eina_Bool your_turn;
} Qd_Game_Info;

void qd_message_free(Qd_Message *m);
void qd_player_free(Qd_Player *p);
void qd_game_info_free(Qd_Game_Info *game);

#endif
