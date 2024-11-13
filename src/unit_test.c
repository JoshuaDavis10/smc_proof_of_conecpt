#include "unit_test.h"

#include "logger.h"
#include "mech.h"

//TODO: include headers for functions you want to test

#include <string.h>
#include <stdlib.h>

#define ASSERT(x) {\
    if(x){}        \
    else {         \
        return 0;  \
    }              \
}

static test_box* tests = 0;

//TODO: function for test function pointers here
//declare here, define at bottom
unsigned char _mech_take_damage_test(); 
unsigned char _mech_move_test(); 

//functions
unsigned char initialize_testing() {
    if(tests != 0) {
        LOGERROR("Tried to initialize testing after it had already been initialized.");
        return 0;
    }

    tests = malloc(sizeof(test_box));
    memset(tests, 0, sizeof(test_box));
    tests->num_tests = 0;

    if(tests == 0) {
        LOGERROR("Failed to initialize testing (tests == NULL)");
        return 0;
    }

    //TODO: add all tests to run here
    test t1;
    strcpy(t1.name, "_mech_take_damage() test");
    t1.test_ptr = _mech_take_damage_test;
    add_test(t1);
    test t2;
    strcpy(t2.name, "_mech_move() test");
    t2.test_ptr = _mech_move_test;
    add_test(t2);


    LOGINFO("Initialized testing.");
    return 1;
}

unsigned char add_test(test test) {
    if(tests == 0) {
        LOGERROR("Tried to add a test when tests have not been initialized.");
        return 0;
    }
    if(tests->num_tests > 1024) {
        LOGWARN("Cannot add anymore tests. Maximum number of tests has been reached.");
        return 0;
    }
    //TODO: code here
    tests->tests[tests->num_tests] = test;
    
    tests->num_tests++;
    return 1;
}

void run_tests() {
    unsigned int num_passed = 0;
    unsigned int num_failed = 0;
    for(int i = 0; i < tests->num_tests; i++) {
        LOGTEXT("\nrunning %s...", LOGBOLDBLUE, tests->tests[i].name);
        if(tests->tests[i].test_ptr()) {
            num_passed++;
            LOGTEXT("Passed!", LOGBOLDGREEN);
        }
        else {
            LOGTEXT("Failed!", LOGBOLDRED);
            num_failed++;
        }
    }
    LOGTEXT("\nPassed %u tests.", LOGBOLDGREEN, num_passed);
    LOGTEXT("Failed %u tests.", LOGBOLDRED  , num_failed);
}
//functions

//test functions
unsigned char _mech_take_damage_test() {

    //variables
    unsigned char result;
    //variables

    //data for test
    mech m;
    m.health = 100;
    //data for test

    //tests
        //passing NULL 'mech' param
    LOGTEXT("1. 'NULL' test...", LOGCYAN);
    result = _mech_take_damage(NULL, 50);
    LOGTEXT("EXPECTED: result = 0", LOGGRAY);
    LOGTEXT("RESULT:   result = %d", LOGGREY, result);
    ASSERT(result == 0);
    LOGTEXT("EXPECTED: health = 100", LOGGRAY);
    LOGTEXT("RESULT:   health = %d", LOGGREY, m.health);
    ASSERT(m.health == 100);
        //passing 0 health 'mech' param
    m.health = 0;
    LOGTEXT("2. 'zero health' test...", LOGCYAN);
    result = _mech_take_damage(&m, 50);
    LOGTEXT("EXPECTED: result = 0", LOGGRAY);
    LOGTEXT("RESULT:   result = %d", LOGGREY, result);
    ASSERT(result == 0);
    LOGTEXT("EXPECTED: health = 0", LOGGRAY);
    LOGTEXT("RESULT:   health = %d", LOGGREY, m.health);
    ASSERT(m.health == 0);
        //passing negative number for 'damage' param
    m.health = 100;
    LOGTEXT("3. 'negaitve damage' test...", LOGCYAN);
    result = _mech_take_damage(&m, -20);
    LOGTEXT("EXPECTED: result = 0", LOGGRAY);
    LOGTEXT("RESULT:   result = %d", LOGGREY, result);
    ASSERT(result == 0);
    LOGTEXT("EXPECTED: health = 100", LOGGRAY);
    LOGTEXT("RESULT:   health = %d", LOGGREY, m.health);
    ASSERT(m.health == 100);
        //dealing more damage than mech's health
    LOGTEXT("4. 'overkill' test...", LOGCYAN);
    result = _mech_take_damage(&m, 150);
    LOGTEXT("EXPECTED: result = 1", LOGGRAY);
    LOGTEXT("RESULT:   result = %d", LOGGREY, result);
    ASSERT(result == 1);
    LOGTEXT("EXPECTED: health = 0", LOGGRAY);
    LOGTEXT("RESULT:   health = %d", LOGGREY, m.health);
    ASSERT(m.health == 0);
        //non-lethal attack
    m.health = 100;    
    LOGTEXT("5. 'normal attack' test...", LOGCYAN);
    result = _mech_take_damage(&m, 47);
    LOGTEXT("EXPECTED: result = 1", LOGGRAY);
    LOGTEXT("RESULT:   result = %d", LOGGREY, result);
    ASSERT(result == 1);
    LOGTEXT("EXPECTED: health = 53", LOGGRAY);
    LOGTEXT("RESULT:   health = %d", LOGGREY, m.health);
    ASSERT(m.health == 53);
    //tests

    return 1;
}

unsigned char _mech_move_test() {
    //TODO: code here
        //NULL mech
        //invalid 'direction' param
        //right-edge hit
        //left-edge hit
        //left with negative 'distance' param
        //right with negative 'distance' param
        //mech with 0 health
    return 1;
}
//test functions
