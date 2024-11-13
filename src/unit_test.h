#pragma once

//structs
typedef struct test {
    //TODO: function pointer? has boolean return (0 for test failure, 1 for test success) 
    //the test itself will just be in the function pointed to by function pointer... it'll do the whole run function and return accordingly
    //assert? idk 
    char name[1024];
    unsigned char (*test_ptr)();
} test;

typedef struct test_box {
    //TODO: we can implement this as a darray  so that there's not a cap on it
    test tests[1024];
    unsigned int num_tests;
} test_box;
//structs

//functions
unsigned char initialize_testing();
unsigned char add_test(test test);
void run_tests(); 
//functions
