
#include <SDL2/SDL.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "types.h"
#include "state.h"
#include "render.h"
#include "init.h"
#include "style.h"


int main() {

    init();

/*    {{{    */
        
    Tile display_tiles[SCREEN_TILES_WIDE][SCREEN_TILES_HIGH];

    for (uint8_t x = 0; x < SCREEN_TILES_WIDE; x++)
    for (uint8_t y = 0; y < SCREEN_TILES_HIGH; y++)
        display_tiles[x][y] = Tile((uint16_t)rand() % (uint16_t)Tile::IDS_COUNT);

    
    Style darken(color_from_palette(PaletteColor::BG_1));
    Style hover(offset_hover_wave());
    Style rainbo(color_rgb_sin());

    Style style_0 = style_default();
    Style style_1 = style_0.compose(rainbo);

    Style style_2 = style_0.compose(darken);
    Style style_3 = style_1.compose(darken);

    Style style_4 = style_0.compose(hover);
    Style style_5 = style_1.compose(hover);

    Style style_6 = style_2.compose(hover);
    Style style_7 = style_3.compose(hover);

    Style styles[2][4] = { { style_0, style_2, style_1, style_3 }, 
                           { style_4, style_6, style_5, style_7 } };

/*    }}}    */


    SDL_Event e;

    // Main loop
    while (1) {

        update_globals();

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
        
        Pair<int8_t> pos;
        for (uint8_t i = 0; i < SCREEN_TILES_WIDE; i++) {
            pos.first = i;
            for (uint8_t j = 0; j < SCREEN_TILES_HIGH; j++) {
                pos.second = j;
                
                draw_tile(
                    display_tiles[i][j],
                    pos,
                    styles[j / (SCREEN_TILES_HIGH / 2)][i / (SCREEN_TILES_WIDE / 4)]
                );
                
            }
        }

        for (int i = 0; i < 5; i++) {
            uint16_t x = (uint16_t)rand() % SCREEN_TILES_WIDE;
            uint16_t y = (uint16_t)rand() % SCREEN_TILES_HIGH;
            display_tiles[x][y] = Tile((uint16_t)rand() % (uint16_t)Tile::IDS_COUNT);
        }

/*    }}}    */

        SDL_RenderPresent(renderer);

    }

    after_main_loop:

    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;

}
