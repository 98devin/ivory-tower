
#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "types.h"
#include "render.h"


/*
    Declarations giving other files
    access to the global state declared and originating
    in `state.c`. If you include this file,
    they will be accessible in other code.
*/


// The size tiles will render (at 1:1 scale), regardless
// of their actual texture size.
static constexpr uint16_t TILE_RENDER_WIDTH  = 32u;
static constexpr uint16_t TILE_RENDER_HEIGHT = 32u;

// The number of tiles across and high
// the screen will be capable of rendering (at 1:1 scale)
static constexpr uint16_t SCREEN_TILES_WIDE = 48u;
static constexpr uint16_t SCREEN_TILES_HIGH = 28u;

// The height and width of the screen,
// in pixels (computed from the above variables, naturally).
static constexpr uint16_t SCREEN_WIDTH  = (TILE_RENDER_WIDTH  * SCREEN_TILES_WIDE);
static constexpr uint16_t SCREEN_HEIGHT = (TILE_RENDER_HEIGHT * SCREEN_TILES_HIGH);



// The default file to load textures from.
static constexpr const char *DEFAULT_TEXTURE_BMP = "./assets/textures/tilemap.bmp";

// The default tile size used for the above two properties
static constexpr Pair<uint8_t> DEFAULT_TILE_SIZE = { 16u, 16u };



/* Primary render configuration */
extern RenderConfig render_config;



/* Primary window for game */
extern SDL_Window   *window;

/* Primary renderer for game */
extern SDL_Renderer *renderer;

 

/* Total ticks since game launch */
uint32_t get_ticks_total();

/* Ticks since last frame */
uint32_t get_ticks_delta();

/* update the ticks for this frame */
void update_tick_counts();



