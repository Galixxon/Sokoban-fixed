#include "structs.h"
int step(int move, char **map, movable *player, movable **boxes, int numOfBoxes, int height, int width);
static int checkWinCon(movable *boxes, int numOfboxes);
static movable* checkForBox(movable *boxes, int numOfBoxes, const int x, const int y);
