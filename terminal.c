#include "terminal.h"

#include <stdlib.h>
#include <menu.h>
#include <string.h>

void UpdateDisplay(WINDOW *win, char **arr, int w, int h, movable *pl, movable *boxes, int numofboxes)
{
    wmove(win,0,0);
    wrefresh(win);
    char tempPlayer = arr[pl->y][pl->x];
    char tempBoxes[numofboxes];
    char boxChar = boxes[0].digit;
    for(int i = 0; i < numofboxes; i++)
    {
        tempBoxes[i] = arr[boxes[i].y][boxes[i].x];
        arr[boxes[i].y][boxes[i].x] = boxChar;
    }

    arr[pl->y][pl->x] = pl->digit;

    //Drawing
    for(int i = 0; i < numofboxes; i++)
    {
        arr[boxes[i].y][boxes[i].x] = boxes[i].digit;
    }

    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            waddch(win,arr[i][j]);
        }
    }
    wrefresh(win);
    arr[pl->y][pl->x] = tempPlayer;

    for(int i = 0; i < numofboxes; i++)
    {
        arr[boxes[i].y][boxes[i].x] = tempBoxes[i];
    }
}
//--------------------------------------------------------------

int getPlayerInput()
{
    int c = getch();
    int m;
    switch (c)
    {
    case KEY_UP:
        m = 1;
        break;
    
    case KEY_DOWN:
        m = 2;
        break;

    case KEY_LEFT:
        m = 3;
        break;

    case KEY_RIGHT:
        m = 4;
        break;
    case 'q':
        m = 5;
        break;
    case 'p':
        m = 6;
        break;
    }
    return m;
}


void gameLoop(int index)
{
    level* actualLevel = getLevelInfo(index);
    if(actualLevel == NULL)
    {
        wprintw(stdscr, "Could not load a level. Press any key to exit");
        getch();
        return;
    }
    int window_width = actualLevel->width;
    int window_height = actualLevel->height;
    int window_x = (COLS - window_width)/2;
    int window_y = (LINES - window_height)/2;
    WINDOW *win = newwin(window_height,window_width,window_y,window_x);

    wborder(win, 0,0,0,0,0,0,0,0);
    int move = 0;
    int state = 0;

    UpdateDisplay(win,actualLevel->map,actualLevel->width,actualLevel->height,actualLevel->player,actualLevel->boxes, actualLevel->num_of_boxes);

    while(state == 0 && (move = getPlayerInput()) != 5)
    {
        if(move == 6)
        {
            reset(actualLevel->player,actualLevel->s_player,actualLevel->boxes,actualLevel->s_boxes,actualLevel->num_of_boxes);
        }
        else
        {
            state = step(move,actualLevel->map,actualLevel->player,actualLevel->boxes,actualLevel->num_of_boxes,actualLevel->height,actualLevel->width);
        }
        UpdateDisplay(win,actualLevel->map,actualLevel->width,actualLevel->height,actualLevel->player,actualLevel->boxes, actualLevel->num_of_boxes);
    }
    delwin(win);
}

int main()
{
    initscr();
    noecho();
    cbreak();
    start_color();
    keypad(stdscr,TRUE);
    //gameLoop(0);
    
    endwin();
    return 0;
}