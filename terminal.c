#include "terminal.h"

#include <stdlib.h>
#include <menu.h>
#include <string.h>

void gameLoop(int index);

void UpdateDisplay(WINDOW *win, char **arr, int w, int h, movable *pl, movable **boxes, int numofboxes)
{
    wmove(win,0,0);
    wrefresh(win);
    //Drawing
    int color = 0;
    char c;
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            
            c = arr[i][j];
            switch (c)
            {
            case '.':
                color = 1;
                break;
            case '#':
                color = 2;
                break;        
            default:
                color = 0;
                break;
            }
            wattrset(win,COLOR_PAIR(color));
            if(c != '0')
            {
                waddch(win,c);
            }
            else
            {
                waddch(win,' ');
            }
            
        }
    }
    wattrset(win,COLOR_PAIR(3));
    for(int i= 0; i < numofboxes; i++)
    {
        mvwaddch(win,boxes[i]->y,boxes[i]->x, boxes[i]->digit);
    }
    wattrset(win,COLOR_PAIR(4));
    mvwaddch(win,pl->y,pl->x,pl->digit);
    wrefresh(win);
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

void levelSelect()
{

    ITEM **levellist;
    MENU *menu;
    ITEM *current;
    int levelamount = getLevelAmount();
    levellist = calloc(levelamount+1,sizeof(ITEM*));
    int size = 0;
    int temp = levelamount;
    int c;
    while(temp > 0)
    {
        size++;
        temp/=10;
    }
    char* levelString[levelamount];
    char label[size+1];
    for(int i = 0; i < levelamount;i++)
    {
        levelString[i] = malloc(sizeof(char) * (size + 8));
        strcpy(levelString[i], "Level ");
        sprintf(label,"%d", i+1);
        strcat(levelString[i], label);
        levellist[i] = new_item(levelString[i],"*");
    }
    levellist[levelamount] = (ITEM*)NULL;

    menu = new_menu(levellist);
    mvprintw(LINES - 2, 0, "Press Q to Exit");
    post_menu(menu);
    refresh();
    while((c=getch())!= 'q')
    {
        current = current_item(menu);
        switch (c)
        {
        case KEY_DOWN:
            menu_driver(menu,REQ_DOWN_ITEM);
            break;
        
        case KEY_UP:
            menu_driver(menu,REQ_UP_ITEM);
            break;
        case 10:
            unpost_menu(menu);
            refresh();
            gameLoop((item_index(current_item(menu)))+1);
            post_menu(menu);
            refresh();
            break;
        default:
            break;
        }
        

    }
    for(int i = 0; i < levelamount;i++)
    {
        free_item(levellist[i]);
        free(levelString[i]);
    }
    free_menu(menu);
}

void gameLoop(int index)
{

    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_CYAN,COLOR_BLACK);
    init_pair(3,COLOR_RED,COLOR_BLACK);
    init_pair(4,COLOR_YELLOW,COLOR_BLACK);

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
    


    const int height = actualLevel->height;
    const int width = actualLevel->width;
    movable *p = actualLevel->player;
    movable **boxes = actualLevel->boxes;
    const int numOfBoxes = actualLevel->num_of_boxes;
    char **map = actualLevel->map;
    
    int move = 0;
    int state = 0;

    UpdateDisplay(win, map,width,height,p,boxes,numOfBoxes);
    while(state == 0 && (move = getPlayerInput()) != 5)
    {
        if(move == 6)
        {
            //reset(p,s_player,boxes,s_boxes,numOfBoxes);
        }
        else
        {
            state = step(move,map,p,boxes,numOfBoxes,height,width);
        }
        UpdateDisplay(win, map,width,height,p,boxes,numOfBoxes);
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
    curs_set(0);

    levelSelect();
    curs_set(1);
    endwin();
    return 0;
}