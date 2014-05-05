#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Eina.h>
#include <Evas.h>
#include <Elementary.h>


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

static Qd_UI view;

static void qd_view_user_data_get(char **un, char **pw)
{
    const char *_un, *_pw;
    size_t len;

    _un = elm_entry_entry_get(view.login.en_user);
    len = strlen(_un);
    *un = malloc(len + 1);
    snprintf(*un, len + 1, "%s", _un); 

    _pw = elm_entry_entry_get(view.login.en_pw);
    len = strlen(_pw);
    *pw = malloc(len + 1);
    snprintf(*pw, len + 1, "%s", _pw); 
}

static void qd_view_games_list_page_show(void);

static void qd_view_login_page_clicked_ok_cb(void *data, Evas_Object *btn, void *event_info)
{
    char *un, *pw;
    qd_view_user_data_get(&un, &pw);
    printf("u: %s, pw: %s\n", un, pw);
    elm_object_part_text_set(view.user_ind.name, "default", un);
    qd_view_games_list_page_show();
}



int qd_view_login_page_add(void)
{
    Evas_Object *frame;
    // box as layout
    view.login.layout = elm_box_add(view.win);
    //elm_win_resize_object_add(view.win, view.login.layout);
    evas_object_size_hint_weight_set(view.login.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view.login.layout, EVAS_HINT_FILL, 0.0);

    // User name entry
    frame = elm_frame_add(view.login.layout);
    view.login.en_user = elm_entry_add(view.login.layout);
    elm_entry_single_line_set(view.login.en_user, EINA_TRUE);
    evas_object_size_hint_weight_set(frame, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(view.login.en_user);
    elm_object_part_content_set(frame, "default", view.login.en_user);
    elm_object_part_text_set(frame, "default", "User name");
    evas_object_show(frame);
    elm_box_pack_end(view.login.layout, frame);

    // pw entry
    frame = elm_frame_add(view.login.layout);
    view.login.en_pw = elm_entry_add(view.login.layout);
    elm_entry_single_line_set(view.login.en_pw, EINA_TRUE);
    elm_entry_password_set(view.login.en_pw, EINA_TRUE);
    evas_object_size_hint_weight_set(frame, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(frame, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(view.login.en_pw);
    elm_object_part_content_set(frame, "default", view.login.en_pw);
    elm_object_part_text_set(frame, "default", "Password");
    evas_object_show(frame);
    elm_box_pack_end(view.login.layout, frame);

    // OK button
    view.login.ok_btn = elm_button_add(view.login.layout);
    elm_object_part_text_set(view.login.ok_btn, "default", "login");
    evas_object_size_hint_weight_set(view.login.ok_btn, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view.login.ok_btn, EVAS_HINT_FILL, 0.0);
    elm_box_pack_end(view.login.layout, view.login.ok_btn);
    evas_object_show(view.login.ok_btn);
    evas_object_smart_callback_add(view.login.ok_btn, "clicked", qd_view_login_page_clicked_ok_cb, NULL);


    return 0;
}




int qd_view_user_indicator_add(void)
{
    Evas_Object *box;

    // use a fram as main layout obj
    view.user_ind.layout = elm_frame_add(view.win);
    elm_object_part_text_set(view.user_ind.layout, "default", "logged in as");

    // put box into frame
    box = elm_box_add(view.user_ind.layout);
    elm_box_horizontal_set(box, EINA_TRUE);
    evas_object_show(box);

    // Label for user name
    view.user_ind.name = elm_label_add(view.user_ind.layout);
    elm_object_part_text_set(view.user_ind.name, "default", "nobody");
    evas_object_size_hint_align_set(view.user_ind.name, 0.0, 0.0);
    evas_object_size_hint_weight_set(view.user_ind.name, EVAS_HINT_EXPAND, 0.0); 
    evas_object_show(view.user_ind.name);
    elm_box_pack_end(box, view.user_ind.name);

    elm_object_part_content_set(view.user_ind.layout, "default", box);
    evas_object_size_hint_align_set(view.user_ind.layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(view.user_ind.layout, EVAS_HINT_EXPAND, 0.0);

}


int qd_view_user_data_set(char *name, Evas_Object *icon)
{

    elm_object_part_text_set(view.user_ind.name, "default", name);
    //view.user_ic = icon;
}

static void _hide_cb(void *data, Evas *e, Evas_Object *obj, void *ev)
{
    evas_object_hide((Evas_Object *) data);
}

static void _show_cb(void *data, Evas *e, Evas_Object *obj, void *ev)
{
    evas_object_show((Evas_Object *) data);
}

static void qd_view_obj_show_hide_with(Evas_Object *parent, Evas_Object *child)
{
    evas_object_event_callback_add(parent, EVAS_CALLBACK_SHOW, _hide_cb, child);
    evas_object_event_callback_add(parent, EVAS_CALLBACK_HIDE, _show_cb, child);
}

void qd_view_preferences_page_show(void);
static void qd_view_games_list_pref_clicked_cb(void *data, Evas_Object *obj, void *ev)
{
    qd_view_preferences_page_show();
}

int qd_view_games_list_page_add(void)
{
    Evas_Object *frame, *scroller, *box;

    view.games_list.layout = elm_box_add(view.win);
    evas_object_size_hint_align_set(view.games_list.layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(view.games_list.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(view.games_list.layout);

    // 1.layout element: user indicator
    elm_box_pack_end(view.games_list.layout, view.user_ind.layout);
    qd_view_obj_show_hide_with(view.games_list.layout, view.user_ind.layout);
    //evas_object_show(view.user_ind.layout);

    // box inside scroller
    // box for different list
    box = elm_box_add(view.games_list.layout);
    evas_object_show(box);
    evas_object_size_hint_align_set(box, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0);

    // add start new game button
    view.games_list.new_btn = elm_button_add(view.games_list.layout);
    elm_object_part_text_set(view.games_list.new_btn, "default", "Start a new game");
    evas_object_size_hint_weight_set(view.games_list.new_btn, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.games_list.new_btn, EVAS_HINT_FILL, 0.0);
    elm_box_pack_end(box, view.games_list.new_btn);
    evas_object_show(view.games_list.new_btn);

    // add preferences button
    view.games_list.pref_btn = elm_button_add(view.games_list.layout);
    elm_object_part_text_set(view.games_list.pref_btn, "default", "Pref.");
    evas_object_size_hint_weight_set(view.games_list.pref_btn, EVAS_HINT_EXPAND, 0.0);
    evas_object_size_hint_align_set(view.games_list.pref_btn, EVAS_HINT_FILL, 0.0);
    evas_object_smart_callback_add(view.games_list.pref_btn, "clicked", qd_view_games_list_pref_clicked_cb, NULL);


    // add a scroller for all the things (2. layout element)
    scroller = elm_scroller_add(view.win);
    elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_ON);
    evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(scroller);
    elm_box_pack_end(view.games_list.layout, scroller);
    elm_object_part_content_set(scroller, "default", box);

    
#define GAMES_LIST_ADD(_list_var, _label_text) \
    frame = elm_frame_add(view.games_list.layout); \
    evas_object_size_hint_align_set(frame, EVAS_HINT_FILL, EVAS_HINT_FILL); \
    evas_object_size_hint_weight_set(frame, EVAS_HINT_EXPAND, 0.0); \
    elm_box_pack_end(box, frame); \
    evas_object_show(frame); \
    _list_var = elm_list_add(view.games_list.layout); \
    elm_list_mode_set(_list_var, ELM_LIST_EXPAND); \
    elm_object_part_text_set(frame, "default", _label_text); \
    elm_object_part_content_set(frame, "default", _list_var); \
    elm_list_go(_list_var)

    GAMES_LIST_ADD(view.games_list.active_list, "Your turn");
    GAMES_LIST_ADD(view.games_list.inactive_list, "Waiting for opponent");
    elm_list_select_mode_set(view.games_list.inactive_list, ELM_OBJECT_SELECT_MODE_NONE);
    GAMES_LIST_ADD(view.games_list.done_list, "Finished games");
    elm_list_select_mode_set(view.games_list.done_list, ELM_OBJECT_SELECT_MODE_NONE);

    //evas_object_show(view.toolbar.layout);
    evas_object_show(view.user_ind.layout);
    return 0;
}

void qd_view_games_list_active_item_add(char *user_name, void *data)
{
    elm_list_item_append(view.games_list.active_list, user_name, NULL, NULL, NULL, data);
}

void qd_view_games_list_inactive_item_add(char *user_name, void *data)
{
    elm_list_item_append(view.games_list.inactive_list, user_name, NULL, NULL, NULL, data);
}

void qd_view_games_list_done_item_add(char *user_name, void *data)
{
    elm_list_item_append(view.games_list.done_list, user_name, NULL, NULL, NULL, data);
}


void qd_view_new_page_show(Evas_Object *new_page_layout)
{
    Evas_Object *old_content;
    //old_content = elm_layout_content_unset(view.layout, "elm.swallow.content");
    //evas_object_hide(old_content);
    //elm_layout_content_set(view.layout, "elm.swallow.content", new_page_layout);
    //evas_object_show(new_page_layout);
    elm_naviframe_item_push(view.layout, "Quizduell", NULL, NULL, new_page_layout, NULL);
}

void qd_view_games_list_page_show(void)
{
    elm_naviframe_item_pop(view.layout);
}

void qd_view_preferences_page_show(void)
{
    elm_naviframe_item_push(view.layout, "Preferences", NULL, NULL, view.preferences.layout, NULL);
    //elm_layout_content_set(view.layout, "elm.swallow.content", view.preferences.layout);    
}

void qd_view_game_page_show(void)
{
    // should get opponent name
    elm_naviframe_item_push(view.layout, "game against", NULL, NULL, view.game.layout, NULL);
    //elm_layout_content_set(view.layout, "elm.swallow.content", view.game.layout);    
}

void qd_view_category_page_show(void)
{
    elm_naviframe_item_push(view.layout, "Select a category", NULL, NULL, view.category.layout, NULL);
    //elm_layout_content_set(view.layout, "elm.swallow.content", view.category.layout);    
}

void qd_view_question_page_show(void)
{
    elm_naviframe_item_push(view.layout, "", NULL, NULL, view.category.layout, NULL);
    //elm_layout_content_set(view.layout, "elm.swallow.content", view.question.layout);    
}

void qd_view_login_page_show(void)
{
    elm_naviframe_item_push(view.layout, "login", NULL, NULL, view.login.layout, NULL);
    //elm_layout_content_set(view.layout, "elm.swallow.content", view.login.layout);    
}

int qd_view_new_game_page_add(void)
{
    view.new_game.layout = elm_layout_add(view.win);
    return 0;
}
int qd_view_toolbar_add(void)
{
    Evas_Object *btn;
    view.toolbar.layout = elm_box_add(view.win);
    elm_box_horizontal_set(view.toolbar.layout, EINA_TRUE);
    evas_object_size_hint_align_set(view.toolbar.layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(view.toolbar.layout, EVAS_HINT_EXPAND, 0.0);

    btn = elm_button_add(view.toolbar.layout);
    elm_object_part_text_set(btn, "default", "pref");
    elm_box_pack_end(view.toolbar.layout, btn);
    btn = elm_button_add(view.toolbar.layout);
    elm_object_part_text_set(btn, "default", "reload");
    elm_box_pack_end(view.toolbar.layout, btn);
    elm_object_part_content_set(view.layout, "elm.swallow.title", view.toolbar.layout);
    evas_object_show(view.toolbar.layout);

}

void _on_exit_cb(void *data, Evas_Object *obj, void *event_info)
{
    elm_exit();
}

int qd_view_main_win_add()
{
    view.win = elm_win_util_standard_add("Quizduell Window", "Quizduell");
    elm_win_focus_highlight_enabled_set(view.win, EINA_TRUE);
    evas_object_smart_callback_add(view.win, "delete,request", _on_exit_cb, NULL);
    
    evas_object_size_hint_weight_set(view.win, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view.win, EVAS_HINT_FILL, EVAS_HINT_FILL);

    view.layout = elm_naviframe_add(view.win);
    elm_naviframe_content_preserve_on_pop_set(view.layout, EINA_TRUE);
    evas_object_show(view.layout);

    evas_object_size_hint_weight_set(view.layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(view.layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_win_resize_object_add(view.win, view.layout);

    evas_object_resize(view.win, 480, 640);
    evas_object_show(view.win);

    return 0;
}

int qd_view_init(int argc, char **argv)
{
    elm_init(argc, argv);
    qd_view_main_win_add();
    qd_view_user_indicator_add();
    qd_view_toolbar_add();
    qd_view_login_page_add();
    qd_view_games_list_page_add();
    qd_view_new_game_page_add();
    return 1;
}

qd_UI_shutdown(void)
{
    elm_shutdown();
}

int main(int argc, char **argv)
{

    if (!qd_view_init(argc, argv))
    {
        return 1;
    }

    elm_naviframe_item_push(view.layout, "Quizduell", view.games_list.reload_btn,
                            view.games_list.pref_btn, view.games_list.layout, NULL);

    elm_naviframe_item_push(view.layout, "Login", NULL, NULL, view.login.layout, NULL);
    //elm_object_part_content_set(view.layout, "elm.swallow.content", view.login.layout);

    elm_run();

    qd_UI_shutdown();



}
