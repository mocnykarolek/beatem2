
#include "logic.h"

void initialization(GameState* gamestate){
    

    gamestate->window = SDL_CreateWindow("basic", 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gamestate->renderer = SDL_CreateRenderer(gamestate->window, 1, 0);
    if (gamestate->renderer == NULL) {
        SDL_Quit();
        printf("Could not create renderer: %s\n", SDL_GetError());
        return;
    };

    gamestate->screen =
        SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000,
                             0x0000FF00, 0x000000FF, 0xFF000000);

    gamestate->scrtex = SDL_CreateTexture(gamestate->renderer, SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                               SCREEN_HEIGHT);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(gamestate->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(gamestate->renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(gamestate->window, "example_window");
}




void mainLoop(GameState* gamestate){
    initialization(gamestate);
    
    int backgroud_color = SDL_MapRGB(gamestate->screen->format, 255, 0, 0);


    int quit = false;
    while(!quit){
        while(SDL_PollEvent(&gamestate->event)) {
            switch (gamestate->event.type) {
            case SDL_KEYDOWN: {
                quit = true;
            } break;
            }
        }


        SDL_RenderClear(gamestate->renderer);
        SDL_FillRect(gamestate->screen, NULL ,backgroud_color);
        SDL_UpdateTexture(gamestate->scrtex, NULL, gamestate->screen->pixels, gamestate->screen->pitch);
        
        SDL_RenderCopy(gamestate->renderer, gamestate->scrtex, NULL, NULL);
        SDL_RenderPresent(gamestate->renderer);


    }

    printf("gameloop\n");


    deallocation(gamestate);
}



void deallocation(GameState* gamestate){

    SDL_DestroyRenderer(gamestate->renderer);
    SDL_DestroyWindow(gamestate->window);
    SDL_Quit();

}
