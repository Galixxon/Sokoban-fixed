#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "levelmanager.h"
#define MAPS_PATH "assets/maps.txt"
#define MAX_SIZE 32
#define MAX_BOXES_AMOUNT 16

void freeLevel(level *level)
{
    free(level->player);
    for(int i = 0; i < level->num_of_boxes; i++)
    {
        free(level->boxes[i]);
    }
    free(level->boxes);
    for(int i = 0; i < level->height; i++)
    {
        free(level->map[i]);
    }
    free(level->map);
    free(level);
}


int getLevelAmount()
{
    int counter = 0;
    FILE* fp = fopen(MAPS_PATH, "r");
    char c = fgetc(fp);
    while(c != EOF)
    {
        c = fgetc(fp);
        if(c==';')
        {
            counter++;
        }
    }
    fclose(fp);
    return counter;
}

level* getLevelInfo(const int index) //Indexowanie od 1
{
    if(index < 1) return NULL;
    FILE *fp = fopen(MAPS_PATH, "r");
    if(fp == NULL) return NULL;
    char* line = malloc(sizeof(char)* MAX_SIZE);

    int mapPoint = index-1;
    int counter = 0;
    while(counter < mapPoint)
    {
        fgets(line,MAX_SIZE,fp);
        if(line[0] == ';')counter++; 
    }
    if(index > 1) fgets(line,MAX_SIZE,fp); //pozbywam się linii pustej (wszędze poza 0 poziomem)
    
    free(line);

    level *l;
    l = malloc(sizeof(level));
    
    l->player = malloc(sizeof(movable));
    l->player->digit = '@';

    l->num_of_boxes = 0;
    int tempwidth = 0;
    int tempheight = 0;
    
    char temparr[MAX_SIZE][MAX_SIZE];
    movable tempBoxes[MAX_BOXES_AMOUNT];
    char c = fgetc(fp);
    int levelIndex;
    while(c!=';')
    {
        tempwidth = 0;
        for(int i = 0; i < MAX_SIZE && c != '\n'; i++)
        {
            
            switch(c)
            {
                case '@':
                l->player->x = tempwidth;
                l->player->y = tempheight;
                c = ' ';
                break;

                case '$':
                levelIndex = l->num_of_boxes;
                (l->num_of_boxes) += 1;
                tempBoxes[levelIndex].x = tempwidth;
                tempBoxes[levelIndex].y = tempheight;
                c = ' ';
                break;

                case '+':
                l->player->x = tempwidth;
                l->player->y = tempheight;
                c = '.';
                break;

                case '*':
                levelIndex = l->num_of_boxes;
                (l->num_of_boxes) += 1;
                tempBoxes[levelIndex].x = tempwidth;
                tempBoxes[levelIndex].y = tempheight;
                c = '.';
                break;
                
                default:
                break;
            }
            temparr[tempheight][tempwidth] = c;
            tempwidth++;
            c = fgetc(fp);
        }
        tempheight++;
        c=fgetc(fp);
    }
    if(tempheight > MAX_SIZE || tempwidth > MAX_SIZE || l->num_of_boxes > MAX_BOXES_AMOUNT)
    {
        printf("Bad level format");
        return NULL;
    }

    l->height = tempheight;
    l->width = tempwidth;


    int a = l->num_of_boxes;
    l->boxes = (movable**)malloc(sizeof(movable*) * a);
    for(int i = 0; i < a;i++)
    {
        (l->boxes)[i] = malloc(sizeof(movable));
        l->boxes[i]->digit = '$';
        l->boxes[i]->x = tempBoxes[i].x;
        l->boxes[i]->y = tempBoxes[i].y;
    }

    l->map = malloc(sizeof(char*) * tempheight);
    for(int i = 0; i < tempheight; i++)
    {
        l->map[i] = malloc(sizeof(char) * tempwidth);
        for(int j = 0; j < tempwidth; j++)
        {
            l->map[i][j] = temparr[i][j];
        }
    }

    char* s = l->map[4];
    fclose(fp);
    return l;
}