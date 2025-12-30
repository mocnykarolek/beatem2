#pragma once
#include  "main.h"

int initialization(GameSession* gameSession);


void playerMovement(GameSession* gs, Player* p, double delta);
void mainLoop(GameSession* gamestate);


void deallocation(GameSession* gamestate, Player* player);