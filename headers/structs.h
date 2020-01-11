typedef enum fieldType {EMPTY, FLOOR, WALL, HOLE} field_t;
typedef enum movemevt {NONE, UP, DOWN, LEFT,RIGHT, RESET, QUIT} move_t;

typedef struct field
{
    int x,y;
    char digit;
    field_t fieldType;
}field;

typedef struct movable
{
    field *pos;
}movable_t;


typedef struct levelinfo
{
    int height;
    int width;
    int numOfBoxes;
    movable_t *playerinfo;
    movable_t *boxes;
    field **map;
    bool completed;
}level;
