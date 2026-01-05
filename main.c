
#include "main.h"
#include "logic.h"
#include "render.h"

const int WORLD_WIDTH = SCREEN_WIDTH;
const int BACKGROUND_HEIGHT = -10 + SCREEN_HEIGHT/4;
// const int WORLD_HEIGHT = SCREEN_HEIGHT;
const int WORLD_HEIGHT = (int)(SCREEN_HEIGHT - BACKGROUND_HEIGHT);

const int WORLD_MAX_Y = SCREEN_HEIGHT;
const int WORLD_MIN_Y = BACKGROUND_HEIGHT;

const int WORLD_MAX_X = 1000;
const int WORLD_MIN_X = 0;
//TODO: fix attack hitboxes


int main(int argc, char** argv){
    
    
    GameState *gms = malloc(sizeof(GameState));
    printf("jfjfffdgd\n");


    printf("chuiju");
    mainLoop(gms);

    free(gms);

   return 0;
}

// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = 8;
    d.h = 8;
    while (*text) {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = x;
        d.y = y;
        SDL_BlitSurface(charset, &s, screen, &d);
        x += 8;
        text++;
    };
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt rodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y,
                 float scale) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y - sprite->h;
    dest.w = sprite->w * scale;
    dest.h = sprite->h * scale;
    SDL_BlitScaled(sprite, NULL, screen, &dest);
};

void DrawTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y,
                 int w, int h, float scale, double rotation_angle,
                 const SDL_RendererFlip flip) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y - h * scale;
    dest.w = w * scale;
    dest.h = h * scale;

    SDL_RenderCopyEx(renderer, texture, NULL, &dest, rotation_angle, NULL,
                     flip);
}

// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *)p = color;
};

// rysowanie linii o d³ugoci l w pionie (gdy dx = 0, dy = 1)
// b¹d poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0)
// line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy,
              Uint32 color) {
    for (int i = 0; i < l; i++) {
        DrawPixel(screen, x, y, color);
        x += dx;
        y += dy;
    };
};

// rysowanie prostok¹ta o d³ugoci boków l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
    int i;
    DrawLine(screen, x, y, k, 0, 1, outlineColor);
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
    DrawLine(screen, x, y, l, 1, 0, outlineColor);
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
    for (i = y + 1; i < y + k - 1; i++)
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};