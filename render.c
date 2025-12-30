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
    player->position.x = x;
    player->position.y = y;

    player->rect.x = x;
    player->rect.y = y;
    player->rect.w = player->surface->w;
    player->rect.h = player->surface->h;
    // player->rect.w = 100;
    // player->rect.h = 100;

    player->direction.x = 0;
    player->direction.y = 0;

    player->scale = 1.0f +  player->position.y / ((float)WORLD_MAX_Y - (float)WORLD_MIN_Y) * 2.0f;

    for (int i = 0; i < 4; i++)
    {
        player->recentActions[i] = "...";
    }
    


    return 0;
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


// SDL_Texture *texture(){

//     SDL_CreateTextureFromSurface(renderer, player_surface);
    
// }