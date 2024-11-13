#include "events.h"
#include "darray.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//callback is a function with parameters specified in events.h
typedef struct registered_event {
    void* listener;
    event_callback callback;
} registered_event;

//all registered events for a particular event code
typedef struct event_code_entry {
    registered_event* events;
} event_code_entry;

//LORGE number
#define MAX_MESSAGE_CODES 16384

//event system's state (just a big array of event code entries; array index indicates the event code of that event code
//entry)
typedef struct event_system_state {
    event_code_entry registered[MAX_MESSAGE_CODES];
} event_system_state;

//static event system state global variable (also a boolean is_initialized that will be checked whenever the initialize event system func is called so that we don't initialize it more than once)
static event_system_state* state_ptr = 0;

unsigned char initialize_events() {
    if(state_ptr != 0) {
        printf("WARNING: Tried to initialize event system state after it was already initialized");
        return 0;
    }

    state_ptr = malloc(sizeof(event_system_state));
    memset(state_ptr, 0, sizeof(event_system_state));
    return 1;
}

void shutdown_events() {
    //TODO: destroy all darrays
    free(state_ptr);
    state_ptr = 0;
    printf("INFO: Event system state shut down");
}

unsigned char register_event (int code, void* listener, event_callback callback) {

    //if event system state hasn't been initialized, return false
    if(!state_ptr) {
        return 0;
    }

    if(state_ptr->registered[code].events == 0) {
        state_ptr->registered[code].events = darray_create(registered_event);
    }

    unsigned long long numRegistered = darray_length(state_ptr->registered[code].events);
    for(int i = 0 ; i < numRegistered ; i++) {
        if(state_ptr->registered[code].events[i].listener == listener) {
            printf("WARNING: Tried to register an event that has already been registered");
            return 0;
        }
    }

    registered_event event;
    event.listener = listener;
    event.callback = callback;
    darray_push(state_ptr->registered[code].events, event);

    return 1;
}

unsigned char unregister_event(int code, void* listener, event_callback callback) {

    
    //if event system state hasn't been initialized, return false
    if(!state_ptr) {
        return 0;
    }

    if(state_ptr->registered[code].events == 0) {
        state_ptr->registered[code].events = darray_create(registered_event);
    }

    unsigned long long numRegistered = darray_length(state_ptr->registered[code].events);
    for(int i = 0 ; i < numRegistered ; i++) {
        if(state_ptr->registered[code].events[i].listener == listener
           && state_ptr->registered[code].events[i].callback == callback) {
            registered_event temp;
            darray_pop_at(state_ptr->registered[code].events, i, &temp);
            return 1;
        }
    }

    return 0;

}

unsigned char fire_event(int code, void* sender, event_context context) {
    if(!state_ptr) {
        return 0;
    }

    if(state_ptr->registered[code].events == 0) {
        return 0;
    }

    unsigned long long numRegistered = darray_length(state_ptr->registered[code].events);
    for(int i = 0 ; i < numRegistered ; i++) {
        registered_event event = state_ptr->registered[code].events[i];
        if(event.callback(code, sender, event.listener, context)) {
            return 1;
        }
    }

    return 0;

}
