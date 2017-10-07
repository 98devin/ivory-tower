
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "types.h"
#include "state.h"
#include "style.h"

/*
    The declarations for global state used in the game.
*/



/* Primary render configuration */
Render_Config render_config = {
    .palette = 
        { { 0x80, 0x80, 0x80 },      // bg1
          { 0x35, 0x97, 0x68 },      // bg2
          { 0xA7, 0xD4, 0x6F },      // fg1
          { 0xFF, 0xED, 0x8F }  },   // fg2
    .render_scale   = {  1,  1 },
};



/* Total ticks since game launch */
uint32_t ticks_total = 0;

/* Ticks since last frame */
uint32_t ticks_delta = 0;



/* The primary window */
SDL_Window *window;

/* The primary renderer */
SDL_Renderer *renderer;