
#pragma once

#include <SDL2/SDL.h>

#include "types.h"
#include "style.h"


/* 
    A map from tile id to the coordinates
    (in tile size grid squares) within the texture
    containing the tiles where it should be found.
*/
typedef std::unordered_map<Tile, Pair<uint8_t>> TextureMapping;


/*
    A map from tile id to texture, so that we can render tiles.
*/
typedef std::unordered_map<Tile, SDL_Texture *> TextureStore;


/* 
    A map from palette layers to colors, e.g. a palette
*/
typedef std::unordered_map<PaletteColor, const ColorRGB> Palette;


/*
    The most basic elements needed to render a tile to the screen.
*/
struct RenderConfig {

    struct RenderInfo {
        Pair<int8_t> pos;
        Tile tile;
    };

    Palette         *const palette;
    TextureStore    *const texture_store;
    Pair<uint8_t>   render_scale;
    RenderInfo      info;

    
    RenderConfig(Palette *, Pair<uint8_t> render_scale);

    ~RenderConfig();

};


bool draw_tile(Tile, Pair<int8_t>, const Style&);

bool draw_string(const std::string&, Pair<int8_t>, const Style&);