
#include <SDL2/SDL.h>
#include <time.h>

#include "types.h"
#include "init.h"
#include "state.h"
#include "render.h"
#include "mappings.h"



bool init() {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }


    // initialize main window
    window = SDL_CreateWindow(
        "SDL2 Test",
         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
         SCREEN_WIDTH, SCREEN_HEIGHT,
         SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // initialize main renderer
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == nullptr) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    
    // init render_config tile map
    render_config.tile_map(new TileMap(
        DEFAULT_TEXTURE_BMP,
        DEFAULT_TILE_SIZE,
        Mappings::default_texture_map()
    ));

    // init random systems
    time_t t;
    srand((unsigned)time(&t));

    // initialize the tick counts
    update_tick_counts();

    return true;

}



/*
    Update global variables
*/
void update_globals() {

    // update ticks for this frame
    update_tick_counts();

    // invalidate values so they will be updated
    // this frame during rendering
    invalidate_rendering_caches();

}
