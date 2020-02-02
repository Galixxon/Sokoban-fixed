#ifndef LEVELMANAGER_H_
#define LEVELMANAGER_H_

#include "structs.h"

int getLevelAmount();
level* getLevelInfo(const int index);
void freeLevel(level *level);

#endif