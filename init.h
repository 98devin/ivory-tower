
#pragma once

#include <string>

#include "types.h"
#include "render.h"


/*
    Make sure the main subsystems necessary to run the
    program are initialized.
*/
bool init();

/*
    Make sure the global variables which
    update every frame are set properly,
    such as `ticks_total` and `ticks_delta`
*/
void update_globals();


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
);

