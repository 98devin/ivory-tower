

#include <algorithm>
#include <vector>
#include <unordered_map>

#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include "types.h"
#include "state.h"
#include "style.h"



/*
    Implementations of the Value subclasses used
*/

std::vector<Color*> Color::dynamic_colors;

Color::Color(CachingType cache_type) : cache_type{cache_type} {
    if (cache_type == CachingType::DYNAMIC)
        Color::dynamic_colors.push_back(this);
}

Color::~Color() {
    if (cache_type == CachingType::DYNAMIC) {
        auto it = std::find(Color::dynamic_colors.begin(), Color::dynamic_colors.end(), this);
        Color::dynamic_colors.erase(it);
    }
}

void Color::invalidate_dynamic() {
    for (auto& color : Color::dynamic_colors) {
        color->invalidate();
    }
}


std::vector<Offset*> Offset::dynamic_offsets;

Offset::Offset(CachingType cache_type) : cache_type{cache_type} {
    if (cache_type == CachingType::DYNAMIC)
        Offset::dynamic_offsets.push_back(this);
}

Offset::~Offset() {
    if (cache_type == CachingType::DYNAMIC) {
        auto it = std::find(Offset::dynamic_offsets.begin(), Offset::dynamic_offsets.end(), this);
        Offset::dynamic_offsets.erase(it);
    }
}

void Offset::invalidate_dynamic() {
    for (auto& offset : Offset::dynamic_offsets) {
        offset->invalidate();
    }
}


std::vector<Scale*> Scale::dynamic_scales;

Scale::Scale(CachingType cache_type) : cache_type{cache_type} {
    if (cache_type == CachingType::DYNAMIC)
        Scale::dynamic_scales.push_back(this);
}

Scale::~Scale() {
    if (cache_type == CachingType::DYNAMIC) {
        auto it = std::find(Scale::dynamic_scales.begin(), Scale::dynamic_scales.end(), this);
        Scale::dynamic_scales.erase(it);
    }
}

void Scale::invalidate_dynamic() {
    for (auto& scale : Scale::dynamic_scales) {
        scale->invalidate();
    }
}


/*
    The Style class
*/

Style::Style(Color *color, Offset *offset, Scale *scale) 
    : c{color}, o{offset}, s{scale} { }

Style::Style(Color *color)
    : Style(color, offset_zero(), scale_default()) { }

Style::Style(Offset *offset)
    : Style(color_white(), offset, scale_default()) { }

Style::Style(Scale *scale)
    : Style(color_white(), offset_zero(), scale) { }


const ColorRGBA Style::color() const {
    return c->value();
}

const Pair<int16_t> Style::offset() const {
    return o->value();
}

const Pair<float> Style::scale() const {
    return s->value();
}


/*
    The most basic kinds of values; those which are static.
*/



class StaticColor : public Color {

    const ColorRGBA val;

public:

    StaticColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : Color(CachingType::STATIC), val(r, g, b, a) { }

    StaticColor(ColorRGBA *val) 
        : Color(CachingType::STATIC), val{*val} { }

    void invalidate() { }

    const ColorRGBA value() { return val; }

};



class StaticOffset : public Offset {

    const Pair<int16_t> val;

public:

    StaticOffset(int16_t x_offset, int16_t y_offset)
        : Offset(CachingType::STATIC), val(x_offset, y_offset) { }
    
    StaticOffset(Pair<int16_t> *val)
        : Offset(CachingType::STATIC), val{*val} { }
    
    void invalidate() { }

    const Pair<int16_t> value() { return val; }
};



class StaticScale : public Scale {

    const Pair<float> val;

public:

    StaticScale(float x_scale, float y_scale)
        : Scale(CachingType::STATIC), val(x_scale, y_scale) { }
    
    StaticScale(Pair<float> *val) 
        : Scale(CachingType::STATIC), val{*val} { }
    
    void invalidate() { }

    const Pair<float> value() { return val; }
};




/*
    Presets for colors/offsets/scales which may be useful across styles
*/

Color *color_white() { 
    static StaticColor white_color(0xFFu, 0xFFu, 0xFFu);
    return &white_color;
}

Offset *offset_zero() {
    static StaticOffset zero_offset(0, 0);
    return &zero_offset;
}

