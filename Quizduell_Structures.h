#ifndef _QUIZDUELL_STRUCTURES_H_
#define _QUIZDUELL_STRUCTURES_H_
#include <time.h>
#include <Eina.h>

#define NO_ROUNDS_PER_GAME 6
#define NO_CAT_CHOICES 3
#define NO_QUESTIONS_PER_ROUND 3

typedef unsigned long Qd_Game_Id;
typedef unsigned long Qd_User_Id;
typedef unsigned long Qd_Question_Id;

typedef struct {
    Eina_Stringshare *title;
    Eina_Stringshare *msg;
} Qd_Server_Message;

typedef enum
{
    QD_CATEGORY_WUNDER_DER_TECHNIK = 1,
    QD_CATEGORY_BUECHER_UND_WOERTER = 2,
    QD_CATEGORY_ESSEN_UND_TRINKEN = 3,
    QD_CATEGORY_GLAUBE_UND_RELIGION = 4,
    QD_CATEGORY_RUND_UM_DIE_WELT = 5,
    QD_CATEGORY_ZEUGEN_DER_ZEIT = 6,
    QD_CATEGORY_IM_LABOR = 7,
    QD_CATEGORY_COMICS = 8,
    QD_CATEGORY_DRAUSSEN_IM_GRUENEN = 9,
    QD_CATEGORY_KOERPER_UND_GEIST = 10,
    QD_CATEGORY_SPORT_UND_FREIZEIT = 11,
    QD_CATEGORY_MUSIK_UND_HITS = 12,
    QD_CATEGORY_MACHT_UND_GELD = 13,
    QD_CATEGORY_COMPUTERSPIELE = 14,
    QD_CATEGORY_DIE_2000 = 15,
    QD_CATEGORY_UNBEKANNT_1 = 16,
    QD_CATEGORY_TV_SERIEN = 17,
    QD_CATEGORY_UNBEKANNT_2 = 18,
    QD_CATEGORY_MEDIEN_UND_UNTERHALTUNG = 19,
    QD_CATEGORY_KUNST_UND_KULTUR = 20,
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
    QD_GAME_STATE_PLAYING = 1,
    QD_GAME_STATE_FINISHED = 2,
    QD_GAME_STATE_GIVEN_UP = 5
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
    int cat_choices[NO_ROUNDS_PER_GAME];
    int elapsed_min;
    Qd_Game_Id game_id;
    Qd_User_Id give_up_player_id;
    Eina_List *messages;
    Qd_Player *opponent;
    int opponent_answers[NO_ROUNDS_PER_GAME][NO_QUESTIONS_PER_ROUND];
    Qd_Question* questions[NO_ROUNDS_PER_GAME][NO_CAT_CHOICES][NO_QUESTIONS_PER_ROUND];
    int rating_bonus;
    Qd_Game_State state;
    Eina_Bool you_gave_up;
    int your_answers[NO_ROUNDS_PER_GAME][NO_QUESTIONS_PER_ROUND];
    Eina_Bool your_turn;
    unsigned int round;
} Qd_Game_Info;

extern const int QD_INVALID_VALUE;
extern const Qd_User_Id QD_USER_ID_UNDEFINED;

void qd_server_message_free(Qd_Server_Message *msg);
void qd_message_free(Qd_Message *m);
void qd_player_free(Qd_Player *p);
void qd_game_info_free(Qd_Game_Info *game);

#endif
