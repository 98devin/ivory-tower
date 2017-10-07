
#pragma once

#include <stdbool.h>
#include "types.h"


/*
    Initialize the referenced Texture_Store
    using the bpm file provided as a texture, and
    the mapping to know where to find each texture in the image.
    The `tile_size` is used to convert from grid coordinates
    to actual pixel locations.

    returns: whether the store was initialized successfully.
*/
bool init_texture_store(
          Texture_Store   t_store,    /* texture store to initialize */
    const Texture_Mapping t_map,      /* texture mapping to use to init store */
    const char           *bmp_path,   /* name of the file to load for textures */
    const u8_pair         tile_size   /* how large each texture is in the file */
);