Scale *scale_default() {
    static StaticScale default_scale(1.0f, 1.0f);
    return &default_scale;
}



Color *color_from_palette(PaletteColor pc) {

    class FromPaletteColor : public Color {

        const PaletteColor pc;
        ColorRGBA color;
        bool valid;

    public:
        
        FromPaletteColor(PaletteColor pc)
            : Color(CachingType::DYNAMIC), pc(pc), color(render_config.palette->at(pc)) { }

        void invalidate() { valid = false; }

        const ColorRGBA value() {
            if (!valid) {
                color = render_config.palette->at(pc);
                valid = true;
            }
            return color;
        }

    };

    static std::unordered_map<PaletteColor, FromPaletteColor*> palette_color(static_cast<size_t>(PaletteColor::COLOR_COUNT));
    
    if (!palette_color.count(pc)) {
        palette_color.insert(std::make_pair(pc, new FromPaletteColor(pc)));
    }

    return palette_color.at(pc);
}



Color *color_rgb_sin() {
    
    class RGBSinColor : public Color {

        static ColorRGBA compute_color() {

            static uint8_t r, g, b;
            static const double pi_13 =      M_PI / 3;
            static const double pi_23 = 2 * (M_PI / 3);
        
            uint32_t ticks = get_ticks_total();
            
            r = (sin(ticks / 300.0)         + 1) * 127;
            g = (sin(ticks / 300.0 + pi_13) + 1) * 127;
            b = (sin(ticks / 300.0 + pi_23) + 1) * 127;
        
            return ColorRGBA(r, g, b);   
        }

        ColorRGBA color;
        bool valid;

    public:

        RGBSinColor()
            : Color(CachingType::DYNAMIC), color{RGBSinColor::compute_color()}, valid(true) { }

        void invalidate() { valid = false; }

        const ColorRGBA value() {
            if (!valid) {
                color = RGBSinColor::compute_color();
                valid = true;
            }
            return color;
        }

    };

    static RGBSinColor rgb_sin_color;

    return &rgb_sin_color;
}



Offset *offset_hover() {
    
    class HoverOffset : public Offset {

        static Pair<int16_t> compute_offset() {
            static int16_t h;
            
            uint32_t ticks = get_ticks_total();
            h = sin(ticks / 200.0) * (TILE_RENDER_HEIGHT / 4);
        
            return std::make_pair(0, h);
        }

        Pair<int16_t> offset;
        bool valid;

    public:

        HoverOffset()
            : Offset(CachingType::DYNAMIC), offset(HoverOffset::compute_offset()), valid(true) { }

        void invalidate() { valid = false; }

        const Pair<int16_t> value() {
            if (!valid) {
                offset = HoverOffset::compute_offset();
                valid = true;
            }
            return offset;
        }
    };

    static HoverOffset hover_offset;

    return &hover_offset;
}


Offset *offset_hover_wave() {
    
    class HoverWaveOffset : public Offset {
    public:

        HoverWaveOffset() : Offset(CachingType::DYNAMIC_NOCACHE) { }

        void invalidate() { }

        const Pair<int16_t> value() { 
            static int16_t h;
            
            uint32_t ticks = get_ticks_total();
            h = sin(ticks / 100.0 + 100 * render_config.info.pos.first + 50 * render_config.info.pos.second) * (TILE_RENDER_HEIGHT / 4);
        
            return std::make_pair(0, h);
        }

    };

    static HoverWaveOffset hover_wave_offset;

    return &hover_wave_offset;
}


