
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "state.h"
#include "style.h"


bool draw_tile(Tile_ID t, i8_pair pos, Style s) {

    render_config.info.pos  = pos;
    render_config.info.tile = t;

    // Find the texture associated with the tile
    SDL_Texture *tex = render_config.texture_store[t];

    if (tex == NULL) { 
        fprintf(stderr, "error in draw_tile; null texture.\n");
        return false;
    }

    // If the cache has not yet been computed,
    // we will do that now.
    ensure_valid_style(&s);
    
    ColorRGB color  = s.color->value;
    i16_pair offset = s.offset->value;
    f32_pair scale  = s.scale->value;

    // Compute the position where we should
    // be drawing the tile.
    SDL_Rect dest = {
        .x = TILE_RENDER_WIDTH  * pos.x + offset.x,
        .y = TILE_RENDER_HEIGHT * pos.y + offset.y,
        .w = TILE_RENDER_WIDTH  * scale.x,
        .h = TILE_RENDER_HEIGHT * scale.y,
    };

    // Set the color modifier so we use the correct color
    SDL_SetTextureColorMod(tex, color.r, color.g, color.b);

    // Perform the draw
    if (SDL_RenderCopy(renderer, tex, NULL, &dest) < 0) {
        fprintf(stderr, "SDL_RenderCopy Error: %s", SDL_GetError());
        return false;
    }

    return true;

}