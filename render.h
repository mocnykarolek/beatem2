#pragma once
#include  "main.h"


int color(GameSession* gameSession,Colors color);
void changePlayersColor(Player*p, GameSession *gs, GameState *gms);
int initialize_player(Player* player, GameSession* gameSession,double x, double y);
int loadCharset(GameSession* gameSession);
int initialization(GameSession* gameSession);
void PlayerAttackState(Player*p, GameSession *gs, GameState *gms);
void RenderEntities(Entity* entities, int numOfEntites, GameSession* gs, int cameraOffset);
void DrawPlayerAttack(Player *p, GameSession *gs, GameState *gms);
void DrawPlayerObstacleHitbox(Player *p, int obsH, GameState* gms);
void DrawPlayerAnimation(GameSession *gs, Player *p, int cameraOffset);

void DrawEntityAnimation(GameSession *gs, Entity *entities, int cameraOffset);
void showMultiplier(GameSession* gs, Player* p);