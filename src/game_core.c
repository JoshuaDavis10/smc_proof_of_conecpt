#include "game_core.h"

#include "logger.h"
#include "events.h"

#include <stdlib.h>
#include <string.h>

//event callbacks
unsigned char walk(int code, void* sender, void* listener, event_context context);
unsigned char jump(int code, void* sender, void* listener, event_context context);
unsigned char roll(int code, void* sender, void* listener, event_context context);
unsigned char fire_weapon(int code, void* sender, void* listener, event_context context);
//event callbacks

static game_state* game = 0;

unsigned char initialize_game_state() {
    if(game != 0) {
        LOGWARN("Tried to initialize game state after it had already been initialized.");
        return 0;
    }

    game = malloc(sizeof(game_state));
    memset(game, 0, sizeof(game_state));

    if(game == 0) {
        LOGERROR("Game state initialization failed.");
        return 0;
    }

    game->turn = P1;

    //mech 1 name
    char buf1[32]="billy";
    //mech 2 name
    char buf2[32]="timmy";

        //mech 1
    game->mechs[P1].position = 0;
    strcpy(game->mechs[P1].name, buf1);
    legs legs1; 
    legs1.health = 30;
    legs1.jump = 0;
    legs1.roll = 1;
    game->mechs[P1].legs = legs1;
    torso torso1;
    torso1.health = 70;
    game->mechs[P1].torso = torso1;
    game->mechs[P1].health = game->mechs[P1].torso.health + game->mechs[P1].legs.health;
    weapon weapon1;
    weapon weapon2;
    weapon1.damage = 27;
    weapon1.range = 3;
    weapon1.uses = 3;
    weapon2.damage = 20;
    weapon2.range = 5;
    weapon2.uses = 2;
    game->mechs[P1].weapon[0] = weapon1;
    game->mechs[P1].weapon[1] = weapon2;

        //mech 2
    game->mechs[P2].position = 9;
    strcpy(game->mechs[P2].name, buf2);
    legs legs2; 
    legs2.health = 30;
    legs2.jump = 1;
    legs2.roll = 0;
    game->mechs[P2].legs = legs2;
    torso torso2;
    torso2.health = 70;
    game->mechs[P2].torso = torso2;
    game->mechs[P2].health = game->mechs[P1].torso.health + game->mechs[P1].legs.health;
    weapon weapon3;
    weapon weapon4;
    weapon3.damage = 40;
    weapon3.range = 2;
    weapon3.uses = 2;
    weapon4.damage = 15;
    weapon4.range = 10;
    weapon4.uses = 2;
    game->mechs[P2].weapon[0] = weapon3;
    game->mechs[P2].weapon[1] = weapon4;

    //register for events
    register_event(GAME_CORE_WALK, game, walk);
    register_event(GAME_CORE_JUMP, game, jump);
    register_event(GAME_CORE_ROLL, game, roll);
    register_event(GAME_CORE_FIRE_WEAPON, game, fire_weapon);


    LOGINFO("Game state initialized.");
    return 1;
}

unsigned char mech_damage(int mech, int damage) {
    if(mech == P1) {
        if(!_mech_take_damage(&(game->mechs[mech]), damage)) {
            return 0;
        }
        return 1;
    }
    if(mech == P2) {
        if(!_mech_take_damage(&(game->mechs[mech]), damage)) {
            return 0;
        }
        return 1;
    }
    LOGERROR("mech_damage: invalid value passed for 'mech' param: %d (must be P1 (%d) or P2 (%d)).", mech, P1, P2);
    return 0;
}

//TODO: mechs should not be able to move past each other? <- check how Super Mechs does it
//currently this function won't let them move to same spot but will let them move past each other
unsigned char mech_move(int mech, int direction, int distance) {
    int resulting_position; 
    //no need for else here since _mech_move will catch invalid direction param anyway
    if(mech == P1) {
        if(direction == DIR_LEFT) {
            resulting_position = game->mechs[mech].position - distance;
            if(resulting_position == game->mechs[P2].position) {
                LOGWARN("mech_move: both mechs cannot be on the same tile; cannot carry out move command for P1 from %d to %d", 
                        game->mechs[mech].position, resulting_position);
                return 0;
            }
        }
        else if(direction == DIR_RIGHT) {
            resulting_position = game->mechs[mech].position + distance;
            if(resulting_position == game->mechs[P2].position) {
                LOGWARN("mech_move: both mechs cannot be on the same tile; cannot carry out move command for P1 from %d to %d", 
                        game->mechs[mech].position, resulting_position);
                return 0;
            }
        }
        if(!_mech_move(&(game->mechs[mech]), direction, distance)) {
            return 0;
        }
        return 1;
    }
    if(mech == P2) {
        if(direction == DIR_LEFT) {
            resulting_position = game->mechs[mech].position - distance;
            if(resulting_position == game->mechs[P1].position) {
                LOGWARN("mech_move: both mechs cannot be on the same tile; cannot carry out move command for P1 from %d to %d", 
                        game->mechs[mech].position, resulting_position);
                return 0;
            }
        }
        else if(direction == DIR_RIGHT) {
            resulting_position = game->mechs[mech].position + distance;
            if(resulting_position == game->mechs[P1].position) {
                LOGWARN("mech_move: both mechs cannot be on the same tile; cannot carry out move command for P1 from %d to %d", 
                        game->mechs[mech].position, resulting_position);
                return 0;
            }
        }
        if(!_mech_move(&(game->mechs[mech]), direction, distance)) {
            return 0;
        }
        return 1;
    }
    LOGERROR("mech_move: invalid value passed for 'mech' param: %d (must be P1 (%d) or P2 (%d)).", mech, P1, P2);
    return 0;
}

