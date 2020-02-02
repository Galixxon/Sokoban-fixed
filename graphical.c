#include "logic.h"
#include "levelmanager.h"
#include "graphical.h"

#include <stdio.h>
#include <stdlib.h>

#define MENU_HEIGHT 600
#define MENU_WIDTH 800
#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024
#define FONT_SIZE 28
#define BUTTON_SIZE 64

#define FONT_PATH "assets/Fonts/MenuFont.ttf"

enum Images
{
    NOTHING,
    BACKGROUND,
    FLOOR,
    WALL,
    HOLE,
    PLAYER,
    BOX,
    LEVEL_BUTTON,
    PAGE_BUTTON,
    TOTAL_TEXTURE_AMOUNT
};

enum Move
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct tile
{
    SDL_Texture *tileText;
    int x,y;
    int size;
};

struct button
{
    SDL_Texture *label;
    SDL_Rect pos;
    SDL_Rect textPos;
    int index;
};

int checkButtonClickEvent(SDL_Rect *buttonArea)
{
    int x,y;
    SDL_GetMouseState(&x,&y);
    if(x > buttonArea->x && x < buttonArea->x+ buttonArea->w && y > buttonArea->y && y < buttonArea->y + buttonArea->h)
    {
        return 1;
    }
    return 0;
}

int catchMoveEvent(SDL_Event e)
{
    int move = NONE;
    switch (e.key.keysym.sym)
    {
    case SDLK_UP:
        move = UP;
        break;
    
    case SDLK_DOWN:
        move = DOWN;
        break;

    case SDLK_LEFT:
        move = LEFT;
        break;

    case SDLK_RIGHT:
        move = RIGHT;
        break;
    default:
        break;
    }
    return move;
}

int SdlInit()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Could not initialize SDL\n");
        return 0;
    }
    if(TTF_Init() == -1)
    {
        printf("Could not initialize TTF module\n");
        return 0;
    }
    return 1;
}

SDL_Texture *readImageFromPath(const char* path, SDL_Renderer *renderer)
{
    SDL_Surface *image = IMG_Load(path);
    SDL_Texture *texture = NULL;
    if(image ==NULL)
    {
        printf("Image could not be loaded %s! SDL Error: %s\n", path,SDL_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer,image);
        if(texture == NULL)
        {
             printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
        }
        SDL_FreeSurface(image);
    }
    return texture;
}

