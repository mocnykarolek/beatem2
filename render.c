#include "render.h"

void init_window(){

    

}

int color(GameState* gamestate, Colors color){

    
    if (color == RED)
    {
        return SDL_MapRGB(gamestate->screen->format, 255, 0, 0);
    }
    else if (color == GREEN){
        return SDL_MapRGB(gamestate->screen->format, 0, 255, 0);
    }
    else if (color == BLUE){
        return SDL_MapRGB(gamestate->screen->format, 0, 255, 255);
    }
    

}


// SDL_Texture *texture(){

//     SDL_CreateTextureFromSurface(renderer, player_surface);
    
// }