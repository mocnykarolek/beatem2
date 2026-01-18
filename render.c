#include "render.h"
#include "logic.h"

int color(GameSession *gameSession, Colors color) {

    switch (color) {
    case RED:
        return SDL_MapRGB(gameSession->screen->format, 0xFF, 0x00, 0x00);
        break;
    case BLACK:
        return SDL_MapRGB(gameSession->screen->format, 0x00, 0x00, 0x00);
        break;
    case GREEN:
        return SDL_MapRGB(gameSession->screen->format, 0x00, 0xFF, 0x00);
        break;
    case BLUE:
        return SDL_MapRGB(gameSession->screen->format, 0x11, 0x11, 0xCC);
        break;
    case SKYBLUE:
        return SDL_MapRGB(gameSession->screen->format, 0x87, 0xce, 0xeb);
        break;
    case TITLEGRAY:
        return SDL_MapRGB(gameSession->screen->format, 0x82, 0x83, 0x85);
        break;
    case YELLOW:
        return SDL_MapRGB(gameSession->screen->format, 0xff, 0xff, 0x00);
        break;
    case ORANGE:
        return SDL_MapRGB(gameSession->screen->format, 0xff, 0x80, 0x00);
        break;
    default:
        return 0;
    }
}

int initialize_player(Player *player, GameSession *gameSession, double x,
                      double y) {

    player->surface = SDL_LoadBMP("./textures/mainPlayerSprite.bmp");
    if (player->surface == NULL) {
        printf("SDL_LoadBMP(stickman.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(gameSession->screen);
        SDL_FreeSurface(gameSession->charset);
        SDL_DestroyTexture(gameSession->scrtex);
        SDL_DestroyWindow(gameSession->window);
        SDL_DestroyRenderer(gameSession->renderer);
        SDL_Quit();
        return 1;
    }

    SDL_SetColorKey(player->surface, SDL_TRUE,
                    SDL_MapRGB(player->surface->format, 255, 0, 255));

    player->texture =
        SDL_CreateTextureFromSurface(gameSession->renderer, player->surface);
    if (player->texture == NULL) {
        printf("player_texture creation error: %s\n", SDL_GetError());
        SDL_FreeSurface(gameSession->screen);
        SDL_FreeSurface(gameSession->charset);
        SDL_DestroyTexture(gameSession->scrtex);
        SDL_DestroyWindow(gameSession->window);
        SDL_DestroyRenderer(gameSession->renderer);
        SDL_Quit();
        return 1;
    }

    
    
    player->frameHeight = 50;
    player->frameWidth = 21;

    player->speed = PLAYER_SPEED;
    player->rect.x = x;
    player->rect.y = y;
    player->rect.w = player->frameWidth;
    player->rect.h = player->frameHeight;
    player->position.x = x;
    player->position.y = y + player->rect.h;


    player->multiplier = 1;
    player->lastHitTime = 0;
    player->max_time_between_hits_s = 1.3;

    player->currentFrame = 0;
    player->currentRow = 0;
    player->animTimer = 0;
    player->timePerFrame = 0.15; // czas trawnia 1 klatki animacji
    player->totalFrames = 4;    

    // player->rect.w = 100;
    // player->rect.h = 100;
    player->remainingHp = PLAYER_MAX_HEALTH;

    player->direction.x = 0;
    player->direction.y = 0;
    player->isJumping = RESET_ACTION;
    player->jumpingTime_s = PLAYER_JUMP_TIME;
    player->RemainingJumpTime_s = player->jumpingTime_s;

    player->isAction = RESET_ACTION;
    player->actions.type = NOACTION;
    player->actions.RemainingTime_s = 0;

    player->color.lighAttack = color(gameSession, GREEN);
    player->color.heavyAttack = color(gameSession, RED);
    player->lastHeading = 1;
    player->scale = 1.0f + player->position.y /
                               ((float)WORLD_MAX_Y - (float)WORLD_MIN_Y) * 2.0f;
    player->comboType.comboType = NO_COMBO;
    for (int i = 0; i < 4; i++) {
        player->recentActions[i] = "...";
    }

    return 0;
}

void changePlayersColor(Player *p, GameSession *gs, GameState *gms) {
    // old v
    if (p->actions.type == LIGHT_ATTACK) {
        SDL_SetTextureColorMod(p->texture, 0, 255, 0);

        if (p->lastHeading == LEFT)
            DrawRectangle(gs->screen,
                          p->position.x +
                              (p->rect.w + 30) * p->lastHeading * 2 -
                              *gms->camera_offset,
                          p->position.y + p->rect.h / 4, 100, 30,
                          color(gs, GREEN), color(gs, GREEN));
        if (p->lastHeading == RIGHT) {
            if (p->position.x + (p->rect.w + 30) - *gms->camera_offset +
                    PLAYER_LIGHT_REACH_px >
                SCREEN_WIDTH) {
                DrawRectangle(gs->screen,
                              p->position.x + (p->rect.w + 30) -
                                  *gms->camera_offset,
                              p->position.y + p->rect.h / 4,
                              SCREEN_WIDTH - p->position.x + (p->rect.w + 30) +
                                  *gms->camera_offset + PLAYER_LIGHT_REACH_px,
                              30, color(gs, GREEN), color(gs, GREEN));
            } else
                DrawRectangle(gs->screen,
                              p->position.x + (p->rect.w + 30) -
                                  *gms->camera_offset,
                              p->position.y + p->rect.h / 4, 100, 30,
                              color(gs, GREEN), color(gs, GREEN));
        }

    } else if (p->actions.type == HEAVY_ATTACK) {
        SDL_SetTextureColorMod(p->texture, 255, 0, 0);
        if (p->lastHeading == LEFT)
            DrawRectangle(gs->screen,
                          p->position.x +
                              (p->rect.w + 30) * p->lastHeading * 4 -
                              *gms->camera_offset,
                          p->position.y + p->rect.h / 4, PLAYER_HEAVY_REACH_px,
                          30, color(gs, RED), color(gs, RED));
        if (p->lastHeading == RIGHT) {
            if (p->position.x + (p->rect.w + 30) - *gms->camera_offset +
                    PLAYER_LIGHT_REACH_px >
                WORLD_MAX_X) {
                DrawRectangle(gs->screen,
                              p->position.x + (p->rect.w + 30) -
                                  *gms->camera_offset,
                              p->position.y + p->rect.h / 4,
                              WORLD_MAX_X - p->position.x + (p->rect.w + 30) +
                                  *gms->camera_offset + PLAYER_HEAVY_REACH_px,
                              30, color(gs, GREEN), color(gs, RED));
            } else
                DrawRectangle(
                    gs->screen,
                    p->position.x + (p->rect.w + 30) - *gms->camera_offset,
                    p->position.y + p->rect.h / 4, PLAYER_HEAVY_REACH_px, 30,
                    color(gs, RED), color(gs, RED));
        }
    } else if (p->comboType.comboType == SUPERCOMBO) {
        SDL_SetTextureColorMod(p->texture, 0, 0, 255);

    } else if (p->actions.type == NOACTION ||
               p->comboType.comboType == NO_COMBO) {
        SDL_SetTextureColorMod(p->texture, 255, 255, 255);
    }
}

void DrawPlayerAnimation(GameSession *gs, Player *p, int cameraOffset) {

    SDL_Rect srcRect;  // Co wycinamy z obrazka
    SDL_Rect destRect; // Gdzie rysujemy na ekranie

    // --- MATEMATYKA SPRITESHEETA ---
    srcRect.x = p->currentFrame * p->frameWidth; // Przesunięcie w prawo
    srcRect.y = p->currentRow * p->frameHeight;  // Przesunięcie w dół
    srcRect.w = p->frameWidth;
    srcRect.h = p->frameHeight;

    // Pozycja na ekranie (z uwzględnieniem kamery i skali)
    destRect.x = (int)p->position.x - cameraOffset;
    destRect.y = (int)p->position.y - (p->rect.h * p->scale) +
                 p->rect.h; // Korekta na stopy
    destRect.w = p->frameWidth * p->scale;
    destRect.h = p->frameHeight * p->scale;

    // Obsługa obracania (lewo/prawo)
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (p->lastHeading == LEFT) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    // Rysujemy wycinek!
    SDL_RenderCopyEx(gs->renderer, p->texture,
                     &srcRect, // <-- Kluczowa zmiana: nie NULL, a srcRect
                     &destRect, 0, NULL, flip);
}

void DrawEntityAnimation(GameSession *gs, Entity *entities, int cameraOffset) {

    for (int i = 0; i < NUMOFOBSTACLES; i++) {
        Entity *e = &entities[i];
        // printf("cords X: %d Y: %d \n", e->rect.x, e->rect.y);
        // printf("fsdfsd\n");
        if (e->isInitialized) {
            SDL_Rect srcRect;  // Co wycinamy z obrazka
            SDL_Rect destRect; // Gdzie rysujemy na ekranie

            // --- MATEMATYKA SPRITESHEETA ---
            srcRect.x = e->currentFrame * e->frameWidth; // Przesunięcie w prawo
            srcRect.y = e->currentRow * e->frameHeight;  // Przesunięcie w dół
            srcRect.w = e->frameWidth;
            srcRect.h = e->frameHeight;

            // Pozycja na ekranie (z uwzględnieniem kamery i skali)
            destRect.x = (int)e->rect.x - cameraOffset;
            destRect.y = (int)e->rect.y; // Korekta na stopy
            destRect.w = e->frameWidth;
            destRect.h = e->frameHeight;

            // Obsługa obracania (lewo/prawo)
            

            // Rysujemy wycinek!
            SDL_RenderCopyEx(
                gs->renderer, e->texture,
                &srcRect, // <-- Kluczowa zmiana: nie NULL, a srcRect
                &destRect, 0, NULL, 0);
        }
    }
}

void PlayerAttackState(Player *p, GameSession *gs, GameState *gms) {

    // if (p->actions.type == LIGHT_ATTACK) {
    //     int colorP = color(gs, GREEN);
    //     SDL_SetTextureColorMod(p->texture, 0, 255, 0);

    //     if (p->lastHeading == RIGHT) {
    //         double AttackScreenPosition =
    //             p->position.x + p->rect.w + 30 - *gms->camera_offset;
    //         double spaceAvailable = SCREEN_WIDTH - AttackScreenPosition;

    //         //

    //         if (spaceAvailable < PLAYER_LIGHT_REACH_px) {
    //             DrawRectangle(gs->screen, AttackScreenPosition,
    //                           p->position.y - (p->rect.h / 2),
    //                           spaceAvailable, 30, colorP, colorP);
    //         } else
    //             DrawRectangle(gs->screen, AttackScreenPosition,
    //                           p->position.y - (p->rect.h / 2),
    //                           PLAYER_LIGHT_REACH_px, 30, colorP, colorP);

    //     } else if (p->lastHeading == LEFT) {
    //         double AttackScreenPosition =
    //             p->position.x - PLAYER_LIGHT_REACH_px - *gms->camera_offset;
    //         double spaceAvailable = p->position.x;

    //         // *gms->debug_exit = 1;
    //         if (AttackScreenPosition < 0) {
    //             DrawRectangle(gs->screen, 0, p->position.y - (p->rect.h / 2),
    //                           spaceAvailable, 30, colorP, colorP);
    //             printf("%lf . %lf\n", AttackScreenPosition, spaceAvailable);
    //         } else
    //             DrawRectangle(gs->screen, AttackScreenPosition,
    //                           p->position.y - (p->rect.h / 2),
    //                           PLAYER_LIGHT_REACH_px, 30, colorP, colorP);
    //     }

    // } else if (p->actions.type == HEAVY_ATTACK) {
    //     int colorP = color(gs, RED);
    //     SDL_SetTextureColorMod(p->texture, 255, 0, 0);

    //     if (p->lastHeading == RIGHT) {
    //         double AttackScreenPosition =
    //             p->position.x + p->rect.w + 30 - *gms->camera_offset;
    //         double spaceAvailable = SCREEN_WIDTH - AttackScreenPosition;

    //         //

    //         if (spaceAvailable < PLAYER_HEAVY_REACH_px) {
    //             DrawRectangle(gs->screen, AttackScreenPosition,
    //                           p->position.y - (p->rect.h / 2),
    //                           spaceAvailable, 30, colorP, colorP);
    //         } else
    //             DrawRectangle(gs->screen, AttackScreenPosition,
    //                           p->position.y - (p->rect.h / 2),
    //                           PLAYER_HEAVY_REACH_px, 30, colorP, colorP);

    //     } else if (p->lastHeading == LEFT) {
    //         double AttackScreenPosition =
    //             p->position.x - PLAYER_HEAVY_REACH_px - *gms->camera_offset;
    //         double spaceAvailable = p->position.x;

    //         // *gms->debug_exit = 1;
    //         if (AttackScreenPosition < 0) {
    //             DrawRectangle(gs->screen, 0, p->position.y - (p->rect.h / 2),
    //                           spaceAvailable, 30, colorP, colorP);
    //             printf("%lf . %lf\n", AttackScreenPosition, spaceAvailable);
    //         } else
    //             DrawRectangle(gs->screen, AttackScreenPosition,
    //                           p->position.y - (p->rect.h / 2),
    //                           PLAYER_HEAVY_REACH_px, 30, colorP, colorP);
    //     }

    // }
    // else if (p->comboType.comboType == SUPERCOMBO) {
    //     SDL_SetTextureColorMod(p->texture, 0, 0, 255);
    // } else if (p->actions.type == NOACTION || p->comboType.comboType ==
    // NO_COMBO) {
    //     SDL_SetTextureColorMod(p->texture, 255, 255, 255);
    // }

    if (p->actions.type == LIGHT_ATTACK) {
        int colorP = color(gs, GREEN);
        SDL_SetTextureColorMod(p->texture, 0, 255, 0);

        if (p->lastHeading == RIGHT) {
            double AttackScreenPosition =
                p->position.x + p->rect.w + 30 - *gms->camera_offset;
            double spaceAvailable = SCREEN_WIDTH - AttackScreenPosition;

            //

            if (spaceAvailable < PLAYER_LIGHT_REACH_px) {
                DrawRectangle(gs->screen, AttackScreenPosition,
                              p->position.y - (p->rect.h / 2), spaceAvailable,
                              30, colorP, colorP);
            } else
                DrawRectangle(gs->screen, AttackScreenPosition,
                              p->position.y - (p->rect.h / 2),
                              PLAYER_LIGHT_REACH_px, 30, colorP, colorP);

        } else if (p->lastHeading == LEFT) {
            double AttackScreenPosition =
                p->position.x - PLAYER_LIGHT_REACH_px - *gms->camera_offset;
            double spaceAvailable = p->position.x;

            // *gms->debug_exit = 1;
            if (AttackScreenPosition < 0) {
                DrawRectangle(gs->screen, 0, p->position.y - (p->rect.h / 2),
                              spaceAvailable, 30, colorP, colorP);
                // printf("%lf . %lf\n", AttackScreenPosition, spaceAvailable);
            } else
                DrawRectangle(gs->screen, AttackScreenPosition,
                              p->position.y - (p->rect.h / 2),
                              PLAYER_LIGHT_REACH_px, 30, colorP, colorP);
        }

    } else if (p->actions.type == HEAVY_ATTACK) {
        int colorP = color(gs, RED);
        SDL_SetTextureColorMod(p->texture, 255, 0, 0);

        if (p->lastHeading == RIGHT) {
            double AttackScreenPosition =
                p->position.x + p->rect.w + 30 - *gms->camera_offset;
            double spaceAvailable = SCREEN_WIDTH - AttackScreenPosition;

            //

            if (spaceAvailable < PLAYER_HEAVY_REACH_px) {
                DrawRectangle(gs->screen, AttackScreenPosition,
                              p->position.y - (p->rect.h / 2), spaceAvailable,
                              30, colorP, colorP);
            } else
                DrawRectangle(gs->screen, AttackScreenPosition,
                              p->position.y - (p->rect.h / 2),
                              PLAYER_HEAVY_REACH_px, 30, colorP, colorP);

        } else if (p->lastHeading == LEFT) {
            double AttackScreenPosition =
                p->position.x - PLAYER_HEAVY_REACH_px - *gms->camera_offset;
            double spaceAvailable = p->position.x;

            // *gms->debug_exit = 1;
            if (AttackScreenPosition < 0) {
                DrawRectangle(gs->screen, 0, p->position.y - (p->rect.h / 2),
                              spaceAvailable, 30, colorP, colorP);
                // printf("%lf . %lf\n", AttackScreenPosition, spaceAvailable);
            } else
                DrawRectangle(gs->screen, AttackScreenPosition,
                              p->position.y - (p->rect.h / 2),
                              PLAYER_HEAVY_REACH_px, 30, colorP, colorP);
        }

    } else if (p->comboType.comboType == SUPERCOMBO) {
        SDL_SetTextureColorMod(p->texture, 0, 0, 255);
    } else if (p->actions.type == NOACTION ||
               p->comboType.comboType == NO_COMBO) {
        SDL_SetTextureColorMod(p->texture, 255, 255, 255);
    }
}

void DrawPlayerAttack(Player *p, GameSession *gs, GameState *gms) {

    SDL_Rect attackBox = getAttackBox(p);
    int attackColor;

    if (p->actions.type == LIGHT_ATTACK) { //
        attackColor = color(gs, GREEN);
        SDL_SetTextureColorMod(p->texture, 0, 255, 0);
    } else if (p->actions.type == HEAVY_ATTACK) { //
        attackColor = color(gs, RED);             //
        SDL_SetTextureColorMod(p->texture, 255, 0, 0);

    } else if (p->comboType.comboType ==
               SUPERCOMBO) { // Zakładam, że dodałeś to do enum
        // SZPONT FIX: Wcześniej nie ustawiałeś tu koloru pudełka!
        attackColor = color(gs, BLUE);
        SDL_SetTextureColorMod(p->texture, 0, 0, 255);
    }else if (p->comboType.comboType ==
               LIGHT_COMBO) { // Zakładam, że dodałeś to do enum
        // SZPONT FIX: Wcześniej nie ustawiałeś tu koloru pudełka!
        attackColor = color(gs, YELLOW);
        SDL_SetTextureColorMod(p->texture, 255, 255, 0);
    } else if (p->comboType.comboType ==
               HEAVY_COMBO) { // Zakładam, że dodałeś to do enum
        // SZPONT FIX: Wcześniej nie ustawiałeś tu koloru pudełka!
        attackColor = color(gs, ORANGE);
        SDL_SetTextureColorMod(p->texture, 255, 128, 0);
    }
    else {
        // To obsługuje NOACTION i NO_COMBO
        SDL_SetTextureColorMod(p->texture, 255, 255,
                               255); // Reset koloru gracza

        // Jeśli nie ma ataku, przerywamy funkcję, żeby nie rysować pustego
        // prostokąta
        return;
    }

    // 4. Rysowanie z konwersją: World Space -> Screen Space
    // Odejmujemy *gms->camera_offset TYLKO tutaj
    DrawRectangle(gs->screen, attackBox.x - *gms->camera_offset, attackBox.y,
                  attackBox.w, attackBox.h, attackColor, attackColor);
}

int loadCharset(GameSession *gameSession) {
    gameSession->charset = SDL_LoadBMP("./bitmaps/cs8x8.bmp");
    if (gameSession->charset == NULL) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(gameSession->screen);
        SDL_FreeSurface(gameSession->charset);
        SDL_DestroyTexture(gameSession->scrtex);
        SDL_DestroyWindow(gameSession->window);
        SDL_DestroyRenderer(gameSession->renderer);
        return 1;
    }

    return 0;
}