/*
    Iterate through all known dynamic values and invalidate their cache
    so that it will be recomputed on the next frame. This
    will likely need to be called at the end of every frame to ensure
    proper updates.
*/
void invalidate_rendering_caches() {
    Color::invalidate_dynamic();
    Offset::invalidate_dynamic();
    Scale::invalidate_dynamic();
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

static CachingType max_caching_type(CachingType c1, CachingType c2) {
    if (c1 == CachingType::DYNAMIC_NOCACHE || c2 == CachingType::DYNAMIC_NOCACHE) {
        return CachingType::DYNAMIC_NOCACHE;
    }
    if (c1 == CachingType::STATIC && c2 == CachingType::STATIC) {
        return CachingType::STATIC;
    }
    return CachingType::DYNAMIC;
}




static const ColorRGBA color_multiply(const ColorRGBA &c1, const ColorRGBA &c2) {
    return ColorRGBA(
        (c1.r * c2.r) / 0xFF,
        (c1.g * c2.g) / 0xFF,
        (c1.b * c2.b) / 0xFF,
        (c1.a * c2.a) / 0xFF
    );
}

static Color *combine_colors(Color *c1, Color *c2) {
    
    class CombinedColor : public Color {
        
        Color *const c1;
        Color *const c2;

        ColorRGBA color;
        bool valid;

    public:

        CombinedColor(Color *c1, Color *c2)
            : Color(max_caching_type(c1->cache_type, c2->cache_type)),
            c1(c1), c2(c2), color(color_multiply(c1->value(), c2->value())), valid(true) { }

        void invalidate() {
            valid = false;
        }

        const ColorRGBA value() {
            if (!valid) {
                color = color_multiply(c1->value(), c2->value());
                valid = true;
            }
            return color;
        }

    };
    
    if (c1 == color_white()) return c2;
    if (c2 == color_white()) return c1;

    return new CombinedColor(c1, c2);
}




static const Pair<int16_t> offset_add(const Pair<int16_t> &o1, const Pair<int16_t> &o2) {
    return std::make_pair(
        o1.first + o2.first,
        o1.second + o2.second
    );
}

static Offset *combine_offsets(Offset *o1, Offset *o2) {
    
    class CombinedOffset : public Offset {
        
        Offset *const o1;
        Offset *const o2;

        Pair<int16_t> offset;
        bool valid;
        
    public:

        CombinedOffset(Offset *o1, Offset *o2)
            : Offset(max_caching_type(o1->cache_type, o2->cache_type)),
            o1(o1), o2(o2), offset(offset_add(o1->value(), o2->value())), valid(true) { }

        void invalidate() {
            valid = false;
        }

        const Pair<int16_t> value() {
            if (!valid) {
                offset = offset_add(o1->value(), o2->value());
                valid = true;
            }
            return offset;
        }

    };

    if (o1 == offset_zero()) return o2;
    if (o2 == offset_zero()) return o1;

    return new CombinedOffset(o1, o2);
}




static const Pair<float> scale_multiply(const Pair<float> &s1, const Pair<float> &s2) {
    return std::make_pair(
        s1.first  * s2.first,
        s1.second * s2.second
    );
}

static Scale *combine_scales(Scale *s1, Scale *s2) {
    
    class CombinedScale : public Scale {
        
        Scale *const s1;
        Scale *const s2;

        Pair<float> scale;
        bool valid;

    public:

        CombinedScale(Scale *s1, Scale *s2)
            : Scale(max_caching_type(s1->cache_type, s2->cache_type)),
            s1(s1), s2(s2), scale(scale_multiply(s1->value(), s2->value())), valid(true) { }

        void invalidate() {
            valid = false;
        }

        const Pair<float> value() {
            if (!valid) {
                scale = scale_multiply(s1->value(), s2->value());
                valid = true;
            }
            return scale;
        }

    };

    if (s1 == scale_default()) return s2;
    if (s2 == scale_default()) return s1;

    return new CombinedScale(s1, s2);
}


/*
    With the above functions, we can implement this:
*/

Style Style::compose(const Style &s1) {
    return Style(
        combine_colors(c, s1.c),
        combine_offsets(o, s1.o),
        combine_scales(s, s1.s)
    );
}

Style Style::compose(Color *c1) {
    return this->compose(Style(c1));
}

Style Style::compose(Offset *o1) {
    return this->compose(Style(o1));
}

Style Style::compose(Scale *s1) {
    return this->compose(Style(s1));
}




/*
    Style presets.
*/



/* Default style. White text, no offset, no scaling. */
Style style_default() {
    return Style(
        color_white(),
        offset_zero(),
        scale_default()
    );
};


/* Style which fetches the color from the render_config's palette */
Style style_from_palette(PaletteColor pl) {
    return Style(color_from_palette(pl));
}


Style style_rgb_sin() {
    return Style(color_rgb_sin());
}

