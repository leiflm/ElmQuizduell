#ifndef QUIZDUELL_STRUCTURES
#define QUIZDUELL_STRUCTURES

#include <time.h>
#include <Eina.h>

#define NO_ROUNDS_PER_GAME 6
extern int QD_INVALID_VALUE;

// append to list items
typedef unsigned long Qd_Game_Id;
typedef unsigned long Qd_User_Id;

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
    time_t created_at;
    Qd_User_Id from;
    Eina_Stringshare *id;
    Qd_User_Id to;
} Qd_Message;

typedef struct
{
    Qd_Category cat_choices[NO_ROUNDS_PER_GAME];
    int elapsed_min;
    Qd_Game_Id game_id;
    Eina_List *messages;
    Qd_Player opponent;
    int opponent_answers[NO_ROUNDS_PER_GAME][3];
    int rating_bonus;
    Qd_Game_State state;
    int your_answers[NO_ROUNDS_PER_GAME][3];
    Eina_Bool your_turn;
} Qd_Game_Info;

void qd_message_free(Qd_Message *m);
void qd_player_free(Qd_Player *p);
void qd_game_info_free(Qd_Game_Info *game);

#endif