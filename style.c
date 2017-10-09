
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "types.h"
#include "state.h"
#include "sl_arr.h"
#include "style.h"


static sl_arr_t used_const_vals; // dynamic values which are not parametrized

static sl_arr_t used_dynam_vals; // dynamic values which ARE parametrized


/* 
    Ensure that the lists containing all styles utilized thus far
    have been initialized.
*/
inline static void ensure_init_val_lists() {
    if (used_const_vals == NULL)
        used_const_vals = sl_arr_init(NULL);
    if (used_dynam_vals == NULL)
        used_dynam_vals = sl_arr_init(NULL);
    //fprintf(stderr, "const_vals: %lu, dynam_vals: %lu\n", used_const_vals->length, used_dynam_vals->length);
}


/*
    Presets for colors/offsets/scales which may be useful across styles
*/

const Color *color_white() { 
    static const Color white_color = {
        .meta.type = VALUE_STATIC,
        .value = { 0xFF, 0xFF, 0xFF },
    };
    return &white_color;
}

const Offset *offset_zero() {
    static const Offset zero_offset = {
        .meta.type = VALUE_STATIC,
        .value = { 0, 0 },
    };
    return &zero_offset;
}

const Scale *scale_default() {
    static const Scale default_scale = {
        .meta.type = VALUE_STATIC,
        .value = { 1.0f, 1.0f },
    };
    return &default_scale;
}


const Color color_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (Color) {
        .meta.type = VALUE_STATIC,
        .value = { r, g, b }
    };
}

const Offset offset_xy(int16_t x, int16_t y) {
    return (Offset) {
        .meta.type = VALUE_STATIC,
        .value = { x, y }
    };
}

const Scale scale_wh(float w, float h) {
    return (Scale) {
        .meta.type = VALUE_STATIC,
        .value = { w, h }
    };
}


static const ColorRGB get_color_from_palette(void *data) {
    return render_config.palette[*(Palette_Layer*)data];
}

const Color *color_from_palette(Palette_Layer pl) {
    static Color palette_color[PALETTE_LAYER_COUNT];
    static bool  initialized  [PALETTE_LAYER_COUNT];

    if (!initialized[pl]) {
        
        Palette_Layer *data = malloc(sizeof(Palette_Layer));
        *data = pl;
        
        palette_color[pl] = (Color) {
            .meta.type = VALUE_DYNAMIC,
            .meta.valid = false,
            .data = (void *) data,
            .compute_dynamic = get_color_from_palette,
        };
        
        ensure_init_val_lists();
        sl_arr_push(used_const_vals, &palette_color[pl]);
        initialized[pl] = true;
    }

    return &palette_color[pl];
}



static const ColorRGB get_color_rgb_sin(void *data) {
    static uint8_t r, g, b;
    static const double pi_13 =      M_PI / 3;
    static const double pi_23 = 2 * (M_PI / 3);

    uint32_t ticks = SDL_GetTicks();
    
    r = (sin(ticks / 300.0)         + 1) * 127;
    g = (sin(ticks / 300.0 + pi_13) + 1) * 127;
    b = (sin(ticks / 300.0 + pi_23) + 1) * 127;

    return (ColorRGB){ r, g, b };
}

const Color *color_rgb_sin() {
    static bool initialized = false;
    static Color rgb_sin_color = {
        .meta.type = VALUE_DYNAMIC,
        .meta.valid = false,
        .data = NULL,
        .compute_dynamic = get_color_rgb_sin,  
    };
    if (!initialized) {
        ensure_init_val_lists();
        sl_arr_push(used_const_vals, &rgb_sin_color);
        initialized = true;
    }
    return &rgb_sin_color;
}


static const i16_pair get_offset_hover(void *data) {
    static int16_t h;
    
    uint32_t ticks = SDL_GetTicks();
    h = sin(ticks / 200.0) * (TILE_RENDER_HEIGHT / 4);

    return (i16_pair){ 0, h };
}

const Offset *offset_hover() {
    static bool initialized = false;
    static Offset hover_offset = {
        .meta.type = VALUE_DYNAMIC,
        .meta.valid = false,
        .data = NULL,
        .compute_dynamic = get_offset_hover,
    };
    if (!initialized) {
        ensure_init_val_lists();
        sl_arr_push(used_const_vals, &hover_offset);
        initialized = true;
    }
    return &hover_offset;
}


static const i16_pair get_offset_hover_wave(void *data) {
    static int16_t h;
    
    uint32_t ticks = SDL_GetTicks();
    h = sin(ticks / 100.0 + 100 * render_config.info.pos.x + 50 * render_config.info.pos.y) * (TILE_RENDER_HEIGHT / 4);

    return (i16_pair){ 0, h };
}

