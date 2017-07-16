/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>


#include "pumiceConsoleMenu.h"

bool run = true;

enum evalInputModes {
   NAV,
   GET_ENTER,
   INPUT_MODE_COUNT
};

menuItem_t* currentMenu;
menuItem_t* backStack;

const char* switchInteraction[] = {
   "|U |N*|D |",
   "|U*|N |D |",
   "|U |N |D*|"
};

const char* buttonInteraction[] = {
   "|R*|A |",
   "|R |A*|",
};

void printUsage();

void clearScreen() {
   printf("\033[2J\r");
   printf("\033[0;0H");
}

void exitPoint(int signo) {
   system("/bin/stty cooked");
   printf("uit\n");
   run = false;
}

void printHelp() {
   clearScreen();
   printUsage();
}

bool navInput(int input, menuItem_t** menu) {
   switch (input) {
      case KEY_UP:
         (*(menu))->i--;
         if ((*(menu))->i < 0) {
            (*(menu))->i = 0;
         }
         return true;
      case KEY_DOWN:
         (*(menu))->i++;
         if ((*(menu))->i > (*(menu))->itemCount - 1) {
            (*(menu))->i = (*(menu))->itemCount - 1;
         }
         return true;
      case KEY_SELECT:
         if ((*(menu))->child[(*(menu))->i] != NULL) {
            *menu = (*(menu))->child[(*(menu))->i];
         }
         return true;
      case KEY_BACK:
         if ((*(menu))->parent != NULL) {
            *menu = (*(menu))->parent;
         }
         return true;
      case KEY_HELP:
         printHelp();
         return true;
      case KEY_QUIT:
         exitPoint(0);
         return true;

   }
   return false;
}

void editInput(int input, menuItem_t** menu) {
   int inputnum = 0;
   int inCount = 0;

   switch ((*(menu))->interactionType) {
      case SWITCH:
         if (input == KEY_EDIT_DOWN) {
            (*(menu))->itemVal[(*(menu))->i] = POS_DPOWN;
         } else if (input == KEY_EDIT_UP) {
            (*(menu))->itemVal[(*(menu))->i] = POS_UP;
         } else if (input == KEY_EDIT_NEU) {
            (*(menu))->itemVal[(*(menu))->i] = NEUTRAL;
         }
         break;
      case BUTTON:
         if (input == KEY_EDDIT) {
            (*(menu))->itemVal[(*(menu))->i] ^= (1 << 0);
         } else if (input == ' ') {
            (*(menu))->itemVal[(*(menu))->i] ^= (1 << 0);
         }
         break;
      case NUM:
         if (input == KEY_EDDIT) {
            system("/bin/stty cooked");
            printf("nter number: ");
            inCount = scanf("%d", &inputnum);
            if (inCount >= 0) {
               (*(menu))->itemVal[(*(menu))->i] = inputnum;
            }
            system("/bin/stty raw");
         }
         break;
   }
   if ((*(menu))->interAction != NULL) {
      (*(menu))->interAction((*(menu))->i, input, (*(menu))->itemVal[(*(menu))->i]);
   }

}

void evalInput(uint8_t mode, menuItem_t** menu) {
   int input = 0;
   int inCount = 0;
   bool validIn = false;


   switch (mode) {
      case NAV:
         inCount = scanf("%c", (char*) &input);
         if (inCount > 0) {
            if (!navInput(input, menu)) {
               editInput(input, menu);
            }
         }
         break;

      case GET_ENTER:
         inCount = scanf("%c", (char*) &input);
         if (inCount > 0) {
            if (input == KEY_QUIT) {
               exitPoint(0);
            } else {
               validIn = true;
            }
         }
         break;
   }
}

void pauseMenuScreen() {
   system("/bin/stty cooked");
   clearScreen();
}

void resumeMenuScreen() {
   system("/bin/stty raw");
}

void exitMenu() {
   clearScreen();
   exitPoint(0);
}

