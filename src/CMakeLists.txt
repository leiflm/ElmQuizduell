configure_file("${PROJECT_SOURCE_DIR}/src/include/Quizduell_Config.h.in"
	       "${PROJECT_SOURCE_DIR}/src/include/Quizduell_Config.h"
	       @ONLY)

include_directories(
    "${CMAKE_SOURCE_DIR}/src/include"
    ${GETTEXT_INCLUDE_DIRS}
    ${EINA_INCLUDE_DIRS}
    ${EVAS_INCLUDE_DIRS}
    ${ECORE_INCLUDE_DIRS}
    ${EET_INCLUDE_DIRS}
    ${EO_INCLUDE_DIRS}
    ${EDJE_INCLUDE_DIRS}
    ${ECORE_EVAS_INCLUDE_DIRS}
    ${ECORE_FILE_INCLUDE_DIRS}
    ${ECORE_X_INCLUDE_DIRS}
    ${ECORE_IPC_INCLUDE_DIRS}
    ${ECORE_INPUT_INCLUDE_DIRS}
    ${ECORE_IMF_INCLUDE_DIRS}
    ${ECORE_CON_INCLUDE_DIRS}
    ${ELDBUS_INCLUDE_DIRS}
    ${EFREET_INCLUDE_DIRS}
    ${ETHUMB_INCLUDE_DIRS}
    ${ETHUMB_CLIENT_INCLUDE_DIRS}
    ${ELEMENTARY_INCLUDE_DIRS}
    ${EFL_INCLUDE_DIRS}
    ${LIBBSD_INCLUDE_DIRS}
    ${JSON-C_INCLUDE_DIRS}
    ${GCRYPT_INCLUDE_DIRS}
    )


LIST(APPEND ELM_QUIZDUELL_CC_SOURCES
    quizduell.c
    quizduell_config.c
    quizduell_connection.c
    quizduell_controller.c
    quizduell_crypto.c
    quizduell_game_json.c
    quizduell_structures.c
    quizduell_view.c
    base64.c
    view/quizduell_category.c
    view/quizduell_game_stat.c
    view/quizduell_games_list.c
    view/quizduell_login.c
    view/quizduell_new_game.c
    view/quizduell_preferences.c
    view/quizduell_question.c
    view/quizduell_search_player.c
    )

add_executable(elm_quizduell ${ELM_QUIZDUELL_CC_SOURCES})

target_link_libraries(elm_quizduell 
    ${GETTEXT_LIBRARIES}
    ${EINA_LIBRARIES}
    ${EVAS_LIBRARIES}
    ${ECORE_LIBRARIES}
    ${EET_LIBRARIES}
    ${EO_LIBRARIES}
    ${EDJE_LIBRARIES}
    ${ECORE_EVAS_LIBRARIES}
    ${ECORE_FILE_LIBRARIES}
    ${ECORE_X_LIBRARIES}
    ${ECORE_INPUT_LIBRARIES}
    ${ECORE_IMF_LIBRARIES}
    ${ECORE_IPC_LIBRARIES}
    ${ECORE_CON_LIBRARIES}
    ${ELDBUS_LIBRARIES}
    ${EFREET_LIBRARIES}
    ${ETHUMB_LIBRARIES}
    ${ETHUMB_CLIENT_LIBRARIES}
    ${ELEMENTARY_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${LIBBSD_LIBRARIES}
    ${GCRYPT_LIBRARIES}
    ${JSON-C_LIBRARIES}
    )

install(TARGETS elm_quizduell DESTINATION bin)
