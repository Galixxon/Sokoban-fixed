#include "levelmanager.h"
int step(int move, char **map, movable *player, movable **boxes, int numOfBoxes, int height, int width);
int checkWinCon(char **map, movable **boxes, int numOfboxes);
movable* checkForBox(movable **boxes, int numOfBoxes, const int x, const int y);
void reset(movable *player, movable *startingPlayer, movable **boxes, movable *startingboxes[], int numOfboxes);
