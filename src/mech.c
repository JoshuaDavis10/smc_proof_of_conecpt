#include "mech.h"
#include "game_core.h"

#include "logger.h"

unsigned char _mech_take_damage(mech* mech, int damage) {
    if(mech == 0) {
        LOGWARN("_mech_take_damage: Passed NULL pointer for 'mech' param.");
        return 0;
    }
    if(mech->health <= 0) {
        LOGWARN("_mech_take_damage: Tried to run on a mech that has <= 0 health.");
        return 0;
    }
    if(damage < 0) {
        LOGWARN("_mech_take_damage: Invalid parameter value for 'damage': %d, must be >= 0", damage);
        return 0;
    }
    mech->health -= damage;
    if(mech->health < 0) {
        mech->health = 0;
    }
    return 1;
}

unsigned char _mech_move(mech* mech, int direction, int distance) {
    if(mech == 0) {
        LOGWARN("_mech_walk: Passed NULL pointer for 'mech' param.");
        return 0;
    }
    if(direction == DIR_LEFT) {
        if(mech->position - distance < 0) {
            LOGWARN("_mech_walk: tried to move mech past left edge of game (from pos: %d to pos: %d).", mech->position, mech->position - distance);
            return 0;
        }
        mech->position-=distance;
        LOGINFO("_mech_walk: moved %s from position %d to position %d.", mech->name, mech->position + distance, mech->position);
        return 1;
    }
    if(direction == DIR_RIGHT) {
        if(mech->position + distance >= NUM_POSITIONS) {
            LOGWARN("_mech_walk: tried to move mech past right edge of game (from pos: %d to pos: %d).", mech->position, mech->position + distance);
            return 0;
        }
        mech->position+=distance;
        LOGINFO("_mech_walk: moved %s from position %d to position %d.", mech->name, mech->position - distance, mech->position);
        return 1;
    }
    LOGWARN("_mech_walk: Passed invalid 'direction' param: %d. Must be DIR_LEFT (%d) or DIR_RIGHT (%d).", direction, DIR_LEFT, DIR_RIGHT);
    return 0;
}
