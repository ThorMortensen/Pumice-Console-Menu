/* 
 * Copyright (c) 2017 Thor Mortensen
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* File:   pumiceConsoleMenu.h
 * Author: Thor Mortensen 
 *
 * Created on July 16, 2017, 5:01 PM
 */

/**
 * Thise files makes it esy to generate a guick and durty terminal menu system.
 * The menu us navigatable with the single key presses.
 * The menu consists of two types of menus: simple menus (MENU) or actions menus
 * (ACTION_MENU).
 * A simple menu is used for navigation. A menu has up to 32 child menus 
 * whitch can be eiather simple menus or arction menus.
 * An action menu is the endpoint of a menu path. It consist of action items
 * which are interactioneble items. 
 * 
 * Usage:
 * 
 * Use this syntax to make a simple menu:
 * 
  #define <MENU_NAME>(MENU_ITEM)\
    MENU_ITEM(<menu item>)\
    MENU_ITEM(APS_VPC)\
    MENU_ITEM(BP_Heater)\
    MENU_ITEM(CMM_Heater)\
    MENU_ITEM(MAN_Mode)\
    MENU_ITEM(Control)\

    GENERATE_MENU(main, MAIN_MENU);
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

