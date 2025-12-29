#pragma once
#include  "main.h"


int color(GameState* gamestate,Colors color);

int initialize_player(Player* player, GameState* gamestate,double x, double y);
int loadCharset(GameState* gamestate);