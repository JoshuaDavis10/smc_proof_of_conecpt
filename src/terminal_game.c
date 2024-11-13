#include "terminal_game.h"

#include "game_core.h"
#include "logger.h"

#include <stdio.h>

#define NUM_ACTIONS_ON_FIRST_TURN 1
#define NUM_ACTIONS_PER_TURN 2

//enums
enum {
    P1_WIN,
    P2_WIN,
    CONTINUE_GAME,
    ERROR
    // ^ return values for check_game() and run_turn() functions
};
//enums

//structs
typedef struct command_flags {
    unsigned char r;
    unsigned char j;
    unsigned char one;
    unsigned char two;
} command_flags;
//structs

//helper functions

//if return 0, continue; if return 1, we're good, if return -1 we have an error and have to stop
unsigned char wwalk(int mech_ID, int direction) {
    //TODO: should we change this 'L' thing?? i feel like we should start over with graphical game from scratch instead of trying to port these over
    //since the logic is very much geared toward the terminal version of the game... maybe just use it as reference?
    //every time u try to make this function you realize that it's basically just a wrapper for mech_move so there's really no point
    if(direction == 'L') {
        if(!mech_move(mech_ID, DIR_LEFT, 1)) {
            LOGWARN("couldn't move left. See output from _mech_move above ^");
            return 0;
        }
    }
    else if(direction == 'R') {
        if(!mech_move(mech_ID, DIR_RIGHT, 1)) {
            LOGWARN("couldn't move right. See output from _mech_move above ^");
            return 0;
        }
    }
    else {
        LOGWARN("invalid movement direction indicator: %c (must be R or L)", direction);
        return 0;
    }

    return 1;
}

int check_game() {
    int p1_health = mech_get_health(P1);
    int p2_health = mech_get_health(P2);
    if(p1_health == -1 || p2_health == -1) {
        return ERROR;
    }
    if(p1_health == 0) {
        return P2_WIN;
    }
    if(p2_health == 0) {
        return P1_WIN;
    }
    return CONTINUE_GAME;
}

