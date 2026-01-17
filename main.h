#pragma once
#include "./SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_blendmode.h"
#include "SDL2-2.0.10/include/SDL_keycode.h"
#include "SDL2-2.0.10/include/SDL_rect.h"

#include "./SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_render.h"
#include "SDL2-2.0.10/include/SDL_surface.h"
#include "SDL2-2.0.10/include/SDL_video.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
extern const int WORLD_WIDTH;
extern const int BACKGROUND_HEIGHT;
extern const int WORLD_HEIGHT;
extern const int WORLD_MAX_Y;
extern const int WORLD_MIN_Y;
extern const int WORLD_MAX_X;
extern const int WORLD_MIN_X;
#define CAMERA_GRACE 100
#define PLAYER_LEFT -1
#define PLAYER_RIGHT 1
#define PLAYER_UP -1
#define PLAYER_DOWN 1
#define PLAYER_SPEED 300
#define RESET_ACTION 0
#define PLAYER_JUMP_TIME 0.5
#define PLAYERS_LIGHT_ACTION_DURATION_S 0.2
#define PLAYERS_HEAVY_ACTION_DURATION_S 0.7
#define PLAYER_LIGHT_REACH_px 100
#define PLAYER_HEAVY_REACH_px 200
#define COMBO_DELAY_S 2
#define DASH_TIME 0.1
#define MAX_ATTEMPTS 1000
#define PLAYER_MAX_HEALTH 100

#define NUMOFOBSTACLES 10

#define NOACTION 0
#define ACTION 1
#define NOACTIONCHAR "..."

typedef enum {
    LEFT = -1,
    RIGHT = 1,
    UP = -1,
    DOWN = 1,
    JUMP = 1,
    LIGHT_ATTACK = 1,
    HEAVY_ATTACK = -1,

} Action;

typedef enum Colors {
    RED,
    BLACK,
    GREEN,
    BLUE,
    SKYBLUE,
    TITLEGRAY,

} Colors;

typedef struct {
    int type;
    double Time_s;
    double RemainingTime_s;

} Player_actions;

typedef struct Position {
    float x;
    float y;
} Position;

typedef struct Direction {
    char x;
    char y;
} Direction;

typedef struct PlayerColor {

    int lighAttack;
    int heavyAttack;

} PlayerColor;

typedef struct {

    int comboType;
    double comboTimeRemaining;
    double comboInitialTime;

} Combo;

typedef struct Player {
    SDL_Rect rect;
    int speed;
    int remainingHp;
    Direction direction;
    SDL_Surface *surface;
    SDL_Texture *texture;
    float scale;
    Position position;
    char **recentActions;
    int isJumping;
    double jumpingTime_s;
    double RemainingJumpTime_s;
    Player_actions actions;
    int isAction;
    PlayerColor color;
    int lastHeading;
    SDL_Rect *attackHitbox;

    Combo comboType;

} Player;

typedef enum {
    LEFT_DASH = 1,
    RIGHT_DASH = 2,
    UP_DASH = 3,
    DOWN_DASH = 4,
    LIGHT_COMBO = 5,
    HEAVY_COMBO = 6,
    NO_COMBO = 0,
    SUPERCOMBO = 7,

} ComboTypes;

typedef struct Enemy {
    int Remaininghealth;
    int Damage;

} Enemy;

typedef struct {
    SDL_Rect rect;
    int health;
    int isInitialized;
} Entity;

typedef struct GameSession {

    SDL_Event event;

    SDL_Surface *screen, *charset;
    SDL_Texture *scrtex;
    SDL_Window *window;
    SDL_Renderer *renderer;

} GameSession;

typedef struct GameState {
    Player *p;
    double *worldTime;
    int *camera_offset;
    GameSession *gs;
    Entity *entites;
    int *debug_exit;
    int *players_points;

} GameState;

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

void DrawFrame(SDL_Surface *screen, int x, int y, int l, int k,
               Uint32 outlineColor);

#endif // !UTIL