void printUsage() {
   system("/bin/stty cooked");
   printf("\n"
           "                Up                      \n"
           "                                        \n"
           "                '%c'                     \n"
           "                 ^                      \n"
           "                 |                      \n"
           "            +----------+                \n"
           "Back '%c'<-- |NAVIGATION| -->'%c'  Select \n"
           "            +----------+                \n"
           "                 |                      \n"
           "                 v                      \n"
           "                '%c'                     \n"
           "                                        \n"
           "                Down                    \n"
           "\n"
           "    +--------------------------+ \n"
           "    |        INTERACTION       |         \n"
           "    +--------------------------+         \n"
           "    |  This Overview  ~> '%c'   |         \n"
           "    |  Eddit          ~> '%c'   |         \n"
           "    |  Switch Up      ~> '%c'   |         \n"
           "    |  Switch Neutral ~> '%c'   |         \n"
           "    |  Switch Down    ~> '%c'   |         \n"
           "    |  Do Stuff       ~> ENTER |         \n"
           "    |  Quit           ~> '%c'   |         \n"
           "    +--------------------------+         \n"
           "\n    Press any key to continue...  \n"
           , KEY_UP, KEY_BACK, KEY_SELECT, KEY_DOWN,
           KEY_HELP, KEY_EDDIT, KEY_EDIT_UP, KEY_EDIT_NEU, KEY_EDIT_DOWN, KEY_QUIT);
   system("/bin/stty raw");
   evalInput(GET_ENTER, NULL);
}

void updateMenuScreen() {
   int i;
   int noIn = 0;
   clearScreen();
   printf("You are here: Main Menu");
   backStack = currentMenu->parent;
   if (backStack != NULL) {
      while (backStack->parent != NULL) {
         backStack = backStack->parent;
      }
      while (backStack != currentMenu) {
         printf(" / %s", backStack->menuStr[backStack->i]);
         backStack = backStack->child[backStack->i];
      }
   }
   printf("*");
   printf("\n\n\r");
   switch (currentMenu->interactionType) {
      case SWITCH:
         printf("   Use Switch keys [%c,%c,%c] to change { Up | Neutral | Down }. Press Enter to set\n\n\r", KEY_EDIT_UP, KEY_EDIT_NEU, KEY_EDIT_DOWN);
         break;
      case BUTTON:
         printf("   Press SPACE to activate button or \n\r");
         printf("   Edit [%c] to toggle { Release | Active }.  Press Enter to set\n\n\r", KEY_EDDIT);
         break;
      case NUM:
         printf("   Edit [%c] to change { value }. Press Enter to set\n\n\r", KEY_EDDIT);
         break;
      default:
         printf("\n\n\r");
         break;

   }
   for (i = 0; i < currentMenu->itemCount; i++) {
      if (currentMenu->i == i) {
         printf("-->[%s]  ", currentMenu->menuStr[i]);
      } else {
         printf("   [%s]  ", currentMenu->menuStr[i]);
      }
      switch (currentMenu->interactionType) {
         case SWITCH:
            printf("\t%s\n\r", switchInteraction[currentMenu->itemVal[i]]);
            break;
         case NUM:
            printf("\t%d\n\r", currentMenu->itemVal[i]);
            break;
         case BUTTON:
            printf("\t%s\n\r", buttonInteraction[currentMenu->itemVal[i]]);
            break;
         default:
            printf("\n\r");
            break;

      }
   }
   printf("\n\r");
   fflush(stdout);
}

void runMenu(menuItem_t* mainMenu, char* wellcomeMsg) {
   if (wellcomeMsg != NULL) {
      printf("%s\n", wellcomeMsg);
   }

   if (signal(SIGINT, exitPoint) == SIG_ERR) {
      printf("\ncan't catch SIGINT\n");
   }

   currentMenu = mainMenu;
   backStack = NULL;

   printUsage();

   while (run) {
      updateMenuScreen();
      evalInput(NAV, &currentMenu);
      printf("\n\r");

   }
   run = true;
   system("/bin/stty cooked");
   printf("\n");
}




