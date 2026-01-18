
#include "logic.h"

#include "render.h"
// [0,1,2,3]

char *checkCombo(Player *p) {
    char **comboBuffer = p->recentActions;
    int test = strcmp(comboBuffer[0], comboBuffer[1]) == 0 &&
               strcmp(comboBuffer[2], "HEAVY") == 0 &&
               strcmp(comboBuffer[0], "LIGHT") == 0;
    printf("%d\n", test);
    if (strcmp(comboBuffer[0], comboBuffer[1]) == 0 &&
        strcmp(comboBuffer[2], "HEAVY") == 0 &&
        strcmp(comboBuffer[0], "LIGHT") == 0) {
        // printf("dDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
        return "SUPERCOMBO";

    } else if (strcmp(comboBuffer[1], comboBuffer[2]) == 0 &&
               strcmp(comboBuffer[3], "HEAVY") == 0 &&
               strcmp(comboBuffer[1], "LIGHT") == 0) {
        // printf("dDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");

        return "SUPERCOMBO";
    } else if (strcmp(comboBuffer[0], comboBuffer[1]) == 0 &&
               strcmp(comboBuffer[1], comboBuffer[2]) == 0 &&
               strcmp(comboBuffer[0], NOACTIONCHAR) != 0) {
        return comboBuffer[0];
    }

    else if (strcmp(comboBuffer[1], comboBuffer[2]) == 0 &&
             strcmp(comboBuffer[2], comboBuffer[3]) == 0 &&
             strcmp(comboBuffer[1], NOACTIONCHAR) != 0) {
        return comboBuffer[1];
    }

    else {
        return NOACTIONCHAR;
    }
}

void updatePlayerMes(Player *p) {

    int base_w = p->surface->w;
    int base_h = p->surface->h;

    p->rect.w = (int)(base_w * p->scale);
    p->rect.h = (int)(base_h * p->scale);
}

void comboHandler(Player *p, double delta, GameSession *gs, GameState *gms) {

    if (p->comboType.comboType != NO_COMBO) {
        if (p->comboType.comboTimeRemaining <= 0) {
            p->comboType.comboType = NO_COMBO;
        }

        switch (p->comboType.comboType) {
        case LEFT_DASH:
            p->position.x += LEFT * p->speed * 2 * delta;

            break;
        case RIGHT_DASH:
            p->position.x += RIGHT * p->speed * 2 * delta;

            break;
        case UP_DASH:
            p->position.y += UP * p->speed * 2 * delta;

            break;
        case DOWN_DASH:
            p->position.y += DOWN * p->speed * 2 * delta;

            break;
        case SUPERCOMBO:
            printf("dDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
                   "DDDDDDDDDDDDDD\n");
            DrawPlayerAttack(p, gs, gms);

            break;

        default:
            break;
        }
    }
}
void UpdateEntityAnimation(Entity *e, double delta) {

    for (int i = 0; i < NUMOFOBSTACLES; i++) {
        if (e[i].isAttacked) {
            e[i].currentRow = 1;
            e[i].timePerFrame = 0.16;
        } else {
            e[i].currentRow = 0;
            e[i].timePerFrame = 0.2;
        }
        e[i].animTimer += delta;

        if (e[i].animTimer >= e[i].timePerFrame) {

            e[i].animTimer = 0;
            e[i].currentFrame++;
            if (e[i].currentFrame >= e[i].totalFrames) {
                e[i].currentFrame = 0;
            }
        }
    }
}

void UpdatePlayerAnimation(Player *p, double delta) {

    // 1. Wybór rzędu (Jaka to czynność?)
    if (p->actions.type == LIGHT_ATTACK || p->actions.type == HEAVY_ATTACK) {
        p->currentRow = 2; // Rząd 3 (index 2) w twoim pliku to atak
        // Opcjonalnie: przyspiesz animację ataku
        p->timePerFrame = 0.1;
    } else if (p->direction.x != 0 || p->direction.y != 0) {
        p->currentRow = 1; // Rząd 2 (index 1) to chodzenie
        p->timePerFrame = 0.15;
    } else {
        p->currentRow = 0;     // Rząd 1 (index 0) to stanie (idle)
        p->timePerFrame = 0.2; // Wolniejsze "oddychanie"
    }

    // 2. Odliczanie czasu
    p->animTimer += delta;

    // 3. Zmiana klatki
    if (p->animTimer >= p->timePerFrame) {
        p->animTimer = 0;
        p->currentFrame++;

        // Zapętlanie (1, 2, 3, 4 -> 1...)
        if (p->currentFrame >= p->totalFrames) {
            p->currentFrame = 0;

            // Fix dla ataku: Jeśli atak się skończył, można tu wyłączyć
            // isAction ale na razie zostawmy proste pętlę.
        }
    }
}

