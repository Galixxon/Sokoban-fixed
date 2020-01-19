#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "levelmanager.h"
#define MAPS_PATH "assets/maps.txt"
#define MAX_SIZE 32

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
    
    level *l;
    l = malloc(sizeof(level));
    
    l->boxes = NULL;
    l->player = malloc(sizeof(movable));
    l->player->digit = '@';

    l->num_of_boxes = 0;
    int tempwidth = 0;
    int tempheight = 0;
    
    char temparr[MAX_SIZE][MAX_SIZE];
    char c = fgetc(fp);
    while(c!=';')
    {
        tempwidth = 0;
        for(int i = 0; i < MAX_SIZE && c != '\n'; i++)
        {
            int levelIndex;
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
                l->boxes = realloc(l->boxes, sizeof(movable) * l->num_of_boxes);
                l->boxes[levelIndex].digit = '$';
                l->boxes[levelIndex].x = tempwidth;
                l->boxes[levelIndex].y = tempheight;
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
                l->boxes = realloc(l->boxes, sizeof(movable) * l->num_of_boxes);
                l->boxes[levelIndex].digit = '$';
                l->boxes[levelIndex].x = tempwidth;
                l->boxes[levelIndex].y = tempheight;
                c = '.';
                break;
            }
            temparr[tempheight][tempwidth] = c;
            tempwidth++;
            c = fgetc(fp);
        }
        tempheight++;
        c=fgetc(fp);
    }
    
    l->height = tempheight;
    l->width = tempwidth;

    l->map = malloc(sizeof(char*) * tempheight);
    for(int i = 0; i < tempheight; i++)
    {
        l->map[i] = malloc(sizeof(char) * tempwidth);
        for(int j = 0; j < tempwidth; j++)
        {
            l->map[i][j] = temparr[i][j];
        }
    }

    l->s_player = malloc(sizeof(movable));
    l->s_boxes = malloc(sizeof(movable) * l->num_of_boxes);
    l->s_player->digit = '@';
    l->s_player->x = l->player->x;
    l->s_player->y = l->player->y;
    int temp = l->num_of_boxes;
    for(int i = 0; i < temp; i++)
    {
        l->s_boxes[i].digit = '$';
        l->s_boxes[i].x = l->boxes[i].x;
        l->s_boxes[i].y = l->boxes[i].y;
    }


    fclose(fp);
    return l;
}
