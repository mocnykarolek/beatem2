#pragma once
#include  "main.h"


int color(GameSession* gameSession,Colors color);

int initialize_player(Player* player, GameSession* gameSession,double x, double y);
int loadCharset(GameSession* gameSession);