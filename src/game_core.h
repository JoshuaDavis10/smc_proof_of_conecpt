#pragma once

#include "mech.h"

#define NUM_POSITIONS 10

//enums
enum {
    DIR_LEFT,
    DIR_RIGHT
};

enum {
    P1,
    P2
};
//enums

//structs
typedef struct game_state {
    mech mechs[2];
    int turn; // either P1 or P2
} game_state;
//structs

//functions
unsigned char initialize_game_state(); 
unsigned char mech_damage(int mech, int damage);
unsigned char mech_move(int mech, int direction, int distance);
int mech_get_health(int mech); //TODO: don't rlly need this function. It is used in terminal_game.c but can use mech_get_info instead
//^ honestly it's nice to have because it's used by check_game() and that's all check game does so it doesn't rlly need all the other mech info
mech mech_get_info(int mech);
unsigned char switch_turn();
int get_turn();
unsigned char mech_use_weapon(int mech, int weapon); //TODO: we could merge this into one function with mech_damage(). So that the game doesn't have
//to worry about manually applying damage it can just run mech_use_weapon() thoughts?
//functions
