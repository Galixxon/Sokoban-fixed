#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#include "headers/logic.h"


//Gameplay functions

static movable* checkForBox(movable *boxes, int numOfBoxes, const int x, const int y)
{
    movable* potentialBox = NULL;
    for(int i = 0; i < numOfBoxes; i++)
    {
        if(boxes[i].x == x && boxes[i].y == y)
        {
            potentialBox = &(boxes[i]);
        }
    }
    return potentialBox;
}


static int checkWinCon(movable *boxes, int numOfboxes)
{
    int completed = 1;
    for(int i = 0; i < numOfboxes; i++)
    {
        if(boxes[i].digit != '.')
        {
            completed = 0;
            break;
        }
    }
    return completed;
}



int step(int move, char **map, movable *player, movable **boxes, int numOfBoxes, int height, int width)
{
    int state = 0;
    int x = player->x;
    int y = player->y;
    int new_x = x, new_y = y;
    int box_x, box_y;
    int ret = 0;
    switch (move)
    {
    case 1:
        new_y = y-1;
        box_y = new_y-1;
        if(new_y < 0) new_y = 0;
        break;
    
    case 2:
        new_y = y+1;
        box_y = new_y+1;
        if(new_y >= height) new_y = height-1;
        break;

    case 3:
        new_x = x-1;
        box_x = new_x-1;
        if(new_x < 0) new_x = 0;
        break;
    
    case 4:
        new_x = x+1;
        box_x = new_x+1;
        if(new_x >= width) new_x = width-1;
        break;

    default:
        break;
    }


    if(map[new_y][new_x] != '#')
    {
        movable* box = checkForBox(*boxes, numOfBoxes, new_x, new_y);
        if(box == NULL)
        {
            player->x = new_x;
            player->y = new_y;
        }
        else
        {
            movable* box2 =  checkForBox(*boxes, numOfBoxes, box_x, box_y);
            if(box == NULL)
            {
                box->x = box_x;
                box->y = box_y;
                if(map[box_y][box_x] == ".")
                {
                    state = (checkWinCon(*boxes,numOfBoxes));
                }
                player->x = new_x;
                player->y = new_y;
            }
        }
    }
    return state;
}

