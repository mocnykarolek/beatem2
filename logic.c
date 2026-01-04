
#include "logic.h"

#include "render.h"
// [0,1,2,3]

void addAction(Player* p, char *action){
    
    for (int i = 3; i > 0; i--)
    {
        p->recentActions[i] = p->recentActions[i-1];
    }
    p->recentActions[0] = action;
    

}

void restartGame(GameState *gms, GameSession* g){
    
    if(initialize_player(gms->p, g, (double)(WORLD_WIDTH /2), (double)(WORLD_HEIGHT /2))){
        return;
    }
    *gms->worldTime = 0;
    *gms->camera_offset = 0;

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
void handleJumping(GameState* gms, double delta){

    if(gms->p->isJumping && gms->p->RemainingJumpTime_s > gms->p->jumpingTime_s / 2 ){
        
        gms->p->RemainingJumpTime_s -= delta;
        gms->p->direction.y = UP;
    }
    else if(gms->p->isJumping && gms->p->RemainingJumpTime_s > 0){
        gms->p->RemainingJumpTime_s -= delta;
        gms->p->direction.y = DOWN;
    }

    if(gms->p->RemainingJumpTime_s <= 0){
        gms->p->isJumping = 0;
        gms->p->RemainingJumpTime_s = 2;
        gms->p->direction.y = RESET_ACTION;
    }


}



void playerMovement(GameSession* gs, Player* p, double delta){
    if(p->position.y < WORLD_MIN_Y){
        p->position.y = WORLD_MIN_Y;
    }

    if(p->position.y + p->rect.h > WORLD_MAX_Y){
        p->position.y = WORLD_MAX_Y - p->rect.h;
    }

    if(p->position.x < WORLD_MIN_X){
        p->position.x = WORLD_MIN_X;
    }

    if(p->position.x > WORLD_MAX_X){
        p->position.x = WORLD_MAX_X;
    }


    p->position.x += p->direction.x * PLAYER_SPEED * delta;

    
    p->position.y += p->direction.y * PLAYER_SPEED * delta;
    
    p->rect.x = (int)p->position.x;
    p->rect.y = (int)p->position.y;


    
    p->scale = 1.0f +  p->position.y / ((float)WORLD_MAX_Y - (float)WORLD_MIN_Y) * 2.0f;





}



void mainLoop(GameSession* gameSession){
    GameState *gms = malloc(sizeof(GameState));

    if(!initialization(gameSession)){
        SDL_Quit();
        return;
    }
    
    Player *player= malloc(sizeof(Player));
    char t[20];
    player->recentActions = malloc(4*sizeof(t));
    
    gms->p = player;
    if(initialize_player(player, gameSession, (double)(WORLD_WIDTH /2), (double)(WORLD_HEIGHT /2))){
        return;
    }
    if(loadCharset(gameSession)){
        return;

    }
    SDL_Rect sky_rect = {0,0, SCREEN_WIDTH, SCREEN_WIDTH/4};

    int camera_offset = 0;
    gms->camera_offset = &camera_offset;
    
    int sky_color = color(gameSession, SKYBLUE);
    int background_color = color(gameSession, TITLEGRAY);
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
    gms->worldTime = &worldTime;


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



        while(SDL_PollEvent(&gameSession->event)) {
            
            switch (gameSession->event.type) {
                case SDL_KEYDOWN: {
                    switch (gameSession->event.key.keysym.sym)
                    {
                    
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_n:
                        restartGame(gms,gameSession);
                        break;
                    case SDLK_w:
                        player->direction.y = UP;
                        
                        printf("Up\n");
                        break;
                    case SDLK_s:
                        player->direction.y = DOWN;
                        printf("Downkey\n");
                        break;
                    case SDLK_a:
                        player->direction.x = LEFT;
                        flip = SDL_FLIP_HORIZONTAL;
                        printf("left\n");
                        break;
                    case SDLK_d:
                        player->direction.x = RIGHT;
                        flip = SDL_FLIP_NONE;
                        printf("right\n");
                        break;


                    default:
                        break;
                    }
                    
                } break;
                case SDL_KEYUP:{
                    switch (gameSession->event.key.keysym.sym){
                        case SDLK_s:
                        case SDLK_w: {
                        
                        addAction(player, "X");
                        player->direction.y = RESET_ACTION;
                        break;
                        }
                        case SDLK_a:
                        case SDLK_d: {
                            addAction(player, "Y");
                        player->direction.x = RESET_ACTION;
                        break;
                        }   
                        case SDLK_SPACE:
                            player->isJumping = JUMP;
                        
                    }


                    
                }; break;
                case SDL_QUIT: 
                    quit = 1;
                    break;
                


            };
            
        
        };


        printf("Player position: %d %d   SCALE: %.02lf C_OFFSET: %d\n", player->rect.x, player->rect.y, player->scale, camera_offset);
        playerMovement(gameSession, player, delta);
        handleJumping(gms, delta);


        if (player->position.x >  SCREEN_WIDTH + camera_offset - CAMERA_GRACE){
            camera_offset = player->position.x + CAMERA_GRACE - SCREEN_WIDTH;
        }
        if( player->position.x < camera_offset + CAMERA_GRACE){
            camera_offset = player->position.x - CAMERA_GRACE;
        }    
        if(camera_offset < 0){
            camera_offset = 0;
        }
        if(camera_offset > WORLD_MAX_X - SCREEN_WIDTH + player->rect.w * player->scale){
            camera_offset = WORLD_MAX_X - SCREEN_WIDTH + player->rect.w * player->scale;
        }

        


        SDL_RenderClear(gameSession->renderer);



        
        SDL_FillRect(gameSession->screen, NULL, background_color);
        SDL_FillRect(gameSession->screen, &sky_rect, sky_color);
        

        // DrawTexture(gamestate->renderer, player->texture, player->position.x, player->position.y+player->rect.h, player->rect.w, player->rect.h, player->scale, 0,flip);
        // Zmienna camera_offset (którą masz w mainLoop) jest ważna, 
// jeśli chcesz przesuwać ekran. W przykładzie była używana.

        char text2[128];
        DrawRectangle(gameSession->screen, 4, 4, SCREEN_WIDTH - 8, 36, color(gameSession, RED), color(gameSession, BLUE));
        sprintf(text, "Czas: %.1lf s %.0lf ""fps ", worldTime, fps);
        DrawString(gameSession->screen, gameSession->screen->w /2 -strlen(text) * 8 /2 ,10, text, gameSession->charset);
        sprintf(text2, "[ %s %s %s %s ]", player->recentActions[0] ,player->recentActions[1], player->recentActions[2], player->recentActions[3]);
        DrawString(gameSession->screen, gameSession->screen->w /2 -strlen(text2) * 8 /2 ,20, text2, gameSession->charset);


        SDL_RenderCopy(gameSession->renderer, gameSession->scrtex, NULL, NULL);


        // SDL_RenderCopy(gamestate->renderer, player->texture, NULL, &player->rect);
        SDL_UpdateTexture(gameSession->scrtex, NULL, gameSession->screen->pixels, gameSession->screen->pitch);
        DrawTexture(gameSession->renderer, 
            player->texture, 
            (int)player->position.x - camera_offset,     // X z uwzględnieniem kamery
            (int)player->position.y + player->rect.h,    // Y STÓP (Głowa + Wysokość)
            player->rect.w,    // Szerokość (100) - bezpieczna z rect
            player->rect.h,    // Wysokość (100) - bezpieczna z rect
            player->scale,     // Twoja skala
            0, 
            flip);
        SDL_RenderPresent(gameSession->renderer);

        frames++;
    }

    printf("gameloop\n");


    deallocation(gameSession, player);
}



void deallocation(GameSession* gamestate, Player* player){

    SDL_DestroyRenderer(gamestate->renderer);
    SDL_DestroyWindow(gamestate->window);
    free(player);

    SDL_Quit();

}

