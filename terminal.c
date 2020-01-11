#include "headers/terminal.h"
#include <ncurses.h>

//Basic Drawing
void printMap(field **arr, int w, int h, movable_t *pl, movable_t *boxes, int numofboxes)
{
    char pl_temp = pl->pos->digit;
    char boxes_temp[numofboxes];
    pl->pos->digit = 'P';

    for(int i = 0; i < numofboxes; i++)
    {
        boxes_temp[i] = boxes[i].pos->digit;
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
    pl->pos->digit = pl_temp;
    for(int i = 0; i < numofboxes; i++)
    {
        boxes[i].pos->digit = boxes_temp[i];
    }
}
//--------------------------------------------------------------

move_t getPlayerInput()
{
    char* c = malloc(sizeof(char));
    move_t m;
    fgets(c,4,stdin);
    switch (c[0])
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
    case 'Q':
        m = QUIT;
        break;
    case 'P':
        m = RESET;
        break;
    }
    free(c);
    return m;
}

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

int main()
{
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
}