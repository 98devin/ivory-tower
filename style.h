
#pragma once

#include <vector>
#include "types.h"



/*
    A type defining what kind of caching a particular
    value uses.
*/
enum class CachingType : uint16_t {
    STATIC,         // no caching since value is static
    DYNAMIC,        // caching, value changes per frame
    DYNAMIC_NOCACHE // no caching since value can change per tile
};



/* 
    Type of a generic `Color` being either
    a static ColorRGB value or a dynamic
    RGB color value.
*/
class Color : public Value<ColorRGBA> {

    static std::vector<Color*> dynamic_colors;
    
public:

    Color(CachingType);
    virtual ~Color();

    static void invalidate_dynamic();
    
    virtual void invalidate() = 0;

    const CachingType cache_type;
    
};

/* 
    The most simple instance of the Color class.
*/
class StaticColor : public Color {
    
    const ColorRGBA val;

public:

    StaticColor(uint8_t, uint8_t, uint8_t, uint8_t);
    StaticColor(ColorRGBA *);

    void invalidate() override;

    const ColorRGBA value() override;

};



/*
    Type of a generic `Offset` being either
    a static pair of x,y offsets or a dynamic offset value.
*/
class Offset : public Value<Pair<int16_t>> {

    static std::vector<Offset*> dynamic_offsets;

public:
    
    Offset(CachingType);
    virtual ~Offset();

    static void invalidate_dynamic();
    
    virtual void invalidate() = 0;

    const CachingType cache_type;
    
};

/*
    The most simple instance of the Offset class.
*/
class StaticOffset : public Offset {
    
    const Pair<int16_t> val;

public:

    StaticOffset(int16_t x_offset, int16_t y_offset);
    StaticOffset(Pair<int16_t> *);

    void invalidate() override;

    const Pair<int16_t> value() override;

};
    


/*
    Type of a `Scale` being either
    a static pair of x, y scale factors or a dynamic value.
*/
class Scale : public Value<Pair<float>> {

    static std::vector<Scale*> dynamic_scales;
    
public:
    
    Scale(CachingType);
    virtual ~Scale();

    static void invalidate_dynamic();
    
    virtual void invalidate() = 0;

    const CachingType cache_type;

};
  
/*
    The most simple instance of the Scale class.
*/
class StaticScale : public Scale {
    
    const Pair<float> val;

public:

    StaticScale(float x_scale, float y_scale);
    StaticScale(Pair<float> *);
    
    void invalidate() override;

    const Pair<float> value() override;

};



/*
    Information meant to accompany a Renderer_Config to
    render a tile with a more specific color, position, or size.
    Incorporates a cache so that we can avoid calling the
    dynamic value calculation functions (if they are present) more
    than once per style, since they may be potentially expensive.
*/
class Style {

    Color  *const c;
    Offset *const o;
    Scale  *const s;

public:

    Style(Color*,  Offset*,  Scale*);

    Style(Color*);
    Style(Offset*);
    Style(Scale*);

    const ColorRGBA     color()  const;
    const Pair<int16_t> offset() const;
    const Pair<float>   scale()  const;

    Style compose(const Style&);

    Style compose(Color *);
    
    Style compose(Offset *);
    
    Style compose(Scale *);

};



/*
    Preset colors, offsets, scales
*/

Color *color_white();
Color *color_from_palette(PaletteColor);
Color *color_rgb_sin();

Offset *offset_zero();
Offset *offset_hover();
Offset *offset_hover_wave();

Scale *scale_default();


/* 
    Preset styles
*/

Style style_default(void);

Style style_from_palette(PaletteColor);

Style style_rgb_sin(void);


/*
    Iterate through all known styles and invalidate their cache
    so that it will be recomputed on the next frame. This
    will likely need to be called at the end of every frame to ensure
    proper updates.
*/
void invalidate_rendering_caches();

