#ifndef TERMINAL_H_
#define TERMINAL_H_

#include "structs.h"
#include<ncurses.h>

void gameLoop(int index);
void levelSelect();
int getPlayerInput();
void UpdateDisplay(WINDOW *win, char **arr, int w, int h, movable *pl, movable **boxes, int numofboxes);

#endif