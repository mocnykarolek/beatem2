#pragma once
#include  "main.h"




void playerMovement(GameSession* gs, Player* p, double delta);
void mainLoop(GameState *gms);
SDL_Rect getAttackBox(Player *p);
SDL_Rect playerObstaclesHitbox(Player *p, int obsH);

void deallocation(GameState* gms);