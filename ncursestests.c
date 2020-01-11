#include <ncurses.h>
#include <string.h>

WINDOW *createNewWindow(int height, int width, int starty, int startx);
void destroyWindow(WINDOW *win);

int main()
{
    WINDOW *win;
    int startx,starty, width, height;
    int ch;

    initscr();
    cbreak();

    keypad(stdscr,TRUE);
    height = 3;
    width = 10;
    starty = (LINES - height) / 2;
    startx = (COLS - width) / 2;
    printw("Press F1 to exit");
    refresh();
    win = createNewWindow(height, width, starty, startx);

    while((ch = getch()) != KEY_F(1))
    {
        switch (ch)
        {
        case KEY_LEFT:
            destroyWindow(win);
            win = createNewWindow(height, width, starty, --startx);
            break;
        
        case KEY_RIGHT:
            destroyWindow(win);
            win = createNewWindow(height, width, starty, ++startx);
            break;

        case KEY_UP:
            destroyWindow(win);
            win = createNewWindow(height, width, --starty, startx);
            break;

        case KEY_DOWN:
            destroyWindow(win);
            win = createNewWindow(height, width, ++starty, startx);
            break;
        default:
            break;
        }
    }

    endwin();
    return 0;
}

WINDOW *createNewWindow(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height,width,starty,startx);
    box(local_win,0,0);
    wrefresh(local_win);
    return local_win;
}

void destroyWindow(WINDOW *win)
{
    wborder(win,' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(win);
    delwin(win);
}