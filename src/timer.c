#include "timer.h"

#include "../include/raylib.h"


double timer_get_time(timer t) {
    return (GetTime() - t.start_time);
}

unsigned char timer_start(timer* t) {
    if(t->start_time == 0.0) {
        t->start_time = GetTime();
        return 1;
    }
    return 0; //timer already started... do nothing
}

unsigned char timer_reset(timer* t) {
    if(t->start_time == 0.0) {
        return 0; //timer already reset... do nothing
    }
    t->start_time = 0.0;
    return 1;
}

unsigned char timer_is_done(timer t) {
    return 1;
}
