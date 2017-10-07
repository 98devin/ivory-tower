
#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "types.h"


/*
    Declarations giving other files
    access to the global state declared and originating
    in `state.c`. If you include this file,
    they will be accessible in other code.
*/


// The size tiles will render (at 1:1 scale), regardless
// of their actual texture size.
#define TILE_RENDER_WIDTH  32u
#define TILE_RENDER_HEIGHT 32u

// The number of tiles across and high
// the screen will be capable of rendering (at 1:1 scale)
#define SCREEN_TILES_WIDE 48u
#define SCREEN_TILES_HIGH 27u

// The height and width of the screen,
// in pixels (computed from the above variables, naturally).
#define SCREEN_WIDTH  (TILE_RENDER_WIDTH  * SCREEN_TILES_WIDE)
#define SCREEN_HEIGHT (TILE_RENDER_HEIGHT * SCREEN_TILES_HIGH)



/* Primary render configuration */
extern Render_Config render_config;



/* Total ticks since game launch */
extern uint32_t ticks_total;

/* Ticks since last frame */
extern uint32_t ticks_delta;



/* Primary window for game */
extern SDL_Window   *window;

/* Primary renderer for game */
extern SDL_Renderer *renderer;

