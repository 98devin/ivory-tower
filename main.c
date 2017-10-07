
#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "types.h"
#include "state.h"
#include "render.h"
#include "init.h"
#include "mappings.h"
#include "style.h"


int main() {

    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow(
        "SDL2 Test",
         SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
         SCREEN_WIDTH, SCREEN_HEIGHT,
         SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 2;
    }


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
        return 3;
    }

    // Load textures for tiles
    init_texture_store(
        render_config.texture_store, 
        default_texture_map, 
        "assets/textures/tilemap.bmp", 
        (u8_pair){ 16, 16 }
    );

/*    {{{    */
    time_t t;
    srand((unsigned)time(&t));
    
    Tile_ID display_tiles[SCREEN_TILES_WIDE][SCREEN_TILES_HIGH];
    for (uint8_t x = 0; x < SCREEN_TILES_WIDE; x++)
    for (uint8_t y = 0; y < SCREEN_TILES_HIGH; y++)
        display_tiles[x][y] = rand() % TILE_IDS_COUNT;

    
    Style style_1 = style_rgb_sin();
    Style style_2 = style_compose(style_from_palette(PALETTE_BG1), style_1);

    Style hover   = style(NULL, offset_hover_wave(), NULL);
    Style style_3 = style_compose(style_1, hover);
    Style style_4 = style_compose(style_2, hover);

    Style styles[2][2] = { { style_1, style_2 }, 
                           { style_3, style_4 } };


/*    }}}    */

    SDL_Event e;

    // Main loop
    while (1) {

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                goto after_main_loop;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) goto after_main_loop;
                break;
            }
        }

        SDL_RenderClear(renderer);
    
/*    {{{    */
        u8_pair pos;
        for (uint8_t i = 0; i < SCREEN_TILES_WIDE; i++) {
            pos.x = i;
            for (uint8_t j = 0; j < SCREEN_TILES_HIGH; j++) {
                pos.y = j;
                draw_tile(
                    display_tiles[i][j],
                    pos,
                    styles[j >= SCREEN_TILES_HIGH / 2][i >= SCREEN_TILES_WIDE / 2]
                );
            }
        }

        for (int i = 0; i < 5; i++) {
            uint8_t x = rand() % SCREEN_TILES_WIDE;
            uint8_t y = rand() % SCREEN_TILES_HIGH;
            display_tiles[x][y] = rand() % TILE_IDS_COUNT;
        }

        invalidate_rendering_caches();
/*    }}}    */

        SDL_RenderPresent(renderer);

    }

    after_main_loop:

    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;

}
