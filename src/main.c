#include "logger.h"
#include "unit_test.h"

#include "mech.h"
#include "game_core.h"
#include "terminal_game.h"
#include "graphical_game.h"

#include <string.h>

int main() {

//unit testing
/*
    if(!initialize_testing()) {
        LOGTEXT("Failed to initialize testing.", LOGBOLDRED);
        return -1;
    }

    run_tests();
*/
//NOTE: unit tests are set up! any mech.c (and honestly game_core.c) functions that get added, add a test for them
//unit testing

//game
    if(!run_g_game()) {
        LOGERROR("Game ran with errors.");
        return -1;
    }
//game

    return 0;
}
