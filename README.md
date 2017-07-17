# Pumice-Console-Menu

## What is it
Pumice-Console-Menu is a simple and barebone menu system to be used in UNIX terminals.
It is intended as a quick and dirty menu systems and is not geared for big or complex navigation tasks. 
It is targeted to be used for small debug programs or to edit simple settings.
The menu is navigable via single key presses and makes it easy to navigate between
different menu entries compared to typing around with commands or numbers as in normal terminal menus. 

## How to use it
The menu consists of two types of menus; simple menus (MENU) or actions menus (ACTION_MENU). 
A simple menu is used for navigation. A menu has up to 32 sub-menus which can be either simple 
menus or action menus. An action menu is the endpoint of a menu path. It consist of 
action items which the user can interact with. 
To sum it up; menus and submenus navigate to the action items the user interacts with.

## The menu tree
```
   Entry point
        |
        v
 +-------------+
 | Simpel menu |->-->-+
 +-------------+      |
                      |
                 +-------------+
                 | Action menu |---->----->----->------+
                 +-------------+                       |
                      |                              +-----------------+
                 +-------------+                     | Action/option 1 |
                 | Simple menu |->-+                 +-----------------+  
                 +-------------+   |                   |
                      |            |                 +-----------------+
                      |    +--------------------+    | Action/option 2 |
                      |    | More simple/action |    +-----------------+
                      |    | menu items etc...  |      |
                      |    +--------------------+    +-----------------+
                 +--------------------+              | etc...          |
                 | More simple/action |              +-----------------+
                 | menu items etc...  | 
                 +--------------------+ 
```
## Usage
The menus is generated by macros. What you get from the macro is
 * declaration of each menu item as menyItem_t type
```menuItem_t <menu name>Meny;```
 * an enum with a entry for each menu item 
```<menu name>MenuEnum <entry name>_e;```
 * a string array with entries for each menu item 
```<menu name>MenuStr[<entry name>_e] = “<entry name>”```
 * a count af all menu items
```<menu name>_COUNT```

To make a simple menu use the GENERATE_MENU macro with the following syntax
```c
#define <MENU NAME>(MENU_ITEM)\
  MENU_ITEM(<item name>)\
  MENU_ITEM(<item name>)\
  MENU_ITEM(<item name>)\
  etc...

GENERATE_MENU(<menu name>, <MENU NAME>);
```
To make a action menu use the ```GENERATE_ACTION_MENU``` macro with the following syntax
```c
#define <MENU NAME>(MENU_ITEM)\
  MENU_ITEM(<item name>)\
  MENU_ITEM(<item name>)\
  MENU_ITEM(<item name>)\
 	etc...

GENERATE_ACTION_MENU(<menu name>, <MENU NAME>, <action type>, <callback>);
```
Where the \<menu name\> will be the name of that menu and by extension the name of the declared
```menyItem_t``` variable and its string/enum companions. The \<MENU NAME\> is the exact name of the macro
you just defined. \<action type\> is a set of predefined action types such as
button, switch or number. \<callback\> is the the function that is going to be called when 
the user interact with any of the action items in that menu. It has the following prototype: 
```void <interaction funktion name>(uint8_t index, uint8_t action, int value);```
Where the index is the index of the item that called the callback function.
the action is the key press the user used to interact with the item and the value is the new/current value of that item. 

When all menus are generated use the following syntax to link them together: 
```c
#define LINK_MENU(PARENT_CHILD)\
  PARENT_CHILD(<parent menu name> , <child menu name>)\
  PARENT_CHILD(<parent menu name> , <child menu name>)\
  PARENT_CHILD(<parent menu name> , <child menu name>)\
  etc...   	
```
Where \<parent menu name\> and \<child menu name\> is the exact names given 
in the ```GENERATE_MENU / GENERATE_ACTION_MENU``` \<menu name\> parameter.

Now you can call the ```LINK_AND_RUN_MENU(<main menu/entry point>, <msg>)``` macro to run your menu. 

Alternatively you can link menus manually or under run time by accessing the parent/child fields in the menus you want to link.