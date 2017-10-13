
#include <SDL2/SDL.h>

#include "types.h"
#include "mappings.h"
#include "render.h"

/*
    The declarations for global state used in the game.
*/



/* Primary render configuration */
RenderConfig render_config(
    Mappings::default_palette(),
    std::make_pair<uint8_t, uint8_t>(16u, 16u)
);



/* Total ticks since game launch */
static uint32_t ticks_total = 0;

const uint32_t get_ticks_total() { return ticks_total; }

/* Ticks since last frame */
static uint32_t ticks_delta = 0;

const uint32_t get_ticks_delta() { return ticks_delta; }


static bool initialized_ticks = false;

static void init_tick_counts() {
    
    ticks_total = SDL_GetTicks();
    ticks_delta = ticks_total;
    initialized_ticks = true; // prevent another initialization later

}


void update_tick_counts() {

    if (!initialized_ticks) {
        init_tick_counts();
        return;
    }

    // update ticks variables
    uint32_t new_ticks_total = SDL_GetTicks();
    ticks_delta = new_ticks_total - ticks_total;
    ticks_total = new_ticks_total;

}


/* The primary window */
SDL_Window *window;

/* The primary renderer */
SDL_Renderer *renderer;