//TODO: refactor -> cleaner output, there's better/cleaner ways to do a lot of the things for sure
//TODO: random things I noticed/fixes to do:
// - mechs should not be able to move past each other (?)
int run_turn(unsigned int actions) {

    int turn = get_turn();
    int enemy_num = -1;
    if(turn == P1) { 
        enemy_num = P2;
    }
    if(turn == P2) {
        enemy_num = P1;
    }
    if(turn == -1) {
        LOGERROR("run_turn: invalid 'turn' value error.");
        return ERROR;       
    }
    
    unsigned int num_actions = actions;
    mech player = mech_get_info(turn);
    mech enemy = mech_get_info(enemy_num);

    while(num_actions > 0) {

        player = mech_get_info(turn);
        enemy = mech_get_info(enemy_num);

        command_flags cflags;
        cflags.j = 0;
        cflags.r = 0;
        cflags.one = 0;
        cflags.two = 0;

        //print available actions
        if(turn == P1) {
            LOGTEXT("%s (P1)'s turn...", LOGBOLDBLUE, player.name);
        }
        if(turn == P2) {
            LOGTEXT("%s (P2)'s turn...", LOGBOLDPINK, player.name);
        }
        LOGTEXT("Current player is at position %d. Enemy player is at position %d.", LOGWHITE, player.position, enemy.position);
        LOGTEXT("Actions left: %u", LOGWHITE, num_actions);
        LOGTEXT("Available commands:", LOGWHITE);
        LOGTEXT("w - walk (move 1 space)", LOGWHITE);
        if(player.legs.jump == 1) {
            LOGTEXT("j - jump (move 2 spaces)", LOGWHITE);
            cflags.j = 1;
        }
        if(player.legs.roll== 1) {
            LOGTEXT("r - roll (move 1-3 spaces)", LOGWHITE);
            cflags.r = 1;
        }
        if(player.weapon[0].uses > 0) {
            int distance = player.position - enemy.position;
            if(distance < 0)
                distance = -distance;

            if(distance <= player.weapon[0].range) {
                LOGTEXT("1 - use weapon 1 (%d damage, %d range, %d remaining uses)", 
                        LOGWHITE, player.weapon[0].damage, player.weapon[0].range, player.weapon[0].uses);
                cflags.one = 1;
            }
            else {
                LOGTEXT("1 - use weapon 1 (%d damage, %d range, %d remaining uses) -- OUT OF RANGE! CANNOT USE THIS COMMAND NOW!", 
                        LOGWHITE, player.weapon[0].damage, player.weapon[0].range, player.weapon[0].uses);
            }
        }

        if(player.weapon[1].uses > 0) {
            int distance = player.position - enemy.position;
            if(distance < 0)
                distance = -distance;

            if(distance <= player.weapon[1].range) {
                LOGTEXT("2 - use weapon 2 (%d damage, %d range, %d remaining uses)", 
                        LOGWHITE, player.weapon[1].damage, player.weapon[1].range, player.weapon[1].uses);
                cflags.two = 1;
            }
            else {
                LOGTEXT("2 - use weapon 2 (%d damage, %d range, %d remaining uses) -- OUT OF RANGE! CANNOT USE THIS COMMAND NOW!", 
                        LOGWHITE, player.weapon[1].damage, player.weapon[1].range, player.weapon[1].uses);
            }
        }
        //print available actions

        //get player input (selected action)
        LOGTEXT("Input your command:", LOGWHITE);
        char input;
        if(!scanf(" %c", &input)) {
            LOGERROR("run_turn: scanf failed.");
            return ERROR;
        }
        LOGINFO("You entered command: %c", input); 
        //get player input (selected action)

        //TODO: break these into functions (take out the if(input == 'x')), just the stuff in the block
        //'w' command
        if(input == 'w') {
            LOGTEXT("Enter direction to walk (L or R):", LOGWHITE);

            char direction;
            if(!scanf(" %c", &direction)) {
                LOGERROR("walk: scanf failed.");
                return ERROR;
            }

            LOGINFO("You entered direction: %c", direction); 

            int result = wwalk(turn, direction);
            if(result == 0) {
                continue;
            }
        }
        //'w' command

        //'j' command
        else if(input == 'j') {
            if(cflags.j == 0) {
                LOGWARN("'j' is an invalid command this turn. Enter a different command");
                continue;
            }

            LOGTEXT("Enter direction to jump (L or R):", LOGWHITE);

            char direction;
            if(!scanf(" %c", &direction)) {
                LOGERROR("run_turn: scanf failed.");
                return ERROR;
            }

            LOGINFO("You entered direction: %c", direction); 

                //
            if(direction == 'L') {
                if(!mech_move(turn, DIR_LEFT, 2)) {
                    LOGWARN("couldn't move left. See output from _mech_move above ^");
                    continue;
                }
            }
            else if(direction == 'R') {
                if(!mech_move(turn, DIR_RIGHT, 2)) {
                    LOGWARN("couldn't move right. See output from _mech_move above ^");
                    continue;
                }
            }
            else {
                LOGWARN("invalid movement direction indicator: %c (must be R or L)", direction);
                continue;
            }
                //
        }
        //'j' command

        //'r' command
        else if(input == 'r') {
            if(cflags.r == 0) {
                LOGWARN("'r' is an invalid command this turn. Enter a different command");
                continue;
            }
            LOGTEXT("Enter direction to roll (L or R):", LOGWHITE);

            char direction;
            if(!scanf(" %c", &direction)) {
                LOGERROR("run_turn: scanf failed.");
                return ERROR;
            }

            LOGINFO("You entered direction: %c", direction); 
            if(direction == 'L') {
                LOGTEXT("Enter distance to roll (1, 2, or 3):", LOGWHITE);
                int distance;
                if(!scanf("%d", &distance)) {
                    LOGERROR("run_turn: scanf failed.");
                    return ERROR;
                }
                if(distance < 1 && distance > 3) {
                    LOGWARN("cannot roll distance: %d. Must be 1, 2, or 3", distance);
                    continue;
                }
                if(!mech_move(turn, DIR_LEFT, distance)) {
                    LOGWARN("couldn't move right. See output from _mech_move above ^");
                    continue;
                }
            }
            else if(direction == 'R') {
                LOGTEXT("Enter distance to roll (1, 2, or 3):", LOGWHITE);
                int distance;
                if(!scanf("%d", &distance)) {
                    LOGERROR("run_turn: scanf failed.");
                    return ERROR;
                }
                if(distance < 1 && distance > 3) {
                    LOGWARN("cannot roll distance: %d. Must be 1, 2, or 3", distance);
                    continue;
                }
                if(!mech_move(turn, DIR_RIGHT, distance)) {
                    LOGWARN("couldn't move right. See output from _mech_move above ^");
                    continue;
                }
            }
            else {
                LOGWARN("invalid movement direction indicator: %c (must be R or L)", direction);
                continue;
            }
        }
        //'r' command

        //'1' command
        else if(input == '1') {
            if(cflags.one == 0) {
                LOGWARN("'1' is an invalid command this turn. Enter a different command");
                continue;
            }
            LOGTEXT("Firing weapon 1...", LOGWHITE);
            int damage = player.weapon[0].damage;
            if(!mech_damage(enemy_num, damage)) {
                LOGWARN("couldn't carry out attack. See output from _mech_take_damage above ^");
            }
            mech_use_weapon(turn, 1);
        }
        //'1' command

        //'2' command
        else if(input == '2') {
            if(cflags.two == 0) {
                LOGWARN("'2' is an invalid command this turn. Enter a different command");
                continue;
            }
            LOGTEXT("Firing weapon 2...", LOGWHITE);
            int damage = player.weapon[1].damage;
            if(!mech_damage(enemy_num, damage)) {
                LOGWARN("couldn't carry out attack. See output from _mech_take_damage above ^");
            }
            mech_use_weapon(turn, 2);
        }
        //'2' command
        //TODO: break these into functions (take out the if(input == 'x')), just the stuff in the block

        //invalid command
        else {
            LOGWARN("Invalid command: %c", input);
            continue;
        }
        //invalid command

        int action_result = check_game();
        if(action_result == ERROR) {
            LOGERROR("run_turn: check_game returned ERROR.");
            return ERROR;
        }
        if(action_result == P1_WIN) {
            return P1_WIN;
        }
        if(action_result == P2_WIN) {
            return P2_WIN;
        }
        num_actions--;
    }

    if(!switch_turn()) {
        LOGERROR("run_turn: switch_turn returned 0."); 
        return ERROR;
    }
    LOGINFO("ran turn.");
    return CONTINUE_GAME;
}
//helper functions

