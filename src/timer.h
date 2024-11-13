#pragma once

typedef struct timer {
    double start_time;
    double duration; //useful if tracking a countdown (or at least that's my hope lol)
} timer;

double timer_get_time(timer t); 
unsigned char timer_start(timer* t); //starts timer (sets start_time to current time)
unsigned char timer_reset(timer* t); //set start time back to 0.0 (i.e. timer is not counting)
unsigned char timer_is_done(timer t); //i.e. is current time - start_time > duration
