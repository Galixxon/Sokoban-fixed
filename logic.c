#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include "headers/logic.h"

#define PATH "assets/maps.txt"

static field* canMove(const move_t dir, movable_t *object, const int max_x, const int max_y, field **map)
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
        
    switch (temp->digit)
    {
    case '#':
        moveField = NULL;
        break;
    case '.':
        moveField = temp;
        break;
    case 'x':
        moveField = temp;
        break;
    default:
        moveField = NULL;
        break;
    }

    return moveField;
}
static movable_t* checkForBox(movable_t boxes[], field *f, int numOfBoxes)
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


static bool checkWinCon(movable_t boxes[], int numOfboxes)
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

int getLevemAmount()
{
    int counter= 0;
    FILE *fp;
    fp = fopen(PATH,"r");
    char c;
    
    fclose(fp);
}


level *getLevelInfo(const int index)
{
    level *l = malloc(sizeof(level));
    char ch = 0;
    FILE *fp;
    fp = fopen(PATH,"r");

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
                    case 'x':
                        l->map[i][j].fieldType = HOLE;
                        break;
                    case 'P':
                        l->playerinfo->pos = &(l->map[i][j]);
                        l->map[i][j].fieldType = FLOOR;
                        l->map[i][j].digit = '.';
                        break;
                    case 'o':
                        l->boxes[temp].pos = &(l->map[i][j]);
                        l->map[i][j].fieldType = FLOOR;
                        l->map[i][j].digit = '.';
                        temp++;
                        break;
                    case 'I':
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

void mainLoop(level *actualLevel)
{
    int *height = actualLevel->height;
    int *width = actualLevel->width;
    field **map = actualLevel->map;
    int *numOfBoxes = actualLevel->numOfBoxes;
    movable_t *player = actualLevel->playerinfo;
    movable_t *boxes = actualLevel->boxes;
    move_t move;
    
    printMap(map, width,height,player,boxes,numOfBoxes);
    
    while(actualLevel->completed==false)
    {
        
        move = getPlayerInput();
        system("clear");            
        if(move == QUIT){ break;}
        else
        {
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
            }
        }
        printMap(map, width,height,player,boxes,numOfBoxes);
    }
    system("clear");
    //printf("Game finished.\n");

    free(player);
    for(int i = 0; i < height; i++)
    {
        free(map[i]);
    }
    free(map);
    free(boxes);
    free(actualLevel);


    //system("clear");
    //printMap(map, width,height,player,boxes,numOfBoxes);

    
}