#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef enum fieldType {EMPTY, FLOOR, WALL, HOLE} field_t;
typedef enum movemevt {NONE, UP, DOWN, LEFT,RIGHT} move_t;

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

move_t      getPlayerInput(); //+
field*      canMove(const move_t dir, movable_t *object, const int max_x, const int max_y, field **map); //+
field_t     checkFieldType(const field *f); //+
movable_t*  checkForBox(movable_t boxes[], field *field, int numOfBoxes);
void        moveBox(field_t *newField, movable_t *box, movable_t boxes[]);
void        movePlayer(move_t lastdir, field **map, int max_x, int max_y, field_t *newField, movable_t *pl, movable_t boxes[]);
bool        checkWinCon(movable_t boxes[], int numOfboxes);
level*      getLevelInfo();

move_t getPlayerInput()
{
    char c;
    move_t m;
    scanf("%c", &c);
    switch (c)
    {
    case 'U':
        m = UP;
        break;
    
    case 'D':
        m = DOWN;
        break;

    case 'L':
        m = LEFT;
        break;

    case 'R':
        m = RIGHT;
        break;
    }

    return m;
}

field* canMove(const move_t dir, movable_t *object, const int max_x, const int max_y, field **map)
{
    int x = object->pos->x;
    int y = object->pos->y;
    int new_x = x, new_y = y;
    field* moveField = NULL;
    switch (dir)
    {
    case UP:
        new_y = y-1;
        if(new_y < 0) new_y = 0;
        break;
    
    case DOWN:
        new_y = y+1;
        if(new_y >= max_y) new_y = max_y-1;
        break;

    case LEFT:
        new_x = x-1;
        if(new_x < 0) new_x = 0;
        break;
    
    case RIGHT:
        new_x = x+1;
        if(new_x >= max_x) new_x = max_x;
        break;

    default:
        break;
    }
    field *temp = &(map[new_y][new_x]);
    field_t newFieldType = checkFieldType(temp);
    
    switch (newFieldType)
    {
    case WALL:
        moveField = NULL;
        break;
    case FLOOR:
        moveField = temp;
        break;
    case HOLE:
        moveField = temp;
        break;
    default:
        moveField = NULL;
        break;
    }

    return moveField;
}
movable_t* checkForBox(movable_t boxes[], field *f, int numOfBoxes)
{
    movable_t *box = NULL;
    for(int i = 0; i< numOfBoxes; i++)
    {
        if(boxes[i].pos == f)
        {
            box = &(boxes[i]);
        }
    }
    return box;
}

field_t checkFieldType(const field *f)
{
    return f->fieldType;
}

bool checkWinCon(movable_t boxes[], int numOfboxes)
{
    bool completed = true;
    for(int i = 0; i < numOfboxes; i++)
    {
        if(boxes[i].pos->fieldType != HOLE)
        {
            completed = false;
            break;
        }
    }
    return completed;
}

level *getLevelInfo(const int index)
{
    level *l = malloc(sizeof(level));
    char ch = 0;
    FILE *fp;
    fp = fopen("maps.txt","r");

    int level;
    int height,width,boxes;
    while(ch != EOF)
    {
        ch = fgetc(fp);
        while(ch != '?')
        {
            ch = fgetc(fp);
        }
        fscanf(fp,"%d",&level);
        if(level == index)
        {
            break;
        }
    }
    if(level != index) return NULL;
    
    fscanf(fp,"%d%d%d", &height, &width, &boxes);
    ch = fgetc(fp);

    l->completed = false;
    l->height = height;
    l->width = width;
    l->numOfBoxes = boxes;
    l->playerinfo = malloc(sizeof(movable_t));
    l->map = malloc(sizeof(field*) * height);

    for(int i = 0; i < height; i++)
    {
        l->map[i] = malloc(sizeof(field) * width);
    }
    l->boxes = malloc(sizeof(movable_t) * boxes);
    int temp = 0;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j <= width; j++)
        {
            ch=fgetc(fp);
            if(ch != '\n')
            {
                l->map[i][j].digit = ch;
                l->map[i][j].y = i;
                l->map[i][j].x = j;
                switch (ch)
                {
                    case ' ':
                        l->map[i][j].fieldType = EMPTY;
                        break;
                    case '.':
                        l->map[i][j].fieldType = FLOOR;
                        break;
                    case '#':
                        l->map[i][j].fieldType = WALL;
                        break;
                    case 'o':
                        l->map[i][j].fieldType = HOLE;
                        break;
                    case 'P':
                        l->playerinfo->pos = &(l->map[i][j]);
                        l->map[i][j].fieldType = FLOOR;
                        l->map[i][j].digit = '.';
                        break;
                    case 'O':
                        l->boxes[temp].pos = &(l->map[i][j]);
                        l->map[i][j].fieldType = FLOOR;
                        l->map[i][j].digit = '.';
                        temp++;
                        break;
                    case '0':
                        l->boxes[temp].pos = &(l->map[i][j]);
                        l->map[i][j].fieldType = HOLE;
                        l->map[i][j].digit = 'o';
                        temp++;
                        break;
                    default:
                        break;
                }
            }   
        }
    }
    fclose(fp);
    return l;
}

void printMap(field **arr, int w, int h, movable_t *pl, movable_t *boxes, int numofboxes)
{
    char temp = pl->pos->digit;
    pl->pos->digit = 'P';

    for(int i = 0; i < numofboxes; i++)
    {
        boxes[i].pos->digit = 'O';
    }
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            putchar(arr[i][j].digit);
        }
        putchar('\n');
    }
    pl->pos->digit = temp;
}




int main()
{
    int a;
    printf("Level: ");
    scanf("%d", &a);
    level *actualLevel = getLevelInfo(0);
    if(actualLevel == NULL)
    {
        printf("Error, could not read a level");
        return 0;
    }
    int height = actualLevel->height;
    int width = actualLevel->width;
    field **map = actualLevel->map;
    int numOfBoxes = actualLevel->numOfBoxes;
    movable_t *player = actualLevel->playerinfo;
    movable_t *boxes = actualLevel->boxes;


    move_t move;
    printMap(map, width,height,player,boxes,numOfBoxes);
    while(actualLevel->completed==false)
    {
        move = getPlayerInput();
        field *playerField = canMove(move,player,width,height,map);
        if(playerField != NULL)
        {
            movable_t *box = checkForBox(boxes,playerField,numOfBoxes);
            if(box != NULL)
            {
                field *boxField = canMove(move,box,width,height,map);
                if(boxField != NULL)
                {
                    movable_t *otherbox = checkForBox(boxes,boxField,numOfBoxes);
                    if(otherbox == NULL)
                    {
                        box->pos = boxField;
                        player->pos = playerField;
                        actualLevel->completed = checkWinCon(boxes,numOfBoxes);
                    }
                }
            }
            else
            {
                player->pos = playerField;
            }
            system("clear");
            printMap(map, width,height,player,boxes,numOfBoxes);
        }
    }
    system("clear");
    printMap(map, width,height,player,boxes,numOfBoxes);

    return 0;
}