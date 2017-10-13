
#include <SDL2/SDL.h>
#include <string.h>
#include <unordered_map>

#include "types.h"
#include "state.h"
#include "style.h"
#include "render.h"


/* Implementation of RenderConfig */

RenderConfig::RenderConfig(Palette *palette, Pair<uint8_t> render_scale) 
    : palette(palette), texture_store(new TextureStore), render_scale(render_scale) { }

RenderConfig::~RenderConfig() {
    delete texture_store;
}


/* Other rendering functions */


/* Draw a single tile `t` at the screen position `pos` with style `s` */
bool draw_tile(Tile t, Pair<int8_t> pos, const Style &s) {

    render_config.info.pos  = pos;
    render_config.info.tile = t;

    // Find the texture associated with the tile
    SDL_Texture *tex;
    try { 
        tex = render_config.texture_store->at(t);
    } catch (const std::out_of_range& e) {
        fprintf(stderr, "Error: %s retrieving texture for tile: %d \n", e.what(), (uint16_t)t);
    }

    if (tex == NULL) { 
        fprintf(stderr, "Error: texture retrieved was NULL\n");
        return false;
    }

    // Get the values relevant to rendering
    // from the style
    const ColorRGB      color  = s.color();
    const Pair<int16_t> offset = s.offset();
    const Pair<float>   scale  = s.scale();

    // Compute the position where we should
    // be drawing the tile.
    SDL_Rect dest{
        TILE_RENDER_WIDTH  * pos.first  + offset.first,  // x
        TILE_RENDER_HEIGHT * pos.second + offset.second, // y
        (int)(TILE_RENDER_WIDTH  * scale.first),         // w
        (int)(TILE_RENDER_HEIGHT * scale.second),        // h
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


bool draw_string(const std::string &str, Pair<int8_t> pos, const Style &s) {
    static std::unordered_map<char, Tile> char_to_tile {
        { 'a', Tile::ALPHANUM_A },
        { 'b', Tile::ALPHANUM_B },
        { 'c', Tile::ALPHANUM_C },
        { 'd', Tile::ALPHANUM_D },
        { 'e', Tile::ALPHANUM_E },
        { 'f', Tile::ALPHANUM_F },
        { 'g', Tile::ALPHANUM_G },
        { 'h', Tile::ALPHANUM_H },
        { 'i', Tile::ALPHANUM_I },
        { 'j', Tile::ALPHANUM_J },
        { 'k', Tile::ALPHANUM_K },
        { 'l', Tile::ALPHANUM_L },
        { 'm', Tile::ALPHANUM_M },
        { 'n', Tile::ALPHANUM_N },
        { 'o', Tile::ALPHANUM_O },
        { 'p', Tile::ALPHANUM_P },
        { 'q', Tile::ALPHANUM_Q },
        { 'r', Tile::ALPHANUM_R },
        { 's', Tile::ALPHANUM_S },
        { 't', Tile::ALPHANUM_T },
        { 'u', Tile::ALPHANUM_U },
        { 'v', Tile::ALPHANUM_V },
        { 'w', Tile::ALPHANUM_W },
        { 'x', Tile::ALPHANUM_X },
        { 'y', Tile::ALPHANUM_Y },
        { 'z', Tile::ALPHANUM_Z },

        { 'A', Tile::ALPHANUM_A_BOLD },
        { 'B', Tile::ALPHANUM_B_BOLD },
        { 'C', Tile::ALPHANUM_C_BOLD },
        { 'D', Tile::ALPHANUM_D_BOLD },
        { 'E', Tile::ALPHANUM_E_BOLD },
        { 'F', Tile::ALPHANUM_F_BOLD },
        { 'G', Tile::ALPHANUM_G_BOLD },
        { 'H', Tile::ALPHANUM_H_BOLD },
        { 'I', Tile::ALPHANUM_I_BOLD },
        { 'J', Tile::ALPHANUM_J_BOLD },
        { 'K', Tile::ALPHANUM_K_BOLD },
        { 'L', Tile::ALPHANUM_L_BOLD },
        { 'M', Tile::ALPHANUM_M_BOLD },
        { 'N', Tile::ALPHANUM_N_BOLD },
        { 'O', Tile::ALPHANUM_O_BOLD },
        { 'P', Tile::ALPHANUM_P_BOLD },
        { 'Q', Tile::ALPHANUM_Q_BOLD },
        { 'R', Tile::ALPHANUM_R_BOLD },
        { 'S', Tile::ALPHANUM_S_BOLD },
        { 'T', Tile::ALPHANUM_T_BOLD },
        { 'U', Tile::ALPHANUM_U_BOLD },
        { 'V', Tile::ALPHANUM_V_BOLD },
        { 'W', Tile::ALPHANUM_W_BOLD },
        { 'X', Tile::ALPHANUM_X_BOLD },
        { 'Y', Tile::ALPHANUM_Y_BOLD },
        { 'Z', Tile::ALPHANUM_Z_BOLD },

        { '0', Tile::ALPHANUM_0 },
        { '1', Tile::ALPHANUM_1 },
        { '2', Tile::ALPHANUM_2 },
        { '3', Tile::ALPHANUM_3 },
        { '4', Tile::ALPHANUM_4 },
        { '5', Tile::ALPHANUM_5 },
        { '6', Tile::ALPHANUM_6 },
        { '7', Tile::ALPHANUM_7 },
        { '8', Tile::ALPHANUM_8 },
        { '9', Tile::ALPHANUM_9 },

        { ' ',  Tile::PUNCT_SPACE     },
        { '.',  Tile::PUNCT_FSTOP     },
        { ',',  Tile::PUNCT_COMMA     },
        { '`',  Tile::PUNCT_LQUOT     },
        { '\'', Tile::PUNCT_RQUOT     },
        { '!',  Tile::PUNCT_EMARK     },
        { '?',  Tile::PUNCT_QMARK     },
        { ':',  Tile::PUNCT_COLON     },
        { '(',  Tile::PUNCT_LPAREN    },
        { ')',  Tile::PUNCT_RPAREN    },
        { '+',  Tile::PUNCT_PLUS      },
        { '-',  Tile::PUNCT_MINUS     },
        { '*',  Tile::PUNCT_MULTSMALL },
        { '<',  Tile::PUNCT_LTSIGN    },
        { '>',  Tile::PUNCT_GTSIGN    },
        { '/',  Tile::PUNCT_SOLIDUS   },
        { '$',  Tile::PUNCT_MONEY     },

    };

    Tile t;
    size_t str_pos{ 0 };
    Pair<int8_t> pos_offset(0, 0);


    while (str_pos < str.size()) {
        
        // Test for multi-character tokens
        if      (str.compare(str_pos, 3, "!!!") == 0) {
            t = Tile::PUNCT_EEEMARK;    
            str_pos += 3;
        } else if (str.compare(str_pos, 2, "!?") == 0) {
            t = Tile::PUNCT_EQMARK;     
            str_pos += 2;
        } else if (str.compare(str_pos, 3, "...") == 0) {
            t = Tile::PUNCT_ELLIPSIS;
            str_pos += 3;
        } else if (str.compare(str_pos, 2, "$$") == 0) {
            t = Tile::PUNCT_MONEY_BOLD;
            str_pos += 2;
        } else if (str.compare(str_pos, 2, "//") == 0) {
            t = Tile::PUNCT_DIVIDE;
            str_pos += 2;
        } else if (str.compare(str_pos, 2, "**") == 0) {
            t = Tile::PUNCT_MULTIPLY;
            str_pos += 2;
        } else if (str[str_pos] == '\n') {

            // For handling new lines, we change our offset
            // to begin drawing the next characters on a lower line.
            pos_offset.first = 0;
            pos_offset.second++;
            str_pos++;
            continue;

        } else {
            try {
                t = char_to_tile.at(str[str_pos]);
            } catch (const std::out_of_range& e) {
                fprintf(stderr, "Error printing tile %u\n", (uint16_t)t);
            }
            str_pos++;
        }
        
        if (!draw_tile(t,
            std::make_pair(pos.first + pos_offset.first, pos.second + pos_offset.second),s)
        ) return false;

        pos_offset.first++;
    }

    return true;
}