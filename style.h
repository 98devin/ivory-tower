
#pragma once

#include <stdbool.h>
#include "types.h"


/*
    Preset colors, offsets, scales
*/

const Color *color_white();
const Color *color_from_palette(Palette_Layer);
const Color *color_rgb_sin();

const Offset *offset_zero();
const Offset *offset_hover();
const Offset *offset_hover_wave();

const Scale *scale_default();


/* 
    Preset styles
*/

Style style_default(void);

Style style_from_palette(Palette_Layer);

Style style_rgb_sin(void);


/*
    Create a style from components directly.
*/

Style style(const Color*, const Offset*, const Scale*);



/*
    Create a style from other styles.
*/

Style style_compose(const Style, const Style);



/*
    Create a style by mixing in properties to an
    existing style.
*/

Style style_compose_color (const Style, const Color*);

Style style_compose_offset(const Style, const Offset*);

Style style_compose_scale (const Style, const Scale*);



/*
    If necessary, re-compute the properties of the style
    (if they are dynamic) or simply copy the static values
    (if they are static) and confirm that the cache was updated
    and validated since it was last invalidated. 
*/
void ensure_valid_style(const Style *);


/*
    Iterate through all known styles and invalidate their cache
    so that it will be recomputed on the next frame. This
    will likely need to be called at the end of every frame to ensure
    proper updates.
*/
void invalidate_rendering_caches();