//game loop
unsigned char run_game() {
    if(!initialize_game_state()) {
        LOGERROR("Failed to initialize game state.");
        return 0;
    }

    //print mech info for both mechs (for debugging purposes)
    mech mech_p1 = mech_get_info(P1);
    mech mech_p2 = mech_get_info(P2);
    //TODO: print mech info...
    LOGINFO("\nMech 1 info:\nname: %s\nhealth: %d\nposition: %d\nlegs:\n- jump: %d\n- roll: %d\nweapon 1:\n- damage: %d\n- range: %d\n- uses: %d\nweapon 2:\n- damage: %d\n- range: %d\n- uses: %d",
            mech_p1.name, mech_p1.health, mech_p1.position, mech_p1.legs.jump, mech_p1.legs.roll, mech_p1.weapon[0].damage, mech_p1.weapon[0].range,
            mech_p1.weapon[0].uses, mech_p1.weapon[1].damage, mech_p1.weapon[1].range, mech_p1.weapon[1].uses);
    LOGINFO("\nMech 2 info:\nname: %s\nhealth: %d\nposition: %d\nlegs:\n- jump: %d\n- roll: %d\nweapon 1:\n- damage: %d\n- range: %d\n- uses: %d\nweapon 2:\n- damage: %d\n- range: %d\n- uses: %d",
            mech_p2.name, mech_p2.health, mech_p2.position, mech_p2.legs.jump, mech_p2.legs.roll, mech_p2.weapon[0].damage, mech_p2.weapon[0].range,
            mech_p2.weapon[0].uses, mech_p2.weapon[1].damage, mech_p2.weapon[1].range, mech_p2.weapon[1].uses);
    
    //print turn at start (for debugging purposes)
    int turn = get_turn();
    if(turn == P1) {
        LOGINFO("Turn is P1.");
    }
    else if(turn == P2) {
        LOGINFO("Turn is P2.");
    }
    else {
        LOGINFO("Turn is %d.", turn);
    }


    int turn_ret_value = run_turn(NUM_ACTIONS_ON_FIRST_TURN);

    //actual game loop
    while(turn_ret_value != P1_WIN && turn_ret_value != P2_WIN && turn_ret_value != ERROR) {
        turn_ret_value = run_turn(NUM_ACTIONS_PER_TURN);
    }
    //actual game loop

    if(turn_ret_value == ERROR) {
        LOGERROR("run_turn() terminated game loop by returning 'ERROR'");
        return 0;
    }
    if(turn_ret_value == P1_WIN) {
        LOGINFO("Player 1 wins!"); 
        return 1;
    }
    if(turn_ret_value == P2_WIN) {
        LOGINFO("Player 2 wins!"); 
        return 1;
    }

    LOGERROR("run_game: somehow reached end of function without turn_ret_value equaling ERROR, P1_WIN, OR P2_WIN");
    return 0;
}
//game loop
