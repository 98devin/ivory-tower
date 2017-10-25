
#include <SDL2/SDL.h>

#include "types.h"
#include "mappings.h"
#include "render.h"

/*
    The declarations for global state used in the game.
*/





/* The primary window */
SDL_Window *window;

/* The primary renderer */
SDL_Renderer *renderer;



/* Primary render configuration */
RenderConfig render_config = RenderConfig(
    *Mappings::default_palette(),
    { 16, 16 }
);



/* Total ticks since game launch */
static uint32_t ticks_total = 0;

uint32_t get_ticks_total() { return ticks_total; }

/* Ticks since last frame */
static uint32_t ticks_delta = 0;

uint32_t get_ticks_delta() { return ticks_delta; }



/* Whether the tick counters have been initialized yet */
static bool initialized_ticks = false;

/* Initialize, for the first time, the tick counters. */
static void init_tick_counts() {
    
    ticks_total = SDL_GetTicks();
    ticks_delta = ticks_total;
    initialized_ticks = true; // prevent another initialization later

}

/* Update the tick counters to reflect the passage of time. */
void update_tick_counts() {

    // make sure not to re-initialize if we already have
    if (!initialized_ticks) {
        init_tick_counts();
        return;
    }

    // update ticks variables
    uint32_t new_ticks_total = SDL_GetTicks();
    ticks_delta = new_ticks_total - ticks_total;
    ticks_total = new_ticks_total;

}