int LoadGameImages(SDL_Texture *textures[] ,SDL_Renderer *renderer)
{
    int good = 1;
    textures[BACKGROUND] = readImageFromPath("assets/graphics/Background.png", renderer);
    if(textures[BACKGROUND] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[FLOOR] = readImageFromPath("assets/graphics/Floor.png", renderer);
    if(textures[FLOOR] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[WALL] = readImageFromPath("assets/graphics/Wall.png",renderer);
    if(textures[WALL] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[PLAYER] = readImageFromPath("assets/graphics/Player.png",renderer);
    if(textures[PLAYER] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[BOX] = readImageFromPath("assets/graphics/Box.png",renderer);
    if(textures[BOX] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[HOLE] = readImageFromPath("assets/graphics/Box_hole.png",renderer);
    if(textures[HOLE] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[LEVEL_BUTTON] = readImageFromPath("assets/graphics/Button.png", renderer);
    if(textures[LEVEL_BUTTON] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    return good;
}

void SDLClose()
{
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void setButtonLabel(SDL_Renderer *ren,menu_button *btn, TTF_Font *font, char* label)
{
    SDL_Color color = {0,0,0,0};
    SDL_Surface *temp = TTF_RenderText_Solid(font,label,color);
    btn->label = SDL_CreateTextureFromSurface(ren,temp);
    SDL_FreeSurface(temp);
}

void PrintButton(SDL_Renderer *renderer, menu_button button, SDL_Texture *btntext)
{
    SDL_RenderCopy(renderer,btntext,NULL,&(button.pos));
    SDL_RenderCopy(renderer,button.label,NULL,&(button.textPos));
}

void setButtonRects(menu_button *button, int x,int y, int SIZE)
{
    button->pos.h = SIZE;
    button->pos.w = SIZE;
    button->pos.x = x;
    button->pos.y = y;

    button->textPos.h = 0.5 * SIZE;
    button->textPos.w = 0.5 * SIZE;
    button->textPos.x = x + 0.25 * SIZE;
    button->textPos.y = y + 0.25 * SIZE;
}

void setPageButtonRects(menu_button *button, int x,int y, int w, int h)
{
    button->pos.h = h;
    button->pos.w = w;
    button->pos.x = x;
    button->pos.y = y;

    button->textPos.x = x+15;
    button->textPos.y = y+15;
    button->textPos.h = h * 0.9;
    button->textPos.w = w * 0.9;
}


void setAdditionalButtons(SDL_Window *win, SDL_Renderer *renderer, TTF_Font *font, menu_button *prev, menu_button *next)
{
    int w,h;
    SDL_GetWindowSize(win,&w,&h);

    setPageButtonRects(prev, (w - 7*BUTTON_SIZE)/2 ,h-1.25*BUTTON_SIZE,3*BUTTON_SIZE,BUTTON_SIZE);
    setPageButtonRects(next, (w - 7*BUTTON_SIZE)/2 + 4*BUTTON_SIZE ,h-1.25*BUTTON_SIZE,3*BUTTON_SIZE,BUTTON_SIZE);
    setButtonLabel(renderer,prev, font, "Previous");
    setButtonLabel(renderer,next, font, "Next");
}

void updateSelectionButtons(SDL_Window *win,SDL_Renderer *renderer, TTF_Font *font, menu_button buttons[],int startingIndex, int rows, int columns, int numOfLevels)
{
    int w,h;
    SDL_GetWindowSize(win,&w,&h);

    int startx = (w - 7*BUTTON_SIZE)/2;
    int starty = h - 8.5*BUTTON_SIZE;

    int temp = startingIndex + (rows*columns);
    int size = 0;
    while(temp > 0)
    {
        size++;
        temp/=10;
    }
    char label[size+1];
    int labelNumber = 0;
    int over = 0;

    for(int i = 0; i < rows && !over; i++)
    {
        for(int j = 0; j < columns && !over; j++)
        {
            labelNumber = columns*i + j;
            if(labelNumber+startingIndex > numOfLevels)
            {
                buttons[labelNumber].index = 0;
                over = 1;
                break;
            }
            buttons[labelNumber].index = labelNumber + startingIndex;
            sprintf(label,"%d", buttons[labelNumber].index);
            setButtonRects(&(buttons[labelNumber]), startx + 1.5*BUTTON_SIZE*j,starty + 1.5*BUTTON_SIZE*i,BUTTON_SIZE);
            setButtonLabel(renderer,&(buttons[labelNumber]), font, label);
        }
    }
}

void renderButtons(menu_button buttons[], menu_button next, menu_button prev, SDL_Renderer *renderer, int rows, int columns, SDL_Texture *btntexture)
{
    int over = 0;
    for(int i  = 0; i < rows && !over;i++)
    {
        for(int j = 0; j < columns && !over;j++)
        {
            if(buttons[columns*i+j].index == 0)
            {
                over = 1;
                break;
            }
            PrintButton(renderer,buttons[columns*i + j],btntexture);
        }
    }
    PrintButton(renderer,next,btntexture);
    PrintButton(renderer,prev,btntexture);
}

void PrintTile(SDL_Renderer *renderer, tile tile, int tile_size)
{
    SDL_Rect rect = {tile.x,tile.y,tile_size,tile_size};
    SDL_RenderCopy(renderer,tile.tileText,NULL,&rect);
}

void PrintMap(char **map, const int width, const int height, movable *player, movable **boxes, int numOfBoxes, SDL_Renderer *ren, SDL_Texture *textures[])
{
    int tileSize = 64;
    while(tileSize*width > SCREEN_WIDTH || tileSize*height > SCREEN_HEIGHT)
    {
        tileSize -= 8;
    }

    SDL_RenderClear(ren);
    SDL_RenderCopy(ren,textures[BACKGROUND],NULL,NULL);
    tile tiles[width*height];
    SDL_Rect  prev;
    SDL_Rect new;
    new.w = width * tileSize;
    new.h = height * tileSize;
    new.x = (SCREEN_WIDTH - new.w)/2;
    new.y = (SCREEN_HEIGHT - new.h)/2;
    SDL_RenderGetViewport(ren,&prev);
    SDL_RenderSetViewport(ren,&new);
    int type;

    //Drawing map
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            tiles[width * i + j].x = j * tileSize;
            tiles[width * i + j].y = i * tileSize;
            switch (map[i][j])
            {
            case '#':
                type = WALL;
                break;
            
            case ' ':
                type = FLOOR;
                break;

            case '.':
                type = HOLE;
                break;

            case '0':
                type = 0;
                break;
                
            default:
                break;
            }
            if(type!=0)
                {
                    tiles[width*i + j].tileText = textures[type];
                    PrintTile(ren,tiles[width*i + j], tileSize);
                }
        }
    }

    //drawing Player
    tile Boxes[numOfBoxes];
    for(int i = 0; i < numOfBoxes; i++)
    {
        Boxes[i].tileText = textures[BOX];
        Boxes[i].x = boxes[i]->x * tileSize;
        Boxes[i].y = boxes[i]->y * tileSize;
        PrintTile(ren,Boxes[i], tileSize);
    }

    tile playerTile;
    playerTile.tileText = textures[PLAYER];
    playerTile.x = player->x * tileSize;
    playerTile.y = player->y * tileSize;
    PrintTile(ren,playerTile,tileSize);

    SDL_RenderSetViewport(ren,&prev);
    SDL_RenderPresent(ren);
}


void Game(int index)
{
    SDL_Window *gameWindow;
    SDL_Renderer *renderer;

    if(SDL_CreateWindowAndRenderer(SCREEN_WIDTH,SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS, &gameWindow, &renderer) != 0)
    {
        printf("Initialization failed. %s", SDL_GetError());
        return;
    }
    SDL_Texture *assets[TOTAL_TEXTURE_AMOUNT];
    if(LoadGameImages(assets,renderer) == 0)
    {
        printf("Loading images failed.");
        return;
    }


    SDL_Event event;
    int quit = 0;
    int move = 0;

    level *levelinfo = getLevelInfo(index);
    if(levelinfo == NULL)
    {
        printf("Could not laod a level");
        return;
    }
    //level info extraction
    movable *player = levelinfo->player;
    movable startingPlayerInfo; 
    startingPlayerInfo.x = levelinfo->player->x;
    startingPlayerInfo.y = levelinfo->player->y;
    int numOfBoxes = levelinfo->num_of_boxes;
    movable **boxes = levelinfo->boxes;
    movable *startinBoxesInfo[numOfBoxes];
    char **map = levelinfo->map;
    const int height = levelinfo->height;
    const int width = levelinfo->width;

    for(int i = 0; i < numOfBoxes; i++)
    {
        startinBoxesInfo[i] = malloc(sizeof(movable));
        memcpy(startinBoxesInfo[i], boxes[i], sizeof(movable));
    }

    PrintMap(map,width,height,player,boxes,numOfBoxes,renderer,assets);
    
    while(quit == 0)
    {
        while(SDL_PollEvent(&event))
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                quit = 1;
                move = 0;
                break;
            case SDLK_r:
                move = 0;
                reset(player,&startingPlayerInfo,boxes,startinBoxesInfo,numOfBoxes);
                PrintMap(map,width,height,player,boxes,numOfBoxes,renderer,assets);
                break;
            case SDLK_UP:
                move = 1;
                break;
            
            case SDLK_DOWN:
                move = 2;
                break;

            case SDLK_LEFT:
                move = 3;
                break;

            case SDLK_RIGHT:
                move = 4;
                break;
            default:
                move = 0;
                break;
            }
            
            if(move > 0)
            {
                quit = step(move,map,player,boxes,numOfBoxes,height,width);
                PrintMap(map,width,height,player,boxes,numOfBoxes,renderer,assets);
            }
            SDL_WaitEventTimeout(&event,150);
        }
    }


    SDL_DestroyRenderer(renderer);
    freeLevel(levelinfo);
    SDL_DestroyWindow(gameWindow);
}


void LevelSelect()
{
    //Getting number of all levels in file and checking whether file is empty
    int numOfLevels = getLevelAmount();
    if(numOfLevels < 1)
    {
        printf("File with levels is empty. Fill it with levels.\n");
        return;
    }

    SDL_Window *gameWindow;
    SDL_Renderer *renderer;
    //Creating level select window
    if(SDL_CreateWindowAndRenderer(MENU_WIDTH,MENU_HEIGHT, SDL_WINDOW_SHOWN, &gameWindow, &renderer) != 0)
    {
        printf("Level select window initialization failed. %s", SDL_GetError());
        return;
    }

    SDL_Texture *assets[TOTAL_TEXTURE_AMOUNT];
    //Loading all images since starting a program
    if(LoadGameImages(assets,renderer) == 0)
    {
        printf("Loading images failed.");
        return;
    }



    SDL_Event e;
    int quit = 0;

    const int numOfDrawnButtons = 25;
    const int cols = 5;
    const int rows = 5;
    menu_button buttons[25];
    menu_button next,prev;
    TTF_Font *font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    
    setAdditionalButtons(gameWindow,renderer,font,&prev, &next);


    updateSelectionButtons(gameWindow,renderer,font,buttons,1,5,5,numOfLevels);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,assets[BACKGROUND], NULL,NULL);
    renderButtons(buttons, next, prev, renderer,5,5,assets[LEVEL_BUTTON]);
    SDL_RenderPresent(renderer);
    int startingIndex = 1;
    int playindex = 0;
    while(!quit)
    {
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
                case SDL_QUIT:
                    quit = 1;
                    break;

                case SDL_MOUSEBUTTONDOWN:

                    for(int i = 0; i< numOfDrawnButtons; i++)
                    {
                        if(checkButtonClickEvent(&(buttons[i].pos)))
                        {
                            playindex = buttons[i].index;
                        }
                    }
                        
                    if(startingIndex - numOfDrawnButtons > 0 && checkButtonClickEvent(&prev.pos))
                    {
                        startingIndex -= numOfDrawnButtons;
                        updateSelectionButtons(gameWindow,renderer,font,buttons,startingIndex,rows,cols,numOfLevels);
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer,assets[BACKGROUND], NULL,NULL);
                        renderButtons(buttons, next, prev, renderer,5,5,assets[LEVEL_BUTTON]);
                        SDL_RenderPresent(renderer);
                    }
                    else if(startingIndex + numOfDrawnButtons < numOfLevels && checkButtonClickEvent(&next.pos))
                    {
                        startingIndex += numOfDrawnButtons;
                        updateSelectionButtons(gameWindow,renderer,font,buttons,startingIndex,rows,cols,numOfLevels);
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer,assets[BACKGROUND], NULL,NULL);
                        renderButtons(buttons, next, prev, renderer,5,5,assets[LEVEL_BUTTON]);
                        SDL_RenderPresent(renderer); 
                    }

                    if(playindex!=0)
                    {
                        SDL_HideWindow(gameWindow);
                        SDL_Delay(500);
                        Game(playindex);
                        SDL_ShowWindow(gameWindow);
                        SDL_RenderPresent(renderer);
                        playindex = 0;
                    }
                    break;  
            }
        }  
    }
}

int main()
{
    SdlInit();
    LevelSelect();
    SDLClose();
}