void DrawPlayerObstacleHitbox(Player *p, int obsH, GameState *gms) {

    SDL_Rect HitboxRect = playerObstaclesHitbox(p, obsH);
    // printf("%d %d %d %d\n", HitboxRect.x, HitboxRect.y, HitboxRect.w,
    //        HitboxRect.h);

    DrawFrame(gms->gs->screen, HitboxRect.x - *gms->camera_offset, HitboxRect.y,
              HitboxRect.h, HitboxRect.w, color(gms->gs, BLACK));
}

void RenderEntities(Entity *entities, int numOfEntites, GameSession *gs,
                    int cameraOffset) {

    for (int i = 0; i < numOfEntites; i++) {
        Entity *e = &entities[i];
        // printf("cords X: %d Y: %d \n", e->rect.x, e->rect.y);
        // printf("fsdfsd\n");
        if (e->isInitialized) {
            int scrX = e->rect.x - cameraOffset;
            DrawRectangle(gs->screen, scrX, e->rect.y, e->rect.w, e->rect.h,
                          color(gs, BLACK), color(gs, RED));
        }
    }
}

int initialization(GameSession *gameSession) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 0;
    }

    gameSession->window =
        SDL_CreateWindow("basic", 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gameSession->renderer = SDL_CreateRenderer(gameSession->window, 1, 0);
    if (gameSession->renderer == NULL) {
        SDL_Quit();
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 0;
    };

    gameSession->screen =
        SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                             0x0000FF00, 0x000000FF, 0xFF000000);

    gameSession->scrtex = SDL_CreateTexture(
        gameSession->renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(gameSession->renderer, SCREEN_WIDTH,
                             SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(gameSession->renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(gameSession->window, "example_window");

    return 1;
}

// SDL_Texture *texture(){

//     SDL_CreateTextureFromSurface(renderer, player_surface);

// }