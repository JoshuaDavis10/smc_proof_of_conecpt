#include "../include/raylib.h"

#include "renderer.h"
#include "game_core.h"
#include "graphical_game.h"

#include "logger.h"
#include "events.h"

#include <stdlib.h>
#include <string.h>

static render_state* state = 0;

//callback functions
unsigned char start_move_animation_timer(int code, void* sender, void* listener, event_context context) {
    if(timer_start(&(((render_state*)listener)->move_animation_timer))) {
        LOGINFO("Move animation timer started.");
        return 1;
    }
    LOGWARN("Tried to start move animation timer after it has already been started.");
    return 0;
}

unsigned char start_attack_animation_timer(int code, void* sender, void* listener, event_context context) {
    if(timer_start(&(((render_state*)listener)->attack_animation_timer))) {
        LOGINFO("Attack animation timer started.");
        return 1;
    }
    LOGWARN("Tried to start attack animation timer after it has already been started.");
    return 0;
}
//callback functions

unsigned char initialize_renderer() {
    if(state != 0) {
        LOGWARN("Tried to initialize renderer after it has already been initialized.");
        return 0;
    }
    state = malloc(sizeof(render_state));
    memset(state, 0, sizeof(render_state));
    state->move_animation_timer.start_time = 0.0;
    state->move_animation_timer.duration   = 2.0;
    state->attack_animation_timer.start_time = 0.0;
    state->attack_animation_timer.duration   = 2.0;
    register_event(MOVE_ANIMATION_STARTED, state, start_move_animation_timer);
    register_event(ATTACK_ANIMATION_STARTED, state, start_move_animation_timer);
    mech p1 = mech_get_info(P1);
    mech p2 = mech_get_info(P2);
    state->p1_pos = p1.position;
    state->p2_pos = p2.position;
    LOGINFO("Renderer initialized.");
    return 1;
}

void draw_mechs() {
    int turn = get_turn();
    mech p1 = mech_get_info(P1);
    mech p2 = mech_get_info(P2);
    int p1_pos = p1.position;
    int p2_pos = p2.position;
    
    //set mech 'world coordinates'
    // i have no idea how to name variables
    int world_pos_spacing = WINDOW_WIDTH/NUM_POSITIONS;
    int deadspace = world_pos_spacing - MECH_WIDTH;
    //used by animation
    double dist_mult = world_pos_spacing/(state->move_animation_timer.duration);
    // i have no idea how to name variables
    int p1_world_pos =  p1_pos * world_pos_spacing + deadspace/2;
    int p2_world_pos =  p2_pos * world_pos_spacing + deadspace/2;

    Color p1_color = WHITE;
    Color p2_color = WHITE;
    if(turn == P1) {
        p1_color = GREEN;
    }
    if(turn == P2) {
        p2_color = GREEN;
    }

    //timer not active
    if(state->move_animation_timer.start_time == 0.0) {
        DrawRectangle(p1_world_pos, WINDOW_HEIGHT - MECH_HEIGHT, MECH_WIDTH, MECH_HEIGHT, p1_color);
        DrawRectangle(p2_world_pos, WINDOW_HEIGHT - MECH_HEIGHT, MECH_WIDTH, MECH_HEIGHT, p2_color);
    }
    //timer active
    else {
        //TODO: fire some event that halts user input
        //subtract '1' from official pos since the mech has been moved in game_core b4 animation is run
        double time_since_start = timer_get_time(state->move_animation_timer);
        //TODO: had to swap P2 and P1 and these if statement since turn switches after move command is called
        if(turn == P2) {
            p1_world_pos = state->p1_pos * world_pos_spacing + deadspace/2;
            if(state->p1_pos < p1_pos) {
                DrawRectangle(p1_world_pos + (time_since_start * dist_mult), WINDOW_HEIGHT - MECH_HEIGHT, MECH_WIDTH, MECH_HEIGHT, p1_color);
            }
            else {
                DrawRectangle(p1_world_pos - (time_since_start * dist_mult), WINDOW_HEIGHT - MECH_HEIGHT, MECH_WIDTH, MECH_HEIGHT, p1_color);
            }
            //p2 draw stays the same
            DrawRectangle(p2_world_pos, WINDOW_HEIGHT - MECH_HEIGHT, MECH_WIDTH, MECH_HEIGHT, p2_color);
        }
        //TODO: had to swap P2 and P1 and these if statement since turn switches after move command is called
        if(turn == P1) {
            //p1 draw stays the same
            DrawRectangle(p1_world_pos, WINDOW_HEIGHT - MECH_HEIGHT, MECH_WIDTH, MECH_HEIGHT, p1_color);
            p2_world_pos = state->p2_pos * world_pos_spacing + deadspace/2;
            if(state->p2_pos < p2_pos) {
                DrawRectangle(p2_world_pos + (time_since_start * dist_mult), WINDOW_HEIGHT - MECH_HEIGHT, MECH_WIDTH, MECH_HEIGHT, p2_color);
            }
            else {
                DrawRectangle(p2_world_pos - (time_since_start * dist_mult), WINDOW_HEIGHT - MECH_HEIGHT, MECH_WIDTH, MECH_HEIGHT, p2_color);
            }

        }
        //check if timer is up... if yes, set renderer's positions to current ones and reset timer
        double animation_time = timer_get_time(state->move_animation_timer);
        if(state->move_animation_timer.duration <= animation_time) {
            state->p1_pos = p1_pos;
            state->p2_pos = p2_pos;
            timer_reset(&(state->move_animation_timer));
            LOGINFO("Animation done.");
            event_context c;
            c.data[0] = 0;
            c.data[1] = 0;
            fire_event(ENABLE_INPUT, 0, c);
        }
    }
}

void draw_attacks() {


}
