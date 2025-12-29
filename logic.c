
#include "logic.h"

#include "render.h"

int initialization(GameState* gamestate){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 0;
    }

    gamestate->window = SDL_CreateWindow("basic", 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    gamestate->renderer = SDL_CreateRenderer(gamestate->window, 1, 0);
    if (gamestate->renderer == NULL) {
        SDL_Quit();
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 0;
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

    return 1;
}


void playerMovement(GameState* gs, Player* p, double delta){

    p->position.x += p->direction.x * PLAYER_SPEED * delta;
    p->position.y += p->direction.y * PLAYER_SPEED * delta;

    p->rect.x = p->position.x;
    p->rect.y = p->position.y;
    p->scale = 1 + p->position.y / (WORLD_MAX_Y - WORLD_MIN_Y) *2;

}



void mainLoop(GameState* gamestate){
    if(!initialization(gamestate)){
        SDL_Quit();
        return;
    }
    
    Player *player= malloc(sizeof(Player));

    if(initialize_player(player, gamestate, (double)(WORLD_WIDTH /2), (double)(WORLD_HEIGHT /2))){
        return;
    }
    if(loadCharset(gamestate)){
        return;

    }
    SDL_Rect sky_rect = {0,0, SCREEN_WIDTH, SCREEN_WIDTH/4};

    int camera_offset = 0;
    
    int sky_color = color(gamestate, SKYBLUE);
    int background_color = color(gamestate, TITLEGRAY);
    int flip = SDL_FLIP_NONE;

    int quit = false;
    int t1, t2, frames, rc;
    double delta, worldTime, fpsTimer, fps, distance;

    t1 = SDL_GetTicks();

    char text[128];
    frames = 0;
    fpsTimer = 0;
    fps = 0;
    worldTime = 0;


    while(!quit){
        t2 = SDL_GetTicks();


        delta = (t2-t1) * 0.001;
        t1 = t2;

        worldTime +=delta;

        fpsTimer += delta;
        if(fpsTimer > 0.5) {
            fps = frames *2;
            frames = 0;
            fpsTimer -= 0.5;
        }



        while(SDL_PollEvent(&gamestate->event)) {
            switch (gamestate->event.type) {
                case SDL_KEYDOWN: {
                    switch (gamestate->event.key.keysym.sym)
                    {
                    
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_w:
                        player->direction.y = PLAYER_UP;
                        printf("Up\n");
                        break;
                    case SDLK_s:
                        player->direction.y = PLAYER_DOWN;
                        printf("Downkey\n");
                        break;
                    case SDLK_a:
                        player->direction.x = PLAYER_LEFT;
                        printf("left\n");
                        break;
                    case SDLK_d:
                        player->direction.x = PLAYER_RIGHT;
                        printf("right\n");
                        break;


                    default:
                        break;
                    }
                    
                } break;
                case SDL_KEYUP:{
                    switch (gamestate->event.key.keysym.sym){
                        case SDLK_s:
                        case SDLK_w: {
                        player->direction.y = 0;
                        break;
                        }
                        case SDLK_a:
                        case SDLK_d: {
                        player->direction.x = 0;
                        break;
                        }
                        
                    }


                    
                }; break;
                case SDL_QUIT: 
                    quit = 1;
                    break;
                


            };
            
        
        };


        printf("Player position: %d %d   SCALE: %.02lf\n", player->rect.x, player->rect.y, player->scale);
        playerMovement(gamestate, player, delta);



        SDL_RenderClear(gamestate->renderer);



        
        SDL_FillRect(gamestate->screen, NULL, background_color);
        SDL_FillRect(gamestate->screen, &sky_rect, sky_color);
        

        // DrawTexture(gamestate->renderer, player->texture, player->position.x, player->position.y+player->rect.h, player->rect.w, player->rect.h, player->scale, 0,flip);
        DrawTexture(gamestate->renderer, player->texture, player->position.x, player->position.y+player->surface->h, player->surface->w, player->surface->h, player->scale, 0, flip);
        DrawRectangle(gamestate->screen, 4, 4, SCREEN_WIDTH - 8, 36, color(gamestate, RED), color(gamestate, BLUE));
        sprintf(text, "Czas: %.1lf s %.0lf ""fps ", worldTime, fps);
        DrawString(gamestate->screen, gamestate->screen->w /2 -strlen(text) * 8 /2 ,10, text, gamestate->charset);


        SDL_RenderCopy(gamestate->renderer, gamestate->scrtex, NULL, NULL);


        SDL_RenderCopy(gamestate->renderer, player->texture, NULL, &player->rect);
        SDL_UpdateTexture(gamestate->scrtex, NULL, gamestate->screen->pixels, gamestate->screen->pitch);
        SDL_RenderPresent(gamestate->renderer);

        frames++;
    }

    printf("gameloop\n");


    deallocation(gamestate);
}



void deallocation(GameState* gamestate){

    SDL_DestroyRenderer(gamestate->renderer);
    SDL_DestroyWindow(gamestate->window);
    SDL_Quit();

}
