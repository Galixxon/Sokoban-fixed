#ifndef GRAPHICAL_H_
#define GRAPHICAL_H_

#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

struct tile;
struct button;
typedef struct tile tile;
typedef struct button menu_button;


int checkButtonClickEvent(SDL_Rect *buttonArea);
int catchMoveEvent(SDL_Event e);
int SdlInit();
SDL_Texture *readImageFromPath(const char* path, SDL_Renderer *renderer);
int LoadGameImages(SDL_Texture *textures[] ,SDL_Renderer *renderer);
void SDLClose();
void setButtonLabel(SDL_Renderer *ren,menu_button *btn, TTF_Font *font, char* label);
void PrintButton(SDL_Renderer *renderer, menu_button button, SDL_Texture *btntext);
void setButtonRects(menu_button *button, int x,int y, int SIZE);
void setPageButtonRects(menu_button *button, int x,int y, int w, int h);
void setAdditionalButtons(SDL_Window *win, SDL_Renderer *renderer, TTF_Font *font, menu_button *prev, menu_button *next);
void updateSelectionButtons(SDL_Window *win,SDL_Renderer *renderer, TTF_Font *font, menu_button *buttons,int startingIndex, int rows, int columns, int numOfLevels);
void renderButtons(menu_button *buttons, menu_button next, menu_button prev, SDL_Renderer *renderer, int rows, int columns, SDL_Texture *btntexture);
void PrintTile(SDL_Renderer *renderer, tile tile, int tile_size);
void PrintMap(char **map, const int width, const int height, movable *player, movable **boxes, int numOfBoxes, SDL_Renderer *ren, SDL_Texture *textures[]);
void Game(int index);
void LevelSelect();

#endif