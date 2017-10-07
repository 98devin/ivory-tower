
#include <stdbool.h>
#include "types.h"
#include "init.h"
#include "state.h"


/*
    Initialize the referenced Texture_Store
    using the bpm file provided as a texture, and
    the mapping to know where to find each texture in the image.
    The `tile_size` is used to convert from grid coordinates
    to actual pixel locations.

    returns: whether the store was initialized successfully.
*/
bool init_texture_store (
          Texture_Store   t_store,    /* texture store to initialize */
    const Texture_Mapping t_map,      /* texture mapping to use to init store */
    const char           *bmp_path,   /* name of the file to load for textures */
    const u8_pair         tile_size   /* how large each texture is in the file */
) {

    SDL_Surface *file_surface = SDL_LoadBMP(bmp_path);
    
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
    SDL_Rect src_rect = {
        .w = tile_size.x,
        .h = tile_size.y,
    };

    for (Tile_ID tile_id = 0; tile_id < TILE_IDS_COUNT; tile_id++) {

        SDL_Texture *tex = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            tile_size.x,
            tile_size.y
        );

        if (tex == NULL) {
            fprintf(stderr, "SDL_CreateTexture Error: %s", SDL_GetError());
            return false;
        }

        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

        SDL_SetRenderTarget(renderer, tex);

        // Update rectangle to be at the correct coordinates
        src_rect.x = (int)tile_size.x * t_map[tile_id].x;
        src_rect.y = (int)tile_size.y * t_map[tile_id].y;

        SDL_RenderCopy(renderer, file_texture, &src_rect, NULL);

        t_store[tile_id] = tex;

    }

    // Reset to previous render target.
    SDL_SetRenderTarget(renderer, prev_target);

    return true;

}