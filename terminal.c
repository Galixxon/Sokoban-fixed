#include "headers/terminal.h"
#include "headers/logic.h"
#include <ncurses.h>

//Basic Drawing
void printMap(char **arr, int w, int h, movable *pl, movable *boxes, int numofboxes)
{
    char pl_temp = pl->digit;
    char boxes_temp[numofboxes];
    pl->digit = 'P';

    for(int i = 0; i < numofboxes; i++)
    {
        boxes_temp[i] = boxes[i].digit;
        boxes[i].digit = 'O';
    }
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            putchar(arr[i][j]);
        }
        putchar('\n');
    }
    pl->digit = pl_temp;
    for(int i = 0; i < numofboxes; i++)
    {
        boxes[i].digit = boxes_temp[i];
    }
}
//--------------------------------------------------------------

int getPlayerInput()
{
    char* c = malloc(sizeof(char));
    int m;
    fgets(c,4,stdin);
    switch (*c)
    {
    case 'U':
        m = 1;
        break;
    
    case 'D':
        m = 2;
        break;

    case 'L':
        m = 3;
        break;

    case 'R':
        m = 4;
        break;
    case 'Q':
        m = 5;
        break;
    case 'P':
        m = 6;
        break;
    }
    free(c);
    return m;
}
/**
level* levelSelection()
{
    level *chosen = NULL;
    int height = 10;
    int width = 10;
    int startx = (LINES - height)/2;
    int starty = (COLS - width)/2;
    WINDOW *selection_win;
    selection_win = newwin(height,width,starty,startx);
    box(selection_win,0,0);
    wrefresh(selection_win);

    int ch;
    while(ch = getch() != KEY_F(1) && chosen == NULL)
    {
        switch (ch)
        {
        case KEY_UP:
            break;
        case KEY_DOWN:
            break;
        case KEY_ENTER:
            break;
        default:
            break;
        }
    }
    wborder(selection_win,' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(selection_win);
    delwin(selection_win);
    return chosen;
}
**/
int main()
{
    /**
    WINDOW *win;
    int startx, starty, width, height;
    initscr();
    cbreak();

    keypad(stdscr,TRUE);
    refresh();
    
    level *actualLevel = levelSelection();
    
    if(actualLevel == NULL)
    {
        printw("Error, could not read a level");
        return;
    }
    int *height = actualLevel->height;
    int *width = actualLevel->width;
    field **map = actualLevel->map;
    int *numOfBoxes = actualLevel->numOfBoxes;
    movable_t *player = actualLevel->playerinfo;
    movable_t *boxes = actualLevel->boxes;
    **/
   level* actualLevel = malloc(sizeof(level));
   
   int move; 
   while((move= getPlayerInput())!= 5)
   {
       
   }
   return 0;
}