void lightAttack(Player *p, double delta, GameSession *gs, GameState *gms) {
    if (p->actions.RemainingTime_s == 0) {
        p->actions.RemainingTime_s = PLAYERS_LIGHT_ACTION_DURATION_S;
    }

    if (p->actions.RemainingTime_s > 0) {

        DrawPlayerAttack(p, gs, gms);

        p->actions.RemainingTime_s -= delta;
    }

    if (p->actions.RemainingTime_s <= 0 && p->isAction) {
        p->isAction = NOACTION;

        p->actions.type = NOACTION;
        p->actions.RemainingTime_s = PLAYERS_LIGHT_ACTION_DURATION_S;
    }
}

void HeavyAttack(Player *p, double delta, GameSession *gs, GameState *gms) {
    if (p->actions.RemainingTime_s == 0) {
        p->actions.RemainingTime_s = PLAYERS_HEAVY_ACTION_DURATION_S;
    }

    if (p->actions.RemainingTime_s > 0) {

        DrawPlayerAttack(p, gs, gms);

        p->actions.RemainingTime_s -= delta;
    }

    if (p->actions.RemainingTime_s <= 0 && p->isAction) {
        p->isAction = NOACTION;

        p->actions.type = NOACTION;
        p->actions.RemainingTime_s = PLAYERS_HEAVY_ACTION_DURATION_S;
    }
}

void actionsHandling(Player *p, double delta, GameSession *gs, GameState *gms) {

    if (p->actions.type == LIGHT_ATTACK) {
        p->isAction = ACTION;
        lightAttack(p, delta, gs, gms);

    } else if (p->actions.type == HEAVY_ATTACK) {
        p->isAction = ACTION;
        HeavyAttack(p, delta, gs, gms);
    } else if (p->actions.type == NOACTION)
        DrawPlayerAttack(p, gs, gms);
}

SDL_Rect
getAttackBox(Player *p) { // Usunąłem gms, bo logika nie powinna znać kamery
    SDL_Rect r = {0, 0, 0, 0}; // 1. Inicjalizacja zerami (bezpiecznik)

    // Jeśli nie ma ataku, zwracamy pusty prostokąt
    if (p->actions.type != LIGHT_ATTACK && p->actions.type != HEAVY_ATTACK) {
        return r;
    }

    int reach = 0;
    int offset_x = 30; // Odstęp od krawędzi gracza

    // 2. Ustalenie parametrów zależnie od ataku (DRY)
    if (p->actions.type == LIGHT_ATTACK) {
        reach = PLAYER_LIGHT_REACH_px;
    } else {
        reach = PLAYER_HEAVY_REACH_px;
    }

    r.h = 30; // Wysokość hitboxa
    r.y =
        (int)p->position.y - (p->rect.h / 2); // Wysokość: np. w połowie postaci

    // 3. Obliczenia w WORLD SPACE (bez kamery!)
    if (p->lastHeading == RIGHT) {
        // Atak w prawo: pozycja gracza + szerokość gracza + odstęp
        r.x = (int)p->position.x + p->rect.w + offset_x;
        r.w = reach;
    } else { // LEFT
        // Atak w lewo: pozycja gracza - zasięg
        r.x = (int)p->position.x - reach;
        // Tutaj opcjonalnie mały fix, jeśli chcesz by atak wychodził idealnie z
        // pleców czy przodu Ale generalnie:
        r.w = reach;
    }

    return r;
}

void checkAttack(Player *p, Entity *e, GameState *gms) {

    for (int i = 0; i < NUMOFOBSTACLES; i++) {
        if (checkCollision(e[i].rect, getAttackBox(p))) {
            gms->players_points++;
            return;
        }
    }
}

SDL_Rect playerObstaclesHitbox(Player *p, int obsH) {
    SDL_Rect playerHitbox;
    playerHitbox.x = p->rect.x;
    playerHitbox.y = p->rect.y + p->rect.h - obsH;
    playerHitbox.w = p->rect.w;
    playerHitbox.h = obsH;

    return playerHitbox;
}

void addAction(Player *p, char *action) {

    for (int i = 3; i > 0; i--) {
        p->recentActions[i] = p->recentActions[i - 1];
    }
    p->recentActions[0] = action;
}

void restartGame(GameState *gms, GameSession *g) {

    if (initialize_player(gms->p, g, (double)(WORLD_WIDTH / 2),
                          (double)(WORLD_HEIGHT / 2))) {
        return;
    }
    *gms->worldTime = 0;
    *gms->camera_offset = 0;
}

Entity *createEntities(int numOfEntities, GameSession* gameSession) {

    Entity *entities = malloc(numOfEntities * sizeof(Entity));
    for (int i = 0; i < numOfEntities; i++) {
        entities[i].rect.w = 30;
        entities[i].rect.h = 30;
        entities[i].isInitialized = false;
        entities[i].health = 100;

        entities[i].surface = SDL_LoadBMP("./textures/entity.bmp");
        if (entities[i].surface == NULL) {

            SDL_Quit();
            return 1;
        }

        SDL_SetColorKey(entities[i].surface, SDL_TRUE,
                        SDL_MapRGB(entities[i].surface->format, 255, 0, 255));

        entities[i].texture = SDL_CreateTextureFromSurface(gameSession->renderer,
                                                       entities[i].surface);
        if (entities[i].texture == NULL) {

            SDL_Quit();
            return 1;
        }


        entities[i].frameHeight = 31;
        entities[i].frameWidth = 31;
    }

    return entities;
}

int checkCollision(SDL_Rect rect1, SDL_Rect rect2) {

    if (rect1.x >= rect2.x + rect2.w) {
        return 0;
    }
    if (rect1.x + rect1.w <= rect2.x) {
        return 0;
    }
    if (rect1.y >= rect2.y + rect2.h) {
        return 0;
    }
    if (rect1.y + rect1.h <= rect2.y) {
        return 0;
    }

    return 1;
}

void EntitiesRandomPosition(Entity *entities, int entityCount) {

    for (int i = 0; i < entityCount; i++) {

        int collisionDetected;

        int attemps = 0;
        SDL_Rect testRect = entities[i].rect;
        do {
            collisionDetected = 0;
            int min_x = WORLD_MIN_X;
            int max_x = WORLD_MAX_X - testRect.w;
            int min_y = WORLD_MIN_Y;
            int max_y = WORLD_MAX_Y - testRect.h;

            testRect.x = min_x + rand() % (max_x - min_x + 1);
            testRect.y = min_y + rand() % (max_y - min_y + 1);

            for (int j = 0; j < i; j++) {
                if (checkCollision(testRect, entities[j].rect)) {
                    collisionDetected = 1;
                    break;
                }
            }

            attemps++;

            if (attemps > MAX_ATTEMPTS) {
                entities[i].isInitialized = false;
                break;
            } else
                entities[i].isInitialized = true;

        } while (collisionDetected);

        entities[i].rect.x = testRect.x;
        entities[i].rect.y = testRect.y;
    }
}

void handleJumping(GameState *gms, double delta) {

    if (gms->p->isJumping &&
        gms->p->RemainingJumpTime_s > gms->p->jumpingTime_s / 2) {

        gms->p->RemainingJumpTime_s -= delta;
        gms->p->direction.y = UP;
    } else if (gms->p->isJumping && gms->p->RemainingJumpTime_s > 0) {
        gms->p->RemainingJumpTime_s -= delta;
        gms->p->direction.y = DOWN;
    }

    if (gms->p->RemainingJumpTime_s <= 0) {
        gms->p->isJumping = 0;
        gms->p->RemainingJumpTime_s = PLAYER_JUMP_TIME;
        gms->p->direction.y = RESET_ACTION;
    }

    // printf("Remaining jump timer: %lf\n", gms->p->RemainingJumpTime_s);
}

Combo InitCombo(char *comboChar) {
    int Combotype;
    if (strcmp(comboChar, "LEFT") == 0)
        Combotype = LEFT_DASH;
    else if (strcmp(comboChar, "RIGHT") == 0)
        Combotype = RIGHT_DASH;
    else if (strcmp(comboChar, "UP") == 0)
        Combotype = UP_DASH;
    else if (strcmp(comboChar, "DOWN") == 0)
        Combotype = DOWN_DASH;
    else if (strcmp(comboChar, "LIGHT") == 0)
        Combotype = LIGHT_COMBO;
    else if (strcmp(comboChar, "HEAVY") == 0)
        Combotype = HEAVY_COMBO;
    else if (strcmp(comboChar, "SUPERCOMBO") == 0)
        Combotype = SUPERCOMBO;

    else
        Combotype = NO_COMBO;

    Combo current_combo;
    switch (Combotype) {
    case LEFT_DASH:
        current_combo.comboType = LEFT_DASH;
        current_combo.comboInitialTime = DASH_TIME;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case RIGHT_DASH:
        current_combo.comboType = RIGHT_DASH;
        current_combo.comboInitialTime = DASH_TIME;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case UP_DASH:
        current_combo.comboType = UP_DASH;
        current_combo.comboInitialTime = DASH_TIME;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case DOWN_DASH:
        current_combo.comboType = DOWN_DASH;
        current_combo.comboInitialTime = DASH_TIME;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case LIGHT_COMBO:
        current_combo.comboType = LIGHT_COMBO;
        current_combo.comboInitialTime = 2;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case HEAVY_COMBO:
        current_combo.comboType = HEAVY_COMBO;
        current_combo.comboInitialTime = 1;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case SUPERCOMBO:
        current_combo.comboType = SUPERCOMBO;
        current_combo.comboInitialTime = 2;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;

    default:
        current_combo.comboType = NO_COMBO;
        current_combo.comboInitialTime = 0;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    }

    return current_combo;
}

void playerMovement(GameSession *gs, Player *p, double delta) {

    if (p->position.y < WORLD_MIN_Y) {
        p->position.y = WORLD_MIN_Y;
    }

    if (p->position.y + p->rect.h > WORLD_MAX_Y) {
        p->position.y = WORLD_MAX_Y - p->rect.h;
    }

    if (p->position.x < WORLD_MIN_X) {
        p->position.x = WORLD_MIN_X;
    }

    if (p->position.x > WORLD_MAX_X) {
        p->position.x = WORLD_MAX_X;
    }

    p->position.x += p->direction.x * p->speed * delta;

    p->position.y += p->direction.y * p->speed * delta;

    p->rect.x = (int)p->position.x;
    p->rect.y = (int)p->position.y;

    p->scale =
        1.0f + p->position.y / ((float)WORLD_MAX_Y - (float)WORLD_MIN_Y) * 2.0f;

    // updatePlayerMes(p);
}

