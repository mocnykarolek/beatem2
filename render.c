#include "render.h"


int color(GameSession* gameSession, Colors color){

    switch (color)
    {
    case RED:
        return SDL_MapRGB(gameSession->screen->format, 0xFF, 0x00, 0x00);
        break;
    case BLACK:
        return  SDL_MapRGB(gameSession->screen->format, 0x00, 0x00, 0x00);
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
    default:
        return 0;
    }

    
    
}

int initialize_player(Player* player, GameSession* gameSession,double x, double y){

    player->surface = SDL_LoadBMP("./textures/stickman3.bmp");
    if(player->surface == NULL){
        printf("SDL_LoadBMP(stickman.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(gameSession->screen);
        SDL_FreeSurface(gameSession->charset);
        SDL_DestroyTexture(gameSession->scrtex);
        SDL_DestroyWindow(gameSession->window);
        SDL_DestroyRenderer(gameSession->renderer);
        SDL_Quit();
        return 1;
    }
    player->texture = SDL_CreateTextureFromSurface(gameSession->renderer, player->surface);
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
    player->speed = PLAYER_SPEED;

    player->rect.x = x;
    player->rect.y = y;
    player->rect.w = player->surface->w;
    player->rect.h = player->surface->h;
    player->position.x = x;
    player->position.y = y + player->rect.h;
    // player->rect.w = 100;
    // player->rect.h = 100;

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
    player->scale = 1.0f +  player->position.y / ((float)WORLD_MAX_Y - (float)WORLD_MIN_Y) * 2.0f;
    player->comboType.comboType = NO_COMBO;
    for (int i = 0; i < 4; i++)
    {
        player->recentActions[i] = "...";
    }
    


    return 0;
}


void changePlayersColor(Player*p, GameSession *gs, GameState *gms){
    
    if(p->actions.type == LIGHT_ATTACK) {
        SDL_SetTextureColorMod(p->texture, 0, 255, 0);
        
        if(p->lastHeading == LEFT) DrawRectangle(gs->screen, p->position.x + (p->rect.w + 30) * p->lastHeading * 2 - *gms->camera_offset, p->position.y + p->rect.h /4 , 100 , 30, color(gs, GREEN), color(gs, GREEN));
        if(p->lastHeading == RIGHT){
            if(p->position.x + (p->rect.w + 30) - *gms->camera_offset + PLAYER_LIGHT_REACH_px > WORLD_MAX_X){
                DrawRectangle(gs->screen, p->position.x + (p->rect.w + 30) - *gms->camera_offset , p->position.y + p->rect.h /4 , WORLD_MAX_X - p->position.x + (p->rect.w + 30) + *gms->camera_offset + PLAYER_LIGHT_REACH_px , 30, color(gs, GREEN), color(gs, GREEN));
            }
            else DrawRectangle(gs->screen, p->position.x + (p->rect.w + 30) - *gms->camera_offset , p->position.y + p->rect.h /4 , 100 , 30, color(gs, GREEN), color(gs, GREEN));
        }


    }
    else if(p->actions.type == HEAVY_ATTACK) {
        SDL_SetTextureColorMod(p->texture, 255, 0, 0);
        if(p->lastHeading == LEFT) DrawRectangle(gs->screen, p->position.x + (p->rect.w + 30) * p->lastHeading * 4 - *gms->camera_offset, p->position.y + p->rect.h /4 , PLAYER_HEAVY_REACH_px , 30, color(gs, RED), color(gs, RED));
        if(p->lastHeading == RIGHT){
            if(p->position.x + (p->rect.w + 30) - *gms->camera_offset + PLAYER_LIGHT_REACH_px > WORLD_MAX_X){
                DrawRectangle(gs->screen, p->position.x + (p->rect.w + 30) - *gms->camera_offset , p->position.y + p->rect.h /4 , WORLD_MAX_X - p->position.x + (p->rect.w + 30) + *gms->camera_offset + PLAYER_HEAVY_REACH_px  , 30, color(gs, GREEN), color(gs, RED));
            }
            else DrawRectangle(gs->screen, p->position.x + (p->rect.w + 30) - *gms->camera_offset , p->position.y + p->rect.h /4 , PLAYER_HEAVY_REACH_px  , 30, color(gs, RED), color(gs, RED));
        }
    }
    else if(p->actions.type == NOACTION) {
        SDL_SetTextureColorMod(p->texture, 255, 255, 255);
    }

}

int loadCharset(GameSession* gameSession){
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


int initialization(GameSession* gameSession){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 0;
    }

    gameSession->window = SDL_CreateWindow("basic", 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gameSession->renderer = SDL_CreateRenderer(gameSession->window, 1, 0);
    if (gameSession->renderer == NULL) {
        SDL_Quit();
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 0;
    };

    gameSession->screen =
        SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                             0x0000FF00, 0x000000FF, 0xFF000000);

    gameSession->scrtex = SDL_CreateTexture(gameSession->renderer, SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                               SCREEN_HEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(gameSession->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(gameSession->renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(gameSession->window, "example_window");

    return 1;
}

// SDL_Texture *texture(){

//     SDL_CreateTextureFromSurface(renderer, player_surface);
    
// }