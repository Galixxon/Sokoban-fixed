#include "graphical.h"
#include "logic.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>


#define TEXTURES_AMOUNT 6
enum ImageNames
{
    BACKGOUND,
    FLOOR,
    WALL,
    PLAYER,
    BOX,
    HOLE
};

/**
SDL_Window* InitLevelSelect(const int width, const int height);

int InitGameScreen(const int width,const int height);

SDL_Texture* LoadImages(SDL_Texture *textures[] ,SDL_Renderer *renderer);
SDL_Surface *getImage(const char* filename,const SDL_PixelFormat *format);

void closeLevel(SDL_Window *level);
SDL_Window *closeGame(SDL_Window *levelselect);
**/
//Importing textures

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
int LoadImages(SDL_Texture *textures[] ,SDL_Renderer *renderer)
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

SDL_Window* InitLevelSelect(const int width, const int height)
{
    SDL_Window* mainWin = NULL;
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return NULL;
    }
    else
    {
        mainWin = SDL_CreateWindow("Sokoban Level Select", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN);
        if(mainWin == NULL)
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return NULL;
        }
        else
        {
            return mainWin;
        }
    }


    return NULL;
}

void *closeGame(SDL_Window *level, SDL_Texture *textures[], SDL_Renderer *renderer)
{
    for(int i = 0; i < TEXTURES_AMOUNT; i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(level);
    IMG_Quit();
}

void PrintMap(SDL_Renderer *ren, SDL_Texture *textures[],const int width,const int height, movable *player,
                     movable *boxes, int amount_of_boxes)
{

}

void LevelSelect()
{
    int quit = 0;
    int success = 1;
    SDL_Window *game = InitLevelSelect(800,600);
    SDL_Texture *images[TEXTURES_AMOUNT];
    SDL_Renderer *renderer = NULL;
    level* actualLevel = NULL;

    const int height = actualLevel->height;
    const int width = actualLevel->width;
    movable *p = actualLevel->player;
    movable *boxes = actualLevel->boxes;
    const int numOfBoxes = actualLevel->num_of_boxes;
    char **map = actualLevel->map;

    movable *s_player = actualLevel->s_player;
    movable *s_boxes = actualLevel->s_boxes;

    int actualmove = 0;
    if(game == NULL)
    {
        printf( "Failed to initialize!\n" );
        success =0;
    }
    else
    {

        renderer = SDL_CreateRenderer(game, -1,SDL_RENDERER_ACCELERATED);
        if(renderer == NULL)
        {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success=0;
        }
        else
        {
            SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags))
            {
                printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() ); 
                success=0;
            }
        }
    }
    if(success == 0)
    {
        closeGame(game,images,renderer);
        return;
    }

    SDL_UpdateWindowSurface(game);

    SDL_Event e;
    while(!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = 1;
            }
            else if(e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_UP:
                    actualmove = 1;
                    break;
                
                case SDLK_DOWN:
                    actualmove = 2;
                    break;

                case SDLK_LEFT:
                    actualmove = 3;
                    break;

                case SDLK_RIGHT:
                    actualmove = 4;
                    break; 
                default:
                    actualmove = 0;
                    break;
                }
                if (actualmove>0)
                {
                    quit = step(actualmove,map,p,boxes,numOfBoxes,height,width);
                }
                else if (actualmove == -1)
                {
                    quit = 1;
                }
                else if (actualmove == -2)
                {
                    reset(p,s_player,boxes,s_boxes,numOfBoxes);
                }
            }
        }
        SDL_RenderClear(renderer);
        //PrintMap(renderer,images,width,height,p,boxes,numOfBoxes);
    }


    closeGame(game, images,renderer);
}

int main()
{
    LevelSelect();
    return 0;
}