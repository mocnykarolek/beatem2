
#include "logic.h"

#include "render.h"
// [0,1,2,3]

char* checkCombo(Player *p){
    char** comboBuffer = p->recentActions;

    if(comboBuffer[0] == comboBuffer[1] && comboBuffer[1] == comboBuffer[2] && comboBuffer[0] != NOACTIONCHAR) return comboBuffer[0];
    else if (comboBuffer[1] == comboBuffer[2] && comboBuffer[2] == comboBuffer[3] && comboBuffer[1]  != NOACTIONCHAR) return comboBuffer[1];
    else return NOACTIONCHAR;


}

void comboHandler(Player*p, double delta){
    
    if(p->comboType.comboType != NO_COMBO){
        if(p->comboType.comboTimeRemaining <= 0){
            p->comboType.comboType = NO_COMBO;
        }
        

        switch (p->comboType.comboType)
        {
        case LEFT_DASH:
            p->position.x += LEFT * p->speed * 2 * delta;   
            


            break;
        
        default:
            break;
        }


    }

}

void lightAttack(Player* p, double delta,GameSession *gs, GameState *gms){
    if(p->actions.RemainingTime_s == 0){
        p->actions.RemainingTime_s = PLAYERS_LIGHT_ACTION_DURATION_S;
    }
    
    if(p->actions.RemainingTime_s > 0){
        
        changePlayersColor(p, gs, gms);
        
        
        p->actions.RemainingTime_s -= delta;
        
    }
    

    if(p->actions.RemainingTime_s <= 0 && p->isAction){
        p->isAction = NOACTION;
        
        p->actions.type = NOACTION;
        p->actions.RemainingTime_s = PLAYERS_LIGHT_ACTION_DURATION_S;
    }

    



}

void HeavyAttack(Player* p, double delta,GameSession *gs, GameState *gms){
    if(p->actions.RemainingTime_s == 0){
        p->actions.RemainingTime_s = PLAYERS_HEAVY_ACTION_DURATION_S;
    }
    
    if(p->actions.RemainingTime_s > 0){
        
        changePlayersColor(p, gs, gms);
        
        p->actions.RemainingTime_s -= delta;
        
    }
    

    if(p->actions.RemainingTime_s <= 0 && p->isAction){
        p->isAction = NOACTION;
        
        p->actions.type = NOACTION;
        p->actions.RemainingTime_s = PLAYERS_HEAVY_ACTION_DURATION_S;
    }
}

void actionsHandling(Player* p, double delta, GameSession *gs, GameState *gms){
    
    
    if(p->actions.type == LIGHT_ATTACK){
        p->isAction = ACTION;
        lightAttack(p, delta,gs, gms);
        
    }
    else if (p->actions.type == HEAVY_ATTACK){
        p->isAction = ACTION;
        HeavyAttack(p, delta,gs, gms);
    }
    else if (p->actions.type == NOACTION) changePlayersColor(p, gs ,gms);
    
    

}


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
        gms->p->RemainingJumpTime_s = PLAYER_JUMP_TIME;
        gms->p->direction.y = RESET_ACTION;
    }

    // printf("Remaining jump timer: %lf\n", gms->p->RemainingJumpTime_s);
}

Combo InitCombo(char* comboChar){
    int Combotype;
    if(strcmp(comboChar, "LEFT") == 0) Combotype = LEFT_DASH;
    else if(strcmp(comboChar, "RIGHT") == 0) Combotype = RIGHT_DASH;
    else if(strcmp(comboChar, "UP")== 0) Combotype = UP_DASH;
    else if(strcmp(comboChar, "DOWN")== 0) Combotype = DOWN_DASH;
    else if(strcmp(comboChar, "LIGHT")== 0) Combotype = LIGHT_COMBO;
    else if(strcmp(comboChar, "HEAVY")== 0) Combotype = HEAVY_COMBO;
    else Combotype = NO_COMBO;
    
    Combo current_combo;
    switch (Combotype)
    {
    case LEFT_DASH:
        current_combo.comboType = LEFT_DASH;
        current_combo.comboInitialTime = 0.5f;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case RIGHT_DASH:
        current_combo.comboType = RIGHT_DASH;
        current_combo.comboInitialTime = 1;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case UP_DASH:
        current_combo.comboType = UP_DASH;
        current_combo.comboInitialTime = 1;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    case DOWN_DASH:
        current_combo.comboType = DOWN_DASH;
        current_combo.comboInitialTime = 1;
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
    
    default:
        current_combo.comboType = NO_COMBO;
        current_combo.comboInitialTime = 0;
        current_combo.comboTimeRemaining = current_combo.comboInitialTime;
        break;
    }
    

    return current_combo;


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

    

    p->position.x += p->direction.x * p->speed * delta;

    
    p->position.y += p->direction.y * p->speed * delta;
    
    p->rect.x = (int)p->position.x;
    p->rect.y = (int)p->position.y;


    
    p->scale = 1.0f +  p->position.y / ((float)WORLD_MAX_Y - (float)WORLD_MIN_Y) * 2.0f;





}



