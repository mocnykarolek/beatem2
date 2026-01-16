#pragma once
#include  "main.h"




void playerMovement(GameSession* gs, Player* p, double delta);
void mainLoop(GameState *gms);
SDL_Rect getAttackBox(Player *p);

void deallocation(GameState* gms);