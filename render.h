
#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <memory>

#include "types.h"
#include "style.h"


/* 
    A map from tile id to the coordinates
    (in tile size grid squares) within the texture
    containing the tiles where it should be found.
*/
typedef std::unordered_map<Tile, Pair<uint8_t>> TextureMapping;


/*
    A map from tile id to texture region, so that we can render tiles.
*/
class TileMap {
public:

    TileMap(const std::string &tex_file, const Pair<uint8_t> tile_size, const TextureMapping *);

    SDL_Rect region(Tile) const;

    SDL_Texture *atlas()  const;

private:

    std::unordered_map<Tile, SDL_Rect> legend;

    SDL_Texture *_atlas;

};


/* 
    A map from palette layers to colors, e.g. a palette
*/
typedef std::unordered_map<PaletteColor, const ColorRGBA> Palette;


/*
    The most basic elements needed to render a tile to the screen.
*/
struct RenderConfig {

    struct RenderInfo {
        Pair<int8_t> pos;
        Tile tile;
    };

    Palette         &palette();
    void             palette(Palette *&);
    void             palette(Palette *&&);

    const TileMap   &tile_map();
    void             tile_map(TileMap *&);
    void             tile_map(TileMap *&&);
    
    Pair<uint8_t>   render_scale;

    RenderInfo      info;

    RenderConfig(Palette &,  Pair<uint8_t> render_scale);
    RenderConfig(Palette &&, Pair<uint8_t> render_scale);
    
private:

    std::unique_ptr<Palette> _palette;
    std::unique_ptr<TileMap> _tile_map;

};


bool draw_tile(Tile, Pair<int8_t>, const Style&);

bool draw_string(const std::string&, Pair<int8_t>, const Style&);