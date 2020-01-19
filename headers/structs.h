typedef struct movable
{
    char digit;
    int x;
    int y;
}movable;

typedef struct levelOverview
{
    char *name;
    char **map;
    int index;
}lOverview;

typedef struct level
{
    int index;
    char* name;
    
    int height;
    int width;
    int num_of_boxes;
    movable *player;
    movable *s_player;
    movable *boxes;
    movable *s_boxes;
    char **map;
}level;