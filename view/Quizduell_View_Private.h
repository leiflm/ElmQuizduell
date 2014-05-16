#ifndef __QUIZDUELL_VIEW_PRIVATE_H__
#define __QUIZDUELL_VIEW_PRIVATE_H__

#include <stdio.h>
#include <string.h>
#include <Eina.h>
#include <Evas.h>
#include <Ecore.h>
#include <Elementary.h>

#include "Quizduell_View.h"
#include "Quizduell_Controller.h"
#include "Quizduell_Structures.h"

typedef struct
{
    Evas_Object *win;
    Evas_Object *layout;
    struct {
        Evas_Object *layout;
        Evas_Object *en_user;
        Evas_Object *en_pw;
        Evas_Object *ok_btn;
    } login;
    struct {
        Evas_Object *layout;
        Evas_Object *icon;
        Evas_Object *name;
    } user_ind;
    struct {
        Evas_Object *layout;
    } toolbar;
    struct {
        Evas_Object *layout;
        Evas_Object *search_btn;
        Evas_Object *rand_btn;
    } new_game;
    struct {
        Evas_Object *layout;
    } preferences;
    struct {
        Evas_Object *layout;
    } game;
    struct {
        Evas_Object *layout;
    } question;
    struct {
        Evas_Object *layout;
    } category;
    struct {
        Evas_Object *pref_btn;
        Evas_Object *reload_btn;
        Evas_Object *new_btn;
        Evas_Object *layout;
        Evas_Object *active_list;
        Evas_Object *inactive_list;
        Evas_Object *done_list;
    } games_list;
} Qd_UI;

Qd_UI view;

typedef struct
{
    char *opp_name;
    int round;



} Game_Data; 

int QD_EVENT_USER_NAME_CHANGED;

// private.h
int qd_view_main_win_add(void);
Evas_Object *qd_view_user_indicator_add(Evas_Object *parent);
Evas_Object *qd_view_category_page_add(Evas_Object *parent);
Evas_Object *qd_view_question_page_add(Evas_Object *parent);
//int qd_view_toolbar_add(void);
int qd_view_login_page_add(void);
int qd_view_games_list_page_add(void);
int qd_view_new_game_page_add(void);
int qd_view_preferences_page_add(void);

#endif