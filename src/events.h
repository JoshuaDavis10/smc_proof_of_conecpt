#pragma once

typedef struct event_context {
	unsigned int data[3];
} event_context;

typedef unsigned char (*event_callback)(int code, void* sender, void* listener, event_context context);

unsigned char initialize_events(); 
void shutdown_events(); 

unsigned char register_event  (int code, void* listener, event_callback callback);
unsigned char unregister_event(int code, void* listener, event_callback callback);
unsigned char fire_event(int code, void* sender, event_context context);

typedef enum event_code {
	MOUSE_CLICK,
    GAME_CORE_WALK,
    GAME_CORE_JUMP,
    GAME_CORE_ROLL,
    GAME_CORE_FIRE_WEAPON,
    MOVE_ANIMATION_STARTED,
    ATTACK_ANIMATION_STARTED,
    ENABLE_INPUT
} event_code;
