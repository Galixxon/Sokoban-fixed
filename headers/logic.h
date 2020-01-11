#include "structs.h"


//void        mainLoop(const int level_index);
field*      canMove(const move_t dir, movable_t *object, const int max_x, const int max_y, field **map); //+
field_t     checkFieldType(const field *f); //+
movable_t*  checkForBox(movable_t boxes[], field *field, int numOfBoxes);
void        moveBox(field_t *newField, movable_t *box, movable_t boxes[]);
void        gameReset(movable_t *player, field *playerPos, movable_t boxes[], field *boxesPos[], int numOfBoxes);
void        movePlayer(move_t lastdir, field **map, int max_x, int max_y, field_t *newField, movable_t *pl, movable_t boxes[]);
bool        checkWinCon(movable_t boxes[], int numOfboxes);
level*      getLevelInfo();