const Offset *offset_hover_wave() {
    static Offset hover_wave_offset = {
        .meta.type = VALUE_DYNAMIC_NOCACHE,
        .data = NULL,
        .compute_dynamic = get_offset_hover_wave,
    };
    return &hover_wave_offset;
}



/* Helper function to use as callback for invalidation */
static void invalidate_value(void *v) {
    ((Value *)v)->valid = false;
}

/*
    Iterate through all known dynamic values and invalidate their cache
    so that it will be recomputed on the next frame. This
    will likely need to be called at the end of every frame to ensure
    proper updates.
*/
void invalidate_rendering_caches() {
    ensure_init_val_lists();
    sl_arr_each(used_const_vals, invalidate_value);
    sl_arr_each(used_dynam_vals, invalidate_value);
}


/*
    Functions for recomputing values.
    They return whether a recomputation was necessary.
*/

static bool recompute_color(Color *c) {
    if (c->meta.type == VALUE_STATIC || c->meta.valid) return false;
    
    c->value = c->compute_dynamic(c->data);

    if (c->meta.type != VALUE_DYNAMIC_NOCACHE)
        c->meta.valid = true;
    
    return true;
}

static bool recompute_offset(Offset *o) {
    if (o->meta.type == VALUE_STATIC || o->meta.valid) return false;
    
    o->value = o->compute_dynamic(o->data);

    if (o->meta.type != VALUE_DYNAMIC_NOCACHE)
        o->meta.valid = true;
    
    return true;
}

static bool recompute_scale(Scale *s) {
    if (s->meta.type == VALUE_STATIC || s->meta.valid) return false;
    
    s->value = s->compute_dynamic(s->data);

    if (s->meta.type != VALUE_DYNAMIC_NOCACHE)
        s->meta.valid = true;
    
    return true;
}



/*

    Functions to allow for combining multiple
    dynamic colors/offsets/scales into a new dynamic
    value which is the result of multiplying or adding
    the two inputs.

    This allows us to create entire styles which are
    kind of like the compositions of two styles in one.
    Furthermore this is done in such a way that the work
    is still optimally shared.

*/


static const ColorRGB color_multiply(const ColorRGB c1, const ColorRGB c2) {
    ColorRGB c;
    c.r = (c1.r * c2.r) / 0xFF;
    c.g = (c1.g * c2.g) / 0xFF;
    c.b = (c1.b * c2.b) / 0xFF;
    return c;
}

static const ColorRGB get_combined_colors(void *data) {
    Color **cs = (Color **) data;
    recompute_color(cs[0]);
    recompute_color(cs[1]);
    return color_multiply(cs[0]->value, cs[1]->value);
}

static const Color *combine_colors(const Color *c1, const Color *c2) {
    
    if (c1 == color_white()) return c2;
    if (c2 == color_white()) return c1;

    Color *c = malloc(sizeof(Color));
    
    // If we can just use a static color, then we will.
    if (c1->meta.type == VALUE_STATIC && c2->meta.type == VALUE_STATIC) {
        c->meta.type = VALUE_STATIC;
        c->value     = color_multiply(c1->value, c2->value);
        c->data      = NULL;
        c->compute_dynamic = NULL;
        return c;
    }
    
    // Otherwise (and likely in most cases), we create a new color
    // which will dynamically represent the multiple of the two provided.

    const Color **data = malloc(2 * sizeof(Color*));
    data[0] = c1;
    data[1] = c2;

    if (c1->meta.type == VALUE_DYNAMIC_NOCACHE || c2->meta.type == VALUE_DYNAMIC_NOCACHE) {
        c->meta.type = VALUE_DYNAMIC_NOCACHE;
    } else {
        c->meta.type = VALUE_DYNAMIC;
        ensure_init_val_lists();
        sl_arr_push(used_dynam_vals, c);
    }
    
    c->data = (void *) data;
    c->compute_dynamic = get_combined_colors;

    return c;
}


static const i16_pair offset_add(const i16_pair o1, const i16_pair o2) {
    i16_pair o;
    o.x = o1.x + o2.x;
    o.y = o1.y + o2.y;
    return o;
}

static const i16_pair get_combined_offsets(void *data) {
    Offset **os = (Offset **) data;
    recompute_offset(os[0]);
    recompute_offset(os[1]);
    return offset_add(os[0]->value, os[1]->value);
}

static const Offset *combine_offsets(const Offset *o1, const Offset *o2) {
    
    if (o1 == offset_zero()) return o2;
    if (o2 == offset_zero()) return o1;

    Offset *o = malloc(sizeof(Offset));
    
    // If we can just use a static offset, then we will.
    if (o1->meta.type == VALUE_STATIC && o2->meta.type == VALUE_STATIC) {
        o->meta.type = VALUE_STATIC;
        o->value     = offset_add(o1->value, o2->value);
        o->data      = NULL;
        o->compute_dynamic = NULL;
        return o;
    }
    
    // Otherwise (and likely in most cases), we create a new offset
    // which will dynamically represent the sum of the two provided.

    const Offset **data = malloc(2 * sizeof(Offset*));
    data[0] = o1;
    data[1] = o2;

    if (o1->meta.type == VALUE_DYNAMIC_NOCACHE || o2->meta.type == VALUE_DYNAMIC_NOCACHE) {
        o->meta.type = VALUE_DYNAMIC_NOCACHE;
    } else {
        o->meta.type = VALUE_DYNAMIC;
        ensure_init_val_lists();
        sl_arr_push(used_dynam_vals, o);
    }
    
    o->data = (void *) data;
    o->compute_dynamic = get_combined_offsets;

    return o;
}


static const f32_pair scale_multiply(const f32_pair s1, const f32_pair s2) {
    f32_pair s;
    s.x = s1.x * s2.x;
    s.y = s1.y * s2.y;
    return s;
}

static const f32_pair get_combined_scales(void *data) {
    Scale **ss = (Scale **) data;
    recompute_scale(ss[0]);
    recompute_scale(ss[1]);
    return scale_multiply(ss[0]->value, ss[1]->value);
}

static const Scale *combine_scales(const Scale *s1, const Scale *s2) {
    
    if (s1 == scale_default()) return s2;
    if (s2 == scale_default()) return s1;

    Scale *s = malloc(sizeof(Scale));
    
    // If we can just use a static scale, then we will.
    if (s1->meta.type == VALUE_STATIC && s2->meta.type == VALUE_STATIC) {
        s->meta.type = VALUE_STATIC;
        s->value     = scale_multiply(s1->value, s2->value);
        s->data      = NULL;
        s->compute_dynamic = NULL;
        return s;
    }
    
    // Otherwise (and likely in most cases), we create a new offset
    // which will dynamically represent the sum of the two provided.

    const Scale **data = malloc(2 * sizeof(Scale*));
    data[0] = s1;
    data[1] = s2;


    if (s1->meta.type == VALUE_DYNAMIC_NOCACHE || s2->meta.type == VALUE_DYNAMIC_NOCACHE) {
        s->meta.type = VALUE_DYNAMIC_NOCACHE;
    } else {
        s->meta.type = VALUE_DYNAMIC;
        ensure_init_val_lists();
        sl_arr_push(used_dynam_vals, s);
    }

    s->data = (void *) data;
    s->compute_dynamic = get_combined_scales;

    return s;
}

/*
    With the above functions, we can implement this:
*/

Style style_compose(const Style s1, const Style s2) {
    return style(
        combine_colors(s1.color, s2.color),
        combine_offsets(s1.offset, s2.offset),
        combine_scales(s1.scale, s2.scale)
    );
}

Style style_compose_color(const Style s1, const Color *c) {
    return style_compose(
        s1,
        style(c, NULL, NULL)
    );
}

Style style_compose_offset(const Style s1, const Offset *o) {
    return style_compose(
        s1,
        style(NULL, o, NULL)
    );
}

Style style_compose_scale(const Style s1, const Scale *s) {
    return style_compose(
        s1,
        style(NULL, NULL, s)
    );
}



/*
    Ensure that a style is properly initialized.
*/
void ensure_valid_style(const Style *s) {
    recompute_color(  (Color  *) s->color  );
    recompute_offset( (Offset *) s->offset );
    recompute_scale(  (Scale  *) s->scale  );
}



/*
    Style presets.
*/



Style style(const Color *color, const Offset *offset, const Scale *scale) {
    return (Style){ 
        color  ?: color_white(), 
        offset ?: offset_zero(), 
        scale  ?: scale_default(), 
    };
}


/* Default style. White text, no offset, no scaling. */
Style style_default() {
    return style(
        color_white(),
        offset_zero(),
        scale_default()
    );
};


/* Style which fetches the color from the render_config's palette */
Style style_from_palette(Palette_Layer pl) {
    return style(
        color_from_palette(pl),
        offset_zero(),
        scale_default()
    );
}


Style style_rgb_sin() {
    return style(
        color_rgb_sin(),
        offset_zero(),
        scale_default()
    );
}

