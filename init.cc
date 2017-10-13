
#include <SDL2/SDL.h>

#include "types.h"
#include "init.h"
#include "state.h"
#include "render.h"
#include "mappings.h"



bool init() {

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    // initialize main window
    window = SDL_CreateWindow(
        "SDL2 Test",
         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
         SCREEN_WIDTH, SCREEN_HEIGHT,
         SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // initialize main renderer
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }


    // Load textures for tiles
    if (!init_texture_store(
        render_config.texture_store, 
        Mappings::default_texture_map(),
        "assets/textures/tilemap.bmp", 
        std::make_pair(16, 16)
    )) return false;


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


/*
    Initialize the referenced Texture_Store
    using the bpm file provided as a texture, and
    the mapping to know where to find each texture in the image.
    The `tile_size` is used to convert from grid coordinates
    to actual pixel locations.

    returns: whether the store was initialized successfully.
*/
bool init_texture_store(
          TextureStore   *t_store,    /* texture store to initialize */
    const TextureMapping *t_map,      /* texture mapping to use to init store */
    const std::string     bmp_path,   /* name of the file to load for textures */
    const Pair<uint8_t>   tile_size   /* how large each texture is in the file */
) {

    SDL_Surface *file_surface = SDL_LoadBMP(bmp_path.c_str());
    
    if (file_surface == NULL) {
        fprintf(stderr, "SDL_LoadBMP Error: %s", SDL_GetError());
        return false;
    }

    SDL_Texture *file_texture = SDL_CreateTextureFromSurface(renderer, file_surface);

    if (file_texture == NULL) {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s", SDL_GetError());
        return false;
    }

    SDL_FreeSurface(file_surface);

    // Maintain the previous render target so we can reset it for convenience
    SDL_Texture *prev_target = SDL_GetRenderTarget(renderer);


    // Declare rect to use as view of the file texture 
    SDL_Rect src_rect{ 0, 0, tile_size.first, tile_size.second };


    for (auto it = t_map->begin(); it != t_map->end(); ++it) {

        const auto [tile, pos] = *it;

        SDL_Texture *tex = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            tile_size.first,
            tile_size.second
        );

        if (tex == NULL) {
            fprintf(stderr, "SDL_CreateTexture Error: %s", SDL_GetError());
            return false;
        }

        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(renderer, tex);

        // Update rectangle to be at the correct coordinates
        src_rect.x = tile_size.first  * pos.first;
        src_rect.y = tile_size.second * pos.second;

        SDL_RenderCopy(renderer, file_texture, &src_rect, NULL);

        t_store->emplace(tile, tex);

    }

    // Reset to previous render target.
    SDL_SetRenderTarget(renderer, prev_target);

    return true;

}