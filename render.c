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

void doneTasks(GameSession *gameSession) {
    char text[128];
    sprintf(text, "WYMOGI: 1234ABCD");
    DrawString(gameSession->screen,
               gameSession->screen->w / 2 - strlen(text) * 8 / 2, 40, text,
               gameSession->charset);
}

void showMultiplier(GameSession *gs, Player *p) {

    char multiplierText[32];
    sprintf(multiplierText, "COMBO x%d", p->multiplier);
    Uint8 r = 255, g = 255, b = 255;
    double scale = 1;

    if (p->multiplier >= ULTRA) {
        r = 255, g = 0, b = 0;
        scale = 2.5;
    } else if (p->multiplier >= MEGA) {
        r = 255, g = 77, b = 0;
        scale = 2;
    } else if (p->multiplier >= SUPER) {
        r = 255, g = 154, b = 0;
        scale = 1.5;
    } else if (p->multiplier >= DOUBLE) {
        r = 255, g = 193, b = 0;
        scale = 1.25;
    } else if (p->multiplier >= NO_MUL) {
        r = 0, g = 255, b = 0;
    }

    SDL_SetSurfaceColorMod(gs->charset, r, g, b);

    
    DrawStringS(gs->screen, SCREEN_WIDTH - (70 * scale), 40, multiplierText,
                gs->charset, scale);

    
    SDL_SetSurfaceColorMod(gs->charset, 255, 255, 255);
}

int initialize_player(Player *player, GameSession *gameSession, double x,
                      double y, GameState *gms) {
    player->surface = SDL_LoadBMP("./textures/mainPlayerSprite.bmp");
    if (player->surface == NULL) {
        printf("SDL_LoadBMP(stickman.bmp) error: %s\n", SDL_GetError());
        deallocation(gms);
        SDL_Quit();
        return 1;
    }
    SDL_SetColorKey(player->surface, SDL_TRUE,
                    SDL_MapRGB(player->surface->format, 255, 0, 255));
    player->texture =
        SDL_CreateTextureFromSurface(gameSession->renderer, player->surface);
    if (player->texture == NULL) {
        printf("player_texture creation error: %s\n", SDL_GetError());
        deallocation(gms);
        SDL_Quit();
        return 1;
    }
    playerInits(player, x, y, gameSession);

    return 0;
}


void DrawPlayerAnimation(GameSession *gs, Player *p, int cameraOffset) {

    SDL_Rect srcRect; 
    SDL_Rect destRect; 

    
    srcRect.x = p->currentFrame * p->frameWidth; 
    srcRect.y = p->currentRow * p->frameHeight;  
    srcRect.w = p->frameWidth;
    srcRect.h = p->frameHeight;

    
    destRect.x = (int)p->position.x - cameraOffset;
    destRect.y = (int)p->position.y - (p->rect.h * p->scale) +
                 p->rect.h; 
    destRect.w = p->frameWidth * p->scale;
    destRect.h = p->frameHeight * p->scale;

    
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (p->lastHeading == LEFT) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    
    SDL_RenderCopyEx(gs->renderer, p->texture,
                     &srcRect, 
                     &destRect, 0, NULL, flip);
}

void DrawEntityAnimation(GameSession *gs, Entity *entities, int cameraOffset) {

    for (int i = 0; i < NUMOFOBSTACLES; i++) {
        Entity *e = &entities[i];
        
        if (e->isInitialized) {
            SDL_Rect srcRect;  
            SDL_Rect destRect; 

            
            srcRect.x = e->currentFrame * e->frameWidth; 
            srcRect.y = e->currentRow * e->frameHeight;  
            srcRect.w = e->frameWidth;
            srcRect.h = e->frameHeight;

            
            destRect.x = (int)e->rect.x - cameraOffset;
            destRect.y = (int)e->rect.y; 
            destRect.w = e->frameWidth;
            destRect.h = e->frameHeight;

            

            
            SDL_RenderCopyEx(
                gs->renderer, e->texture,
                &srcRect, 
                &destRect, 0, NULL, 0);
        }
    }
}



void DrawPlayerAttack(Player *p, GameSession *gs, GameState *gms) {

    SDL_Rect attackBox = getAttackBox(p);
    int attackColor;

    if (p->actions.type == LIGHT_ATTACK) {
        attackColor = color(gs, GREEN);
        SDL_SetTextureColorMod(p->texture, 0, 255, 0);
    } else if (p->actions.type == HEAVY_ATTACK) {
        attackColor = color(gs, RED);
        SDL_SetTextureColorMod(p->texture, 255, 0, 0);

    } else if (p->comboType.comboType == SUPERCOMBO) {

        attackColor = color(gs, BLUE);
        SDL_SetTextureColorMod(p->texture, 0, 0, 255);
    } else if (p->comboType.comboType == LIGHT_COMBO) {

        attackColor = color(gs, YELLOW);
        SDL_SetTextureColorMod(p->texture, 255, 255, 0);
    } else if (p->comboType.comboType == HEAVY_COMBO) {

        attackColor = color(gs, ORANGE);
        SDL_SetTextureColorMod(p->texture, 255, 128, 0);
    } else {

        SDL_SetTextureColorMod(p->texture, 255, 255,
                               255); // Reset koloru gracza

        return;
    }

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


    DrawFrame(gms->gs->screen, HitboxRect.x - *gms->camera_offset, HitboxRect.y,
              HitboxRect.h, HitboxRect.w, color(gms->gs, BLACK));
}

void RenderEntities(Entity *entities, int numOfEntites, GameSession *gs,
                    int cameraOffset) {

    for (int i = 0; i < numOfEntites; i++) {
        Entity *e = &entities[i];

        if (e->isInitialized) {
            int scrX = e->rect.x - cameraOffset;
            DrawRectangle(gs->screen, scrX, e->rect.y, e->rect.w, e->rect.h,
                          color(gs, BLACK), color(gs, RED));
        }
    }
}

int initialization(GameSession *gameSession, GameState *gms) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 0;
    }

    gameSession->window =
        SDL_CreateWindow("basic", 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gameSession->renderer = SDL_CreateRenderer(gameSession->window, 1, 0);
    if (gameSession->renderer == NULL) {
        deallocation(gms);
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