int mech_get_health(int mech) {
    if(mech == P1 || mech == P2) {
        return (game->mechs[mech].health);
    }
    LOGERROR("mech_get_health: invalid paramterer for 'mech': %d. Must be P1 (%d) or P2 (%d). Returning -1", mech, P1, P2);
    return -1; //bc of the way _mech_take_damage func works, health shouldn't ever be below 0
}

mech mech_get_info(int mech) {
    if(mech == P1 || mech == P2) {
        return game->mechs[mech];
    }
    LOGERROR("mech_get_health: invalid paramterer for 'mech': %d. Must be P1 (%d) or P2 (%d). Return behavior undefined.", mech, P1, P2);
    return game->mechs[mech];
}

unsigned char switch_turn() {
    if(game->turn == P1) {
        game->turn = P2;
        return 1;
    }
    if(game->turn == P2) {
        game->turn = P1;
        return 1;
    }
    LOGERROR("switch_turn: game's turn was somehow set to something other than P1 or P2.");
    return 0;
}

int get_turn() {
    if(game->turn == P1) 
        return P1;
    if(game->turn == P2) 
        return P2;
    LOGERROR("get_turn: game's turn was somehow set to something other than P1 or P2.");
    return -1;


}

unsigned char mech_use_weapon(int mech, int weapon) {
    if(mech == P1 || mech == P2) {
        if(weapon == 1 || weapon == 2) {
            game->mechs[mech].weapon[weapon-1].uses--;
            LOGINFO("decreased %s's weapon %d's uses from %u to %u", game->mechs[mech].name, weapon, game->mechs[mech].weapon[weapon-1].uses+1, game->mechs[mech].weapon[weapon-1].uses); 
            return 1;
        }
        LOGERROR("mech_use_weapon: invalid 'weapon' paramter %d. Must be 1 or 2", weapon);
        return 0;
    }
    LOGERROR("mech_use_weapon: invalid 'mech' paramter %d. Must be P1 (0) or P2 (1)", mech);
    return 0;
}

//event callbacks definitions
unsigned char walk(int code, void* sender, void* listener, event_context context) {
    //TODO: code here
    //that is weird if it's an 'E'
    int turn;
    turn = context.data[1];
    if(!mech_move(turn, context.data[0], 1)) {
        return 0;
    }
    switch_turn();
    return 1;
}

unsigned char jump(int code, void* sender, void* listener, event_context context) {
    //TODO: code here
    char dir;
    int turn;
    turn = context.data[1];
    if(context.data[0] == DIR_LEFT)
        dir = 'L';
    if(context.data[0] == DIR_RIGHT)
        dir = 'R';
    LOGTEXT("jump called with direction '%c'!", LOGBOLDBLUE, dir);
    mech_move(turn, context.data[0], 2);
    switch_turn();
    return 1;
}

unsigned char roll(int code, void* sender, void* listener, event_context context) {
    //TODO: code here
    char dir;
    int dist;
    if(context.data[0] == DIR_LEFT)
        dir = 'L';
    if(context.data[0] == DIR_RIGHT)
        dir = 'R';
    dist = context.data[1];
    LOGTEXT("roll called with direction '%c' and distance %d!", LOGBOLDBLUE, dir, dist);
    return 1;
}

unsigned char fire_weapon(int code, void* sender, void* listener, event_context context) {
    LOGTEXT("fire_weapon called!", LOGBOLDBLUE);

    int enemy;
    int mech;
    //idk that's weird
    if(context.data[1] == P1) {
        mech = P1;
        enemy = P2;
    }
    else {
        mech = P2;
        enemy = P1;
    }
    int weapon = context.data[0];

    LOGINFO("%s's weapon %d uses: %u", game->mechs[mech].name, weapon + 1, game->mechs[mech].weapon[weapon].uses);
    if(game->mechs[mech].weapon[weapon].uses > 0) {
        int distance = game->mechs[mech].position - game->mechs[enemy].position;
        if(distance < 0)
            distance = -distance;

        if(distance <= game->mechs[mech].weapon[weapon].range) {
            int damage = game->mechs[mech].weapon[weapon].damage;
            if(!mech_damage(enemy, damage)) {
                LOGWARN("couldn't carry out attack. See output from _mech_take_damage above ^");
            }
            mech_use_weapon(mech, weapon+1);
            switch_turn();
            LOGINFO("%s's health: %d", game->mechs[P1].name, game->mechs[P1].health);
            LOGINFO("%s's health: %d", game->mechs[P2].name, game->mechs[P2].health);
        }
        else {
            LOGWARN("can't fire weapon %d for player %d... out of range", weapon, mech);
            return 0;
        }
    }
    else {
        LOGWARN("can't fire weapon %d for player %d... weapon is out of uses", weapon, mech);
        return 0;
    }

    return 1;
}
//event callbacks definitions
