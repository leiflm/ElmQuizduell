#include <Eina.h>

#define NO_ROUNDS_PER_GAME 6

// append to list items
typedef unsigned long Qd_Game_Id;

typedef enum
{
    QD_CATEGORY_BLA = 0
} Qd_Category;

typedef struct
{
    Eina_Stringshare *user_id;
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
    Qd_Category cat_choices[NO_ROUNDS_PER_GAME];
    int elapsed_min;
    Qd_Game_Id game_id;
    Eina_List *messages;
    Qd_Player opponent;
    unsigned int opponent_answers[NO_ROUNDS_PER_GAME][3];
    int rating_bonus;
    Qd_Game_State state;
    int your_answers[NO_ROUNDS_PER_GAME][3];
    Eina_Bool your_turn;
} Qd_Game_Info;


