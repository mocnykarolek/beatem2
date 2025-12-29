#pragma once
#include  "main.h"

int initialization(GameState* gamestate);


void playerMovement(GameState* gs, Player* p, double delta);
void mainLoop(GameState* gamestate);


void deallocation(GameState* gamestate);