#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "headers/structs.h"
#define MAPS_PATH "assets/maps.txt"
#define MAX_WIDTH 64
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

level* getLevelInfo(const int index)
{
    
    level* newLevel = malloc(sizeof(level));
    int searcher = 0;
    printf("%d\n", searcher);
    FILE* fp = fopen(MAPS_PATH, "r");
    newLevel->height = 0;
    newLevel->width = 0;
    newLevel->map = NULL;
    newLevel->boxes = NULL;
    newLevel->num_of_boxes;

    if(fp == NULL)
    {
        return NULL;
    }

    char* line = malloc(sizeof(char)*64);
    while(fgets(line,64,fp)!=NULL && searcher != index)
    {
        if(line[0] == ';')
        {
            searcher++;
        }
    }
    char c = fgetc(fp);
    newLevel->map = malloc(sizeof(char*));
    fgets(line,64,fp);
    newLevel->width = strlen(line)-1;
    newLevel->map[0] = malloc(sizeof(char) * newLevel->width);
    for(int i = 0; i < newLevel->width; i++)
    {

        /**
         * '#' - ściana
         * '@' - gracz
         * '$' - skrzynia
         * ' ' - podłoga
         * '.' - dziura
         * '*' - skrzynia na dziurze
         * '+' - gracz na dziurze
         **/
        if(line[i] == '$')
        {
            (newLevel->num_of_boxes)++;
            newLevel->boxes = realloc(newLevel->boxes,sizeof(movable) * newLevel->num_of_boxes);
            newLevel->boxes[newLevel->num_of_boxes-1].digit = '$';
            
            newLevel->map[0][i] = ' ';
        }
        else if(line[i] == '@')
        {
            newLevel->map[0][i] = ' ';
        }
        else if(line[i] == '+')
        {
            newLevel->map[0][i] = '.';
        }
        else if(line[i] == '*')
        {
            newLevel->map[0][i] = '.';
        }
        else
        {
            newLevel->map[0][i] = line[i];
        }
    }
    while(c!=';')
    {
        if(c == '\n')
        {
            newLevel->height += 1;
            newLevel->map = realloc(newLevel->map,sizeof(char*) * newLevel->height);
            
        }
        else
        {

        }
    }

    free(line);
    return NULL;
}

int main()
{
    level* a = getLevelInfo(2);
    return 0;
}