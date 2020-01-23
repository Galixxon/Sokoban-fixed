#include "graphical.h"
#include "logic.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define GAME_TEXTURES_AMOUNT 6
#define MENU_TEXTURES_AMOUNT 4
#define TILE_SIZE 32

enum GameImageNames
{
    BACKGOUND,
    FLOOR,
    WALL,
    PLAYER,
    BOX,
    HOLE
};




typedef struct WindowType
{
    SDL_Renderer *renderer;
    SDL_Window *window;
    
}window_t;



void destroyGraphicsAndRenderer(SDL_Texture *textures[], SDL_Renderer *renderer, const int amount)
{
    for(int i = 0; i < amount; i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyRenderer(renderer);
}


//Importing textures by filenames
SDL_Texture *getTexture(const char* filename, SDL_Renderer *renderer)
{
    SDL_Surface *image = IMG_Load(filename);
    SDL_Texture *texture = NULL;
    if(image ==NULL)
    {
        printf("Image could not be loaded %s! SDL Errpr: %s\n", filename,SDL_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer,image);
        if(texture == NULL)
        {
             printf( "Unable to create texture from %s! SDL Error: %s\n", filename, SDL_GetError() );
        }
        SDL_FreeSurface(image);
    }
    
    return texture;
}



//Loading images to memory
int LoadGameImages(SDL_Texture *textures[] ,SDL_Renderer *renderer)
{
    int good = 1;
    textures[BACKGOUND] = getTexture("assets/graphics/Background.png", renderer);
    if(textures[BACKGOUND] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[FLOOR] = getTexture("assets/graphics/Floor.png", renderer);
    if(textures[FLOOR] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[WALL] = getTexture("assets/graphics/Wall.png",renderer);
    if(textures[WALL] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[PLAYER] = getTexture("assets/graphics/Player.png",renderer);
    if(textures[PLAYER] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[BOX] = getTexture("assets/graphics/Box.png",renderer);
    if(textures[BOX] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    textures[HOLE] = getTexture("assets/graphics/Box_hole.png",renderer);
    if(textures[HOLE] == NULL)
    {
        printf( "Failed to load default image!\n" );
        good = 0;
    }
    return good;
}


void LevelSelect()
{
    SDL_Window *selectWin;
    SDL_Renderer *selectRen; 
    SDL_Texture *textures[MENU_TEXTURES_AMOUNT];
    
    //Initialisation
    
    if(SDL_CreateWindowAndRenderer(800,600,SDL_WINDOW_BORDERLESS, &selectWin, &selectRen) != 0)
    {
        printf("Initialization failed. %s", SDL_GetError());
        return;
    }
    //Loading images
    if(LoadGameImages(textures,selectRen) == 0)
    {
        printf("Image Loading error!");
        return;
    }

}

void PrintMap(char **map, const int width, const int height, movable *player, movable **boxes, int numOfBoxes, SDL_Renderer *ren, SDL_Texture *textures[])
{
    SDL_Rect  prev;
    SDL_Rect  new;
    new.w = width * TILE_SIZE;
    new.h = height * TILE_SIZE;
    new.x = (800 - new.w)/2;
    new.y = (600 - new.h)/2;
    SDL_RenderGetViewport(ren,&prev);
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren,textures[BACKGOUND],NULL,NULL);
    SDL_Rect rects[width*height];
    SDL_RenderSetViewport(ren,&new);
    SDL_Rect *actual = NULL;
    int x,y;
    int type;

    //Drawing map
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            actual = &rects[i*height + j];
            x = j * TILE_SIZE;
            y = i * TILE_SIZE;
            actual->x = x;
            actual->y = y;
            actual->h = TILE_SIZE;
            actual->w = TILE_SIZE;
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

            default:
                break;
            }
            SDL_RenderCopy(ren,textures[type], NULL, actual);
        }
    }

    //drawing Player
    SDL_Rect BoxesRects[numOfBoxes];
    for(int i = 0; i < numOfBoxes; i++)
    {
        actual = &BoxesRects[i];
        actual->h = TILE_SIZE;
        actual->w = TILE_SIZE;
        actual->x = boxes[i]->x * TILE_SIZE;
        actual->y = boxes[i]->y * TILE_SIZE;
        SDL_RenderCopy(ren,textures[BOX], NULL,actual);
    }

    SDL_Rect playerRect;
    playerRect.h = TILE_SIZE;
    playerRect.w = TILE_SIZE;
    playerRect.x = player->x * TILE_SIZE;
    playerRect.y = player->y * TILE_SIZE;
    SDL_RenderCopy(ren,textures[PLAYER], NULL, &playerRect);

    SDL_RenderPresent(ren);
    SDL_RenderSetViewport(ren,&prev);
}

void LevelGame(const int index)
{   
    SDL_Window *gamewin;
    SDL_Renderer *gameRen; 
    SDL_Texture *textures[GAME_TEXTURES_AMOUNT];
    
    //Initialisation
    
    if(SDL_CreateWindowAndRenderer(800,600, SDL_WINDOW_BORDERLESS, &gamewin, &gameRen) != 0)
    {
        printf("Initialization failed. %s", SDL_GetError());
        return;
    }
    //Loading images
    if(LoadGameImages(textures,gameRen) == 0)
    {
        printf("Image Loading error!");
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

    PrintMap(map,width,height,player,boxes,numOfBoxes,gameRen,textures);
    while(quit == 0)
    {
        
        while(SDL_PollEvent(&event) != 0)
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
                PrintMap(map,width,height,player,boxes,numOfBoxes,gameRen,textures);
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
                PrintMap(map,width,height,player,boxes,numOfBoxes,gameRen,textures);
            }
            SDL_WaitEventTimeout(&event,250);
            
        }
    }

    destroyGraphicsAndRenderer(textures,gameRen,GAME_TEXTURES_AMOUNT);
    freeLevel(levelinfo);
    SDL_DestroyWindow(gamewin);
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Could not initialize SDL");
        return 0;
    }
    LevelGame(1);

    IMG_Quit();
    SDL_Quit();
    return 0;
}