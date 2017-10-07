
#pragma once

#include <SDL2/SDL_render.h>
#include <stdbool.h>



/* Basic pair types useful for coordinates, etc. */
typedef struct u8_pair {
    uint8_t x, y;
} u8_pair;

typedef struct u16_pair {
    uint16_t x, y;
} u16_pair;

typedef struct i8_pair {
    int8_t x, y;
} i8_pair;

typedef struct i16_pair {
    int16_t x, y;
} i16_pair;

typedef struct f32_pair {
    float x, y;
} f32_pair;


/* The enumeration of each tile we can render */
typedef enum Tile_ID {

    TILE_PLAIN_FILL,

    TILE_BORDER_N,
    TILE_BORDER_S,
    TILE_BORDER_E,
    TILE_BORDER_W,
    TILE_BORDER_NW,
    TILE_BORDER_NE,
    TILE_BORDER_SW,
    TILE_BORDER_SE,
    TILE_BORDER_NS,
    TILE_BORDER_WE,
    TILE_BORDER_NSW,
    TILE_BORDER_NSE,
    TILE_BORDER_SWE,
    TILE_BORDER_NWE,
    TILE_BORDER_NSWE,
    TILE_BORDER_NONE,

    TILE_BORDER_N_BOLD,
    TILE_BORDER_S_BOLD,
    TILE_BORDER_E_BOLD,
    TILE_BORDER_W_BOLD,
    TILE_BORDER_NW_BOLD,
    TILE_BORDER_NE_BOLD,
    TILE_BORDER_SW_BOLD,
    TILE_BORDER_SE_BOLD,
    TILE_BORDER_NS_BOLD,
    TILE_BORDER_WE_BOLD,
    TILE_BORDER_NSW_BOLD,
    TILE_BORDER_NSE_BOLD,
    TILE_BORDER_SWE_BOLD,
    TILE_BORDER_NWE_BOLD,
    TILE_BORDER_NSWE_BOLD,
    TILE_BORDER_NONE_BOLD,

    TILE_BORDER_ROUND_N,
    TILE_BORDER_ROUND_S,
    TILE_BORDER_ROUND_E,
    TILE_BORDER_ROUND_W,
    TILE_BORDER_ROUND_NW,
    TILE_BORDER_ROUND_NE,
    TILE_BORDER_ROUND_SW,
    TILE_BORDER_ROUND_SE,
    TILE_BORDER_ROUND_NS,
    TILE_BORDER_ROUND_WE,
    TILE_BORDER_ROUND_NSW,
    TILE_BORDER_ROUND_NSE,
    TILE_BORDER_ROUND_SWE,
    TILE_BORDER_ROUND_NWE,
    TILE_BORDER_ROUND_NSWE,
    TILE_BORDER_ROUND_NONE,

    TILE_BORDER_ROUND_N_BOLD,
    TILE_BORDER_ROUND_S_BOLD,
    TILE_BORDER_ROUND_E_BOLD,
    TILE_BORDER_ROUND_W_BOLD,
    TILE_BORDER_ROUND_NW_BOLD,
    TILE_BORDER_ROUND_NE_BOLD,
    TILE_BORDER_ROUND_SW_BOLD,
    TILE_BORDER_ROUND_SE_BOLD,
    TILE_BORDER_ROUND_NS_BOLD,
    TILE_BORDER_ROUND_WE_BOLD,
    TILE_BORDER_ROUND_NSW_BOLD,
    TILE_BORDER_ROUND_NSE_BOLD,
    TILE_BORDER_ROUND_SWE_BOLD,
    TILE_BORDER_ROUND_NWE_BOLD,
    TILE_BORDER_ROUND_NSWE_BOLD,
    TILE_BORDER_ROUND_NONE_BOLD,

    TILE_PUNCT_SPACE,       // 
    TILE_PUNCT_FSTOP,       // .
    TILE_PUNCT_COMMA,       // ,
    TILE_PUNCT_LQUOT,       // “
    TILE_PUNCT_RQUOT,       // ”
    TILE_PUNCT_EMARK,       // !
    TILE_PUNCT_QMARK,       // ?
    TILE_PUNCT_ELLIPSIS,    // ...
    TILE_PUNCT_COLON,       // :
    TILE_PUNCT_LPAREN,      // (
    TILE_PUNCT_RPAREN,      // )
    TILE_PUNCT_EEEMARK,     // !!!
    TILE_PUNCT_EQMARK,      // !?
    TILE_PUNCT_PLUS,        // +
    TILE_PUNCT_MINUS,       // -
    TILE_PUNCT_DIVIDE,      // ÷
    TILE_PUNCT_MULTIPLY,    // ✖
    TILE_PUNCT_LTSIGN,      // <
    TILE_PUNCT_GTSIGN,      // >
    TILE_PUNCT_SOLIDUS,     // /
    TILE_PUNCT_MULTSMALL,   // ×
    TILE_PUNCT_MONEY,       // $
    TILE_PUNCT_MONEY_BOLD,  
    
    TILE_ALPHANUM_A,
    TILE_ALPHANUM_B,
    TILE_ALPHANUM_C,
    TILE_ALPHANUM_D,
    TILE_ALPHANUM_E,
    TILE_ALPHANUM_F,
    TILE_ALPHANUM_G,
    TILE_ALPHANUM_H,
    TILE_ALPHANUM_I,
    TILE_ALPHANUM_J,
    TILE_ALPHANUM_K,
    TILE_ALPHANUM_L,
    TILE_ALPHANUM_M,
    TILE_ALPHANUM_N,
    TILE_ALPHANUM_O,
    TILE_ALPHANUM_P,
    TILE_ALPHANUM_Q,
    TILE_ALPHANUM_R,
    TILE_ALPHANUM_S,
    TILE_ALPHANUM_T,
    TILE_ALPHANUM_U,
    TILE_ALPHANUM_V,
    TILE_ALPHANUM_W,
    TILE_ALPHANUM_X,
    TILE_ALPHANUM_Y,
    TILE_ALPHANUM_Z,
    
    TILE_ALPHANUM_A_BOLD,
    TILE_ALPHANUM_B_BOLD,
    TILE_ALPHANUM_C_BOLD,
    TILE_ALPHANUM_D_BOLD,
    TILE_ALPHANUM_E_BOLD,
    TILE_ALPHANUM_F_BOLD,
    TILE_ALPHANUM_G_BOLD,
    TILE_ALPHANUM_H_BOLD,
    TILE_ALPHANUM_I_BOLD,
    TILE_ALPHANUM_J_BOLD,
    TILE_ALPHANUM_K_BOLD,
    TILE_ALPHANUM_L_BOLD,
    TILE_ALPHANUM_M_BOLD,
    TILE_ALPHANUM_N_BOLD,
    TILE_ALPHANUM_O_BOLD,
    TILE_ALPHANUM_P_BOLD,
    TILE_ALPHANUM_Q_BOLD,
    TILE_ALPHANUM_R_BOLD,
    TILE_ALPHANUM_S_BOLD,
    TILE_ALPHANUM_T_BOLD,
    TILE_ALPHANUM_U_BOLD,
    TILE_ALPHANUM_V_BOLD,
    TILE_ALPHANUM_W_BOLD,
    TILE_ALPHANUM_X_BOLD,
    TILE_ALPHANUM_Y_BOLD,
    TILE_ALPHANUM_Z_BOLD,

    TILE_ALPHANUM_0,
    TILE_ALPHANUM_1,
    TILE_ALPHANUM_2,
    TILE_ALPHANUM_3,
    TILE_ALPHANUM_4,
    TILE_ALPHANUM_5,
    TILE_ALPHANUM_6,
    TILE_ALPHANUM_7,
    TILE_ALPHANUM_8,
    TILE_ALPHANUM_9,

    TILE_MENU_CURSOR_SELECTED,
    TILE_MENU_CURSER_UNSELECTED,

    // JUST A HACK TO GET THE LENGTH OF THIS ENUM
    TILE_IDS_COUNT, 
} Tile_ID;




/* Type of a standard 8x8x8 bit RGB color */
typedef struct ColorRGB {
    uint8_t r, g, b;
} ColorRGB;

/* Type of a color with added 8-bit alpha channel */
typedef struct ColorRGBA {
    struct ColorRGB;
    uint8_t a;
} ColorRGBA;



/* An enumeration of layers in the palette */
typedef enum Palette_Layer {
    PALETTE_BG1,
    PALETTE_BG2,
    PALETTE_FG1,
    PALETTE_FG2,
    PALETTE_LAYER_COUNT,
} Palette_Layer;



typedef enum Value_Type {
    VALUE_STATIC,
    VALUE_DYNAMIC,
    VALUE_DYNAMIC_NOCACHE,
} Value_Type;


typedef struct Value {
    Value_Type type;
    bool       valid;
} Value;


/* 
    Type of a generic `Color` being either
    a static ColorRGB value or a dynamic
    RGB color value.
*/
typedef struct Color {

    Value meta;

    ColorRGB value; // static color / cache
    void *data;     // associated data used for computation if necessary
    const ColorRGB (*compute_dynamic)(void*); // dynamic color

} Color;

/*
    Type of a generic `Offset` being either
    a static pair of x,y offsets or a dynamic offset value.
*/
typedef struct Offset {

    Value meta;
    
    i16_pair value;
    void *data;
    const i16_pair (*compute_dynamic)(void*);

} Offset;

/*
    Type of a `Scale` being either
    a static pair of x, y scale factors or a dynamic value.
*/
typedef struct Scale {
    
    Value meta;
    
    f32_pair value;
    void *data;
    const f32_pair (*compute_dynamic)(void*);
    
} Scale;
    

/*
    Information meant to accompany a Renderer_Config to
    render a tile with a more specific color, position, or size.
    Incorporates a cache so that we can avoid calling the
    dynamic value calculation functions (if they are present) more
    than once per style, since they may be potentially expensive.
*/
typedef struct Style {
    
    Color  const *color;  // (potentially dynamic) color
    Offset const *offset; // (potentially dynamic) offset
    Scale  const *scale;  // (potentially dynamic) scale

} Style;



/* 
    A map from tile id to the coordinates
    (in tile size grid squares) within the texture
    containing the tiles where it should be found.
*/
typedef u8_pair Texture_Mapping[TILE_IDS_COUNT];

/*
    A map from tile id to texture, so that we can render tiles.
*/
typedef SDL_Texture *Texture_Store[TILE_IDS_COUNT];




typedef struct Render_Info {
    i8_pair pos;
    Tile_ID tile;
} Render_Info;

/*
    The most basic elements needed to render a tile to the screen.
*/
typedef struct Render_Config {
    ColorRGB        palette[4];
    u8_pair         render_scale;
    Texture_Store   texture_store;
    Render_Info     info; // updated every draw call
} Render_Config;

