#pragma once

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 500

#define MECH_WIDTH 50
#define MECH_HEIGHT 80

typedef struct g_game {
    unsigned char allow_input_flag;
} g_game;

unsigned char run_g_game();
