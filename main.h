#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_blendmode.h"
#include "SDL2-2.0.10/include/SDL_keycode.h"
#include "SDL2-2.0.10/include/SDL_rect.h"


#include "./SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_render.h"
#include "SDL2-2.0.10/include/SDL_surface.h"
#include "SDL2-2.0.10/include/SDL_video.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

typedef enum Colors{
    RED,
    GREEN,
    BLUE,
}Colors;



typedef struct GameState{
    int time;


    SDL_Event event;

    SDL_Surface *screen, *charset, *player_surface;
    SDL_Texture *scrtex;
    SDL_Window *window;
    SDL_Renderer *renderer;

}GameState;


#ifndef UTIL


// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset);

// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt rodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y,
                 float scale);

void DrawTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y,
                 int w, int h, float scale, double rotation_angle,
                 const SDL_RendererFlip flip);

// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);

// rysowanie linii o d³ugoci l w pionie (gdy dx = 0, dy = 1)
// b¹d poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0)
// line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy,
              Uint32 color);

// rysowanie prostok¹ta o d³ugoci boków l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor);





#endif // !UTIL