void mainLoop(GameState *gms) {
    GameSession *gameSession = malloc(sizeof(GameSession));
    gms->gs = gameSession;

    if (!initialization(gameSession)) {
        SDL_Quit();
        return;
    }

    Player *player = malloc(sizeof(Player));

    player->recentActions = malloc(4 * sizeof(char *));

    gms->p = player;
    if (initialize_player(player, gameSession, (double)(WORLD_WIDTH / 2),
                          (double)(WORLD_HEIGHT / 2))) {
        return;
    }
    if (loadCharset(gameSession)) {
        return;
    }
    SDL_Rect sky_rect = {0, 0, SCREEN_WIDTH, SCREEN_WIDTH / 4};
    Entity *entities = createEntities(NUMOFOBSTACLES, gameSession);

    gms->entites = entities;
    EntitiesRandomPosition(entities, NUMOFOBSTACLES);

    int camera_offset = 0;
    gms->camera_offset = &camera_offset;

    int playerPoints = 0;
    gms->players_points = &playerPoints;
    int sky_color = color(gameSession, SKYBLUE);
    int background_color = color(gameSession, TITLEGRAY);
    int flip = SDL_FLIP_NONE;

    int quit = false;
    int t1, t2, frames, rc;
    double delta, worldTime, fpsTimer, fps, distance, timer_s, combo_delay;

    t1 = SDL_GetTicks();

    char text[128];
    combo_delay = COMBO_DELAY_S;
    frames = 0;
    fpsTimer = 0;
    fps = 0;
    worldTime = 0;
    gms->worldTime = &worldTime;
    timer_s = 0;

    while (!quit) {
        if (*gms->debug_exit == 1) {
            break;
        }

        t2 = SDL_GetTicks();
        SDL_RenderClear(gameSession->renderer);
        SDL_FillRect(gameSession->screen, NULL, background_color);
        SDL_FillRect(gameSession->screen, &sky_rect, sky_color);

        delta = (t2 - t1) * 0.001;
        t1 = t2;

        worldTime += delta;
        timer_s += delta;

        fpsTimer += delta;
        if (fpsTimer > 0.5) {
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 0.5;
        }

        if (timer_s >= 1) {
            addAction(player, NOACTIONCHAR);
            timer_s = 0;
        }

        while (SDL_PollEvent(&gameSession->event)) {

            switch (gameSession->event.type) {
            case SDL_KEYDOWN: {
                switch (gameSession->event.key.keysym.sym) {

                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_n:
                    restartGame(gms, gameSession);
                    break;
                case SDLK_w:
                    player->direction.y = UP;

                    break;
                case SDLK_s:
                    player->direction.y = DOWN;

                    break;
                case SDLK_a:
                    player->direction.x = LEFT;
                    flip = SDL_FLIP_HORIZONTAL;
                    player->lastHeading = LEFT;

                    break;
                case SDLK_d:
                    player->direction.x = RIGHT;
                    flip = SDL_FLIP_NONE;
                    player->lastHeading = RIGHT;

                    break;

                default:
                    break;
                }

            } break;
            case SDL_KEYUP: {
                switch (gameSession->event.key.keysym.sym) {

                case SDLK_s:
                    addAction(player, "DOWN");
                    timer_s = 0;
                    player->direction.y = RESET_ACTION;
                    break;

                case SDLK_w:

                    addAction(player, "UP");
                    timer_s = 0;
                    player->direction.y = RESET_ACTION;
                    break;

                case SDLK_a:
                    addAction(player, "LEFT");
                    timer_s = 0;
                    player->direction.x = RESET_ACTION;
                    break;

                case SDLK_d:
                    addAction(player, "RIGHT");
                    timer_s = 0;
                    player->direction.x = RESET_ACTION;
                    break;

                case SDLK_SPACE:
                    player->isJumping = JUMP;
                    addAction(player, "JUMP");
                    timer_s = 0;
                    break;
                case SDLK_x:
                    player->actions.type = LIGHT_ATTACK;
                    addAction(player, "LIGHT");
                    timer_s = 0;
                    // printf("Light attack executed\n");
                    break;
                case SDLK_y:
                    player->actions.type = HEAVY_ATTACK;
                    addAction(player, "HEAVY");
                    timer_s = 0;
                    // printf("Heavy attack executed\n");
                    break;
                }

            }; break;
            case SDL_QUIT:
                quit = 1;
                break;
            };
        };

        printf("Player position: %d %d   SCALE: %.02lf C_OFFSET: %d\n",
               player->rect.x, player->rect.y, player->scale, camera_offset);
        playerMovement(gameSession, player, delta);
        handleJumping(gms, delta);
        checkAttack(player, entities, gms);
        actionsHandling(player, delta, gameSession, gms);
        UpdatePlayerAnimation(player, delta);
        UpdateEntityAnimation(entities, delta);
        printf("checkCombo: %s\n", checkCombo(player));
        printf("CURRENT COMBO: %d DELAY: %lf\n", player->comboType.comboType,
               combo_delay);
        int test = checkCombo(player) != NOACTIONCHAR;
        printf("%d\n", test);
        if (strcmp(checkCombo(player), NOACTIONCHAR) != 0 &&
            player->comboType.comboType == NO_COMBO) {
            printf("CURRENT COMBO: %d DELAY: %lf\n",
                   player->comboType.comboType, combo_delay);
            player->comboType = InitCombo(checkCombo(player));
            for (int i = 0; i < 4; i++) {
                player->recentActions[i] = NOACTIONCHAR;
            }
        }
        if (player->comboType.comboType != NO_COMBO)
            player->comboType.comboTimeRemaining -= delta;
        else
            player->comboType.comboTimeRemaining = 0;
        printf("CURRENT COMBO: %d COMBOTIMER: %lf \n",
               player->comboType.comboType,
               player->comboType.comboTimeRemaining);
        // break;

        comboHandler(player, delta, gameSession, gms);

        if (player->position.x > SCREEN_WIDTH + camera_offset - CAMERA_GRACE) {
            camera_offset = player->position.x + CAMERA_GRACE - SCREEN_WIDTH;
        }
        if (player->position.x < camera_offset + CAMERA_GRACE) {
            camera_offset = player->position.x - CAMERA_GRACE;
        }
        if (camera_offset < 0) {
            camera_offset = 0;
        }
        if (camera_offset >
            WORLD_MAX_X - SCREEN_WIDTH + player->rect.w * player->scale) {
            camera_offset =
                WORLD_MAX_X - SCREEN_WIDTH + player->rect.w * player->scale;
        }

        // DrawTexture(gamestate->renderer, player->texture, player->position.x,
        // player->position.y+player->rect.h, player->rect.w, player->rect.h,
        // player->scale, 0,flip); Zmienna camera_offset (którą masz w mainLoop)
        // jest ważna,
        // jeśli chcesz przesuwać ekran. W przykładzie była używana.

        char text2[128];

        DrawRectangle(gameSession->screen, 4, 4, SCREEN_WIDTH - 8, 36,
                      color(gameSession, RED), color(gameSession, BLUE));
        sprintf(text,
                "Czas: %.1lf s %.0lf "
                "fps ",
                worldTime, fps);
        DrawString(gameSession->screen,
                   gameSession->screen->w / 2 - strlen(text) * 8 / 2, 10, text,
                   gameSession->charset);
        sprintf(text2, "[ %s %s %s %s ]", player->recentActions[0],
                player->recentActions[1], player->recentActions[2],
                player->recentActions[3]);
        DrawString(gameSession->screen,
                   gameSession->screen->w / 2 - strlen(text2) * 8 / 2, 20,
                   text2, gameSession->charset);
        // fffffffffffffffff
        DrawRectangle(gameSession->screen, 4, 4, 100, 36,
                      color(gameSession, BLACK), color(gameSession, RED));
        sprintf(text, "HEALTH: %d ", player->remainingHp);

        DrawString(gameSession->screen, 6, 20, text, gameSession->charset);

        // fffffffffffffffffffffffff
        // RenderEntities(entities, NUMOFOBSTACLES, gameSession, camera_offset);
        DrawEntityAnimation(gameSession, entities, camera_offset);
        DrawPlayerObstacleHitbox(player, 30, gms);
        DrawRectangle(gameSession->screen, player->position.x - camera_offset,
                      player->position.y + player->rect.h, 4, 4,
                      color(gameSession, RED), color(gameSession, BLUE));

        SDL_RenderCopy(gameSession->renderer, gameSession->scrtex, NULL, NULL);
        sprintf(text2, "[ %d POINTS  ]", *gms->players_points);
        DrawString(gameSession->screen,
                   gameSession->screen->w - strlen(text2) * 8,
                   SCREEN_HEIGHT - 30, text2, gameSession->charset);
        // SDL_RenderCopy(gamestate->renderer, player->texture, NULL,
        // &player->rect);

        SDL_UpdateTexture(gameSession->scrtex, NULL,
                          gameSession->screen->pixels,
                          gameSession->screen->pitch);

        // DrawTexture(gameSession->renderer, player->texture,
        //             (int)player->position.x -
        //                 camera_offset, // X z uwzględnieniem kamery
        //             (int)player->position.y +
        //                 player->rect.h, // Y STÓP (Głowa + Wysokość)
        //             player->rect.w,     // Szerokość (100) - bezpieczna z
        //             rect player->rect.h,     // Wysokość (100) - bezpieczna z
        //             rect player->scale,      // Twoja skala 0, flip);
        DrawPlayerAnimation(gameSession, player, *gms->camera_offset);

        SDL_RenderPresent(gameSession->renderer);

        frames++;
    }

    printf("gameloop\n");

    deallocation(gms);
}

void deallocation(GameState *gms) {

    SDL_DestroyRenderer(gms->gs->renderer);
    SDL_DestroyWindow(gms->gs->window);
    free(gms->p);
    free(gms->entites);
    free(gms->p->recentActions);
    free(gms->gs);

    SDL_Quit();
}
