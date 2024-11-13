#include "graphical_game.h"

#include "logger.h"
#include "game_core.h"
#include "events.h"
#include "renderer.h"

#include "../include/raylib.h"

#include <stdlib.h>
#include <string.h>

static g_game* game = 0;

//functions
void poll_user_input() {
    
    if(game->allow_input_flag) {

        if(IsKeyPressed(KEY_Q)) {
            event_context c;
            c.data[0] = DIR_LEFT;
            c.data[1] = get_turn();
            if(!fire_event(GAME_CORE_WALK, 0, c)){}
            else {
                fire_event(MOVE_ANIMATION_STARTED, 0, c); 
                game->allow_input_flag = 0;
                LOGINFO("input disabled.");
            }
        }
        if(IsKeyPressed(KEY_E)) {
            event_context c;
            c.data[0] = DIR_RIGHT;
            c.data[1] = get_turn();
            if(!fire_event(GAME_CORE_WALK, 0, c)){}
            else {
                fire_event(MOVE_ANIMATION_STARTED, 0, c); 
                game->allow_input_flag = 0;
                LOGINFO("input disabled.");
            }
        }
        if(IsKeyPressed(KEY_A)) {
            event_context c;
            c.data[0] = DIR_LEFT;
            c.data[1] = get_turn();
            fire_event(GAME_CORE_JUMP, 0, c); 
            game->allow_input_flag = 0;
            LOGINFO("input disabled.");
        }
        if(IsKeyPressed(KEY_D)) {
            event_context c;
            c.data[0] = DIR_RIGHT;
            c.data[1] = get_turn();
            fire_event(GAME_CORE_JUMP, 0, c); 
            game->allow_input_flag = 0;
            LOGINFO("input disabled.");
        }
        if(IsKeyPressed(KEY_ONE)) {
            event_context c;
            c.data[0] = 0; //weapon
            c.data[1] = get_turn(); //turn
            fire_event(GAME_CORE_FIRE_WEAPON, 0, c);
        }
        if(IsKeyPressed(KEY_TWO)) {
            event_context c;
            c.data[0] = 1; //weapon
            c.data[1] = get_turn(); //turn
            fire_event(GAME_CORE_FIRE_WEAPON, 0, c);
        }
    } 
}

unsigned char enable_input(int code, void* sender, void* listener, event_context context) {
    if(((g_game*)listener)->allow_input_flag == 1) 
        LOGWARN("enable_input called when input flag was already enable.");

    ((g_game*)listener)->allow_input_flag = 1;
    LOGINFO("input enabled.");
    return 1;
}
//functions

//execution
unsigned char run_g_game() {

    if(!initialize_events()) {
        LOGERROR("Failed to initialize events.");
        return 0;
    }

    //initialize graphical game
    game = malloc(sizeof(g_game));
    memset(game, 0, sizeof(g_game));
    game->allow_input_flag = 1;
    register_event(ENABLE_INPUT, game, enable_input); 
    //initialize graphical game

    if(!initialize_game_state()) {
        LOGERROR("Failed to initialize game state.");
        return 0;
    }

    if(!initialize_renderer()) {
        LOGERROR("Failed to initialized renderer.");
        return 0;
    }

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "graphical game");

    //draw loop
    while(!WindowShouldClose()) {

        mech p1 = mech_get_info(P1);
        mech p2 = mech_get_info(P2);

        //TODO: game logic here
        if(mech_get_health(P1) <= 0) {
            CloseWindow();
            LOGINFO("%s wins!", p2.name);
            return 1;
        }
        if(mech_get_health(P2) <= 0) {
            CloseWindow();
            LOGINFO("%s wins!", p1.name);
            return 1;
        }

        poll_user_input();

        //for now do keyboard commands. poll keyboard input and run functions accordingly. if you want the game loop to not manually call  
        //game_core functions, use your event system. game logic will fire an event and send it to game... game will have registered callbacks
        //for these events that will alter the game state when ran. that way all the game logic has to do is fire events without worrying about
        //having to manually call game_core functions?

        BeginDrawing();

        ClearBackground(BLUE);
        draw_mechs();

        EndDrawing();
    }
    //draw loop


    return 1;
}
//execution
