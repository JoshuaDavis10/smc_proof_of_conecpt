#pragma once

//structs
typedef struct legs {
    unsigned int health;
    unsigned char jump; //can they jump?
    unsigned char roll; //can they roll?
    //TODO: add stomp info
} legs;

typedef struct torso {
    unsigned int health;
} torso;

//NOTE: strictly 2 side weapons max for now
typedef struct weapon {
    unsigned int damage;
    unsigned int range;
    unsigned int uses;
} weapon;

typedef struct mech {
    int health;
    int position;

    char name[32];

    legs legs;
    torso torso;
    weapon weapon[2];
} mech;
//structs


//functions
unsigned char _mech_take_damage(mech* mech, int damage); 
unsigned char _mech_move(mech* mech, int direction, int distance);

//TODO: one day we will have energy/heat, healing, etc... whatever stats are added for mechs we will need to keep track of
//so we'll have functions for each of those: _mech_lose_heat, _mech_gain_heat, _mech_lose_energy, etc...
//functions
