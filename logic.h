#pragma once
#include  "main.h"




void playerMovement(GameSession* gs, Player* p, double delta);
void mainLoop(GameState *gms);
SDL_Rect getAttackBox(Player *p);
SDL_Rect playerObstaclesHitbox(Player *p, int obsH);
int checkCollision(SDL_Rect rect1, SDL_Rect rect2);
void UpdatePlayerAnimation(Player *p, double delta);
void deallocation(GameState* gms);
Entity *createEntities(int numOfEntities, GameSession* gameSession);
void playerInits(Player*player, double x, double y, GameSession* gameSession);
void EntitiesRandomPosition(Entity *entities, int entityCount);
void gameLoop(GameState *gms);
void timerOperations(double *delta, int *t2, int *t1, double *worldTime,
                     double *timer_s, double *fpsTimer, Player* player, double* fps, int*frames);
void userInputHandling(Player *player, GameSession *gameSession, GameState *gms,
                       int *quit, int *flip, double *timer_s, int *debug);
void updateCamera(GameState* gms);
void comboServing(Player*player, double* delta, GameSession* gameSession, GameState* gms);