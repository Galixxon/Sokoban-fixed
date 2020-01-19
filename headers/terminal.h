#include "logic.h"
#include<ncurses.h>

void UpdateDisplay(WINDOW *win, char **arr, int w, int h, movable *pl, movable *boxes, int numofboxes);
void print_menu(WINDOW *menu_win, int highlight, int n_choices, char** choices);
int getPlayerInput();
char** setMenuItems(int n_choices);
void levelSelect();
void gameLoop(int index);