void mainLoop(GameState *gms){
    GameSession* gameSession = malloc(sizeof(GameSession));
    gms->gs = gameSession;

    if(!initialization(gameSession)){
        SDL_Quit();
        return;
    }
    
    Player *player= malloc(sizeof(Player));
    
    player->recentActions = malloc(4*sizeof(char*));
    
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


    while(!quit){
        
        t2 = SDL_GetTicks();
        SDL_RenderClear(gameSession->renderer);
        SDL_FillRect(gameSession->screen, NULL, background_color);
        SDL_FillRect(gameSession->screen, &sky_rect, sky_color);
        
        
        
        delta = (t2-t1) * 0.001;
        t1 = t2;

        worldTime +=delta;
        timer_s +=delta;

        fpsTimer += delta;
        if(fpsTimer > 0.5) {
            fps = frames *2;
            frames = 0;
            fpsTimer -= 0.5;
        }
        
        if(timer_s >= 1 ){
            addAction(player, NOACTIONCHAR);
            timer_s = 0;
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
                case SDL_KEYUP:{
                    switch (gameSession->event.key.keysym.sym){

                        case SDLK_s:
                        addAction(player, "DOWN");
                        player->direction.y = RESET_ACTION;
                        break;

                        case SDLK_w: 
                        
                        addAction(player, "UP");
                        player->direction.y = RESET_ACTION;
                        break;
                        
                        case SDLK_a:
                        addAction(player, "LEFT");
                        player->direction.x = RESET_ACTION;
                        break;

                        case SDLK_d: 
                            addAction(player, "RIGHT");
                        player->direction.x = RESET_ACTION;
                        break;
                          
                        case SDLK_SPACE:
                            player->isJumping = JUMP;
                            addAction(player, "JUMP");
                            break;
                        case SDLK_x:
                            player->actions.type = LIGHT_ATTACK;
                            addAction(player, "LIGHT");
                            // printf("Light attack executed\n");
                            break;
                        case SDLK_y:
                            player->actions.type = HEAVY_ATTACK;
                            addAction(player, "HEAVY");
                            // printf("Heavy attack executed\n");
                            break;
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
        actionsHandling(player, delta, gameSession, gms);
        printf("checkCombo: %s\n", checkCombo(player));
        printf("CURRENT COMBO: %d DELAY: %lf\n", player->comboType.comboType, combo_delay);
        int test = checkCombo(player) != NOACTIONCHAR;
        printf("%d\n", test);
        if(strcmp(checkCombo(player), NOACTIONCHAR) != 0 && player->comboType.comboType == NO_COMBO){
            printf("CURRENT COMBO: %d DELAY: %lf\n", player->comboType.comboType, combo_delay);
            player->comboType = InitCombo(checkCombo(player));
            
        }
        if(player->comboType.comboType != NO_COMBO) player->comboType.comboTimeRemaining -= delta;
        else player->comboType.comboTimeRemaining =0;
        printf("CURRENT COMBO: %d COMBOTIMER: %lf \n", player->comboType.comboType, player->comboType.comboTimeRemaining);
        // break;
        
        comboHandler(player, delta);
        

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

        

        

        
        





        // DrawTexture(gamestate->renderer, player->texture, player->position.x, player->position.y+player->rect.h, player->rect.w, player->rect.h, player->scale, 0,flip);
        // Zmienna camera_offset (którą masz w mainLoop) jest ważna, 
// jeśli chcesz przesuwać ekran. W przykładzie była używana.

        char text2[128];
        DrawRectangle(gameSession->screen, 4, 4, SCREEN_WIDTH - 8, 36, color(gameSession, RED), color(gameSession, BLUE));
        sprintf(text, "Czas: %.1lf s %.0lf ""fps ", worldTime, fps);
        DrawString(gameSession->screen, gameSession->screen->w /2 -strlen(text) * 8 /2 ,10, text, gameSession->charset);
        sprintf(text2, "[ %s %s %s %s ]", player->recentActions[0] ,player->recentActions[1], player->recentActions[2], player->recentActions[3]);
        DrawString(gameSession->screen, gameSession->screen->w /2 -strlen(text2) * 8 /2 ,20, text2, gameSession->charset);

        DrawRectangle(gameSession->screen, player->position.x - camera_offset, player->position.y + player->rect.h, 4, 4, color(gameSession, RED), color(gameSession, BLUE));

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


    deallocation(gms);
}



void deallocation(GameState* gms){

    SDL_DestroyRenderer(gms->gs->renderer);
    SDL_DestroyWindow(gms->gs->window);
    free(gms->p);

    SDL_Quit();

}

