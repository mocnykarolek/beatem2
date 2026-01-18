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