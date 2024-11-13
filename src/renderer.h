#pragma once

#include "timer.h"

typedef struct render_state {
    //TODO: timers basically? store animations here?
    timer move_animation_timer; //TODO: this is temp im just tryna figure things out don't make fun of urself
    timer attack_animation_timer;
    int p1_pos; //this will be changed after move animation is finished... effectively used to calculate 'diff' between actual position and where the renderer things it is
    int p2_pos;
} render_state;

unsigned char initialize_renderer();
void draw_mechs();
void draw_attacks();
unsigned char start_move_animation_timer();
unsigned char reset_move_animation_timer();
