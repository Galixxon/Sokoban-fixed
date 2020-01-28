#include "logic.h"
#include<ncurses.h>

void gameLoop(int index);
void levelSelect();
int getPlayerInput();
void UpdateDisplay(WINDOW *win, char **arr, int w, int h, movable *pl, movable **boxes, int numofboxes);
