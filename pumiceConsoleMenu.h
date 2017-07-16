/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   pumiceConsoleMenu.h
 * Author: Thor Mortensen 
 *
 * Created on July 16, 2017, 5:01 PM
 */

/**
 * Usage:
 * Generate a menu with the 
 * 
 * 
 * you must use the LINK_MENU(SET_LINK) macro to to link the menu together 
 */


#ifndef PUMICECONSOLEMENU_H
#define PUMICECONSOLEMENU_H

#ifdef __cplusplus
extern "C" {
#endif

    
#include <stdint.h>


#define KEY_UP      'w'
#define KEY_DOWN    's'
#define KEY_BACK    'a'
#define KEY_SELECT  'd'
#define KEY_QUIT    'q'
#define KEY_HELP    'h'

#define KEY_EDDIT   'e'

#define KEY_EDIT_UP 'i'
#define KEY_EDIT_NEU 'o'
#define KEY_EDIT_DOWN 'p'

#define KEY_ENTER 13

enum interactionPos_e {
    NEUTRAL,
    POS_UP,
    POS_DPOWN,
    ENTER
};

typedef enum interaction_e {
    NOP,
    SWITCH,
    BUTTON,
    NUM
} interaction_t;

typedef struct menuItem_s {
    struct menuItem_s* parent;
    uint8_t itemCount;
    struct menuItem_s* child[32];
    int8_t i;
    interaction_t interactionType;
    int* itemVal;
    void (*interAction)(uint8_t index, uint8_t action, int value);
    const char** menuStr;
} menuItem_t;

#define GENERATE_ENUM(ENUM) ENUM##_e,
#define GENERATE_STRING(STRING) #STRING,

#define SET_LINK(parentName,childName)\
    parentName##Menu.child[childName##_e] = &childName##Menu;\
    childName##Menu.parent = &parentName##Menu;

#define GENERATE_ACTION_MENU(name, MENU_ITEMS, type, actionFunc) \
    enum name##MenuEnum{        \
    MENU_ITEMS(GENERATE_ENUM)   \
            name##_COUNT        \
};                              \
const char* name##MenuStr[] = {MENU_ITEMS(GENERATE_STRING)};\
int name##Actions[name##_COUNT] = {0};          \
menuItem_t name##Menu = {                       \
        .parent = NULL,                    \
        .child = {NULL},                         \
        .menuStr = name##MenuStr,               \
        .itemCount = name##_COUNT,              \
        .interactionType = type,                \
        .itemVal = name##Actions,                \
        .interAction = actionFunc,              \
        .i = 0                                  \
};  \

#define GENERATE_MENU(name, MENU_ITEMS) \
    enum name##MenuEnum{        \
    MENU_ITEMS(GENERATE_ENUM)   \
            name##_COUNT        \
};                              \
const char* name##MenuStr[] = {MENU_ITEMS(GENERATE_STRING)};\
menuItem_t name##Menu = {                       \
        .parent = NULL,                         \
        .child = {(NULL)},                         \
        .menuStr = name##MenuStr,               \
        .itemCount = name##_COUNT,              \
        .interactionType = NOP,                \
        .itemVal = NULL,                \
        .interAction = NULL,              \
        .i = 0                                  \
}    

#define LINK_AND_RUN_MENU(mainMenuPtr, welcomeMsg)\
   LINK_MENU(SET_LINK);             \
   runMenu(&mainMenuPtr, welcomeMsg)\

void resumeMenuScreen();
void pauseMenuScreen();
void updateMenuScreen();
void runMenu(menuItem_t* mainMenu, char* wellcomeMsg);
void exitMenu();



#ifdef __cplusplus
}
#endif

#endif /* PUMICECONSOLEMENU_H */

