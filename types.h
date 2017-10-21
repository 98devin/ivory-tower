
#pragma once

#include <SDL2/SDL_render.h>
#include <utility>
#include <unordered_map>



template <typename T, typename U = T>
using Pair = std::pair<T, U>;


template <typename T>
class Value {
public:
    virtual const T value() = 0;
    virtual operator const T() { return value(); }
};



/* Type of a standard 8x8x8 bit RGB color */
struct ColorRGB {
    
    uint8_t r, g, b;

    ColorRGB()
        : ColorRGB(0, 0, 0) { }

    ColorRGB(uint8_t r, uint8_t g, uint8_t b)
        : r(r), g(g), b(b) { }
};


/* Type of a color with added 8-bit alpha channel */
struct ColorRGBA : public ColorRGB {
    
    uint8_t a;

    ColorRGBA()
        : ColorRGBA(0, 0, 0, 255) { }

    ColorRGBA(ColorRGB &&c)
        : ColorRGB(c), a(255) { }

    ColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : ColorRGB(r, g, b), a(a) { }
};



/* The enumeration of each tile we can render */
enum class Tile : uint8_t {

    PLAIN_FILL,

    BORDER_N,
    BORDER_S,
    BORDER_E,
    BORDER_W,
    BORDER_NW,
    BORDER_NE,
    BORDER_SW,
    BORDER_SE,
    BORDER_NS,
    BORDER_WE,
    BORDER_NSW,
    BORDER_NSE,
    BORDER_SWE,
    BORDER_NWE,
    BORDER_NSWE,
    BORDER_NONE,

    BORDER_N_BOLD,
    BORDER_S_BOLD,
    BORDER_E_BOLD,
    BORDER_W_BOLD,
    BORDER_NW_BOLD,
    BORDER_NE_BOLD,
    BORDER_SW_BOLD,
    BORDER_SE_BOLD,
    BORDER_NS_BOLD,
    BORDER_WE_BOLD,
    BORDER_NSW_BOLD,
    BORDER_NSE_BOLD,
    BORDER_SWE_BOLD,
    BORDER_NWE_BOLD,
    BORDER_NSWE_BOLD,
    BORDER_NONE_BOLD,

    BORDER_ROUND_N,
    BORDER_ROUND_S,
    BORDER_ROUND_E,
    BORDER_ROUND_W,
    BORDER_ROUND_NW,
    BORDER_ROUND_NE,
    BORDER_ROUND_SW,
    BORDER_ROUND_SE,
    BORDER_ROUND_NS,
    BORDER_ROUND_WE,
    BORDER_ROUND_NSW,
    BORDER_ROUND_NSE,
    BORDER_ROUND_SWE,
    BORDER_ROUND_NWE,
    BORDER_ROUND_NSWE,
    BORDER_ROUND_NONE,

    BORDER_ROUND_N_BOLD,
    BORDER_ROUND_S_BOLD,
    BORDER_ROUND_E_BOLD,
    BORDER_ROUND_W_BOLD,
    BORDER_ROUND_NW_BOLD,
    BORDER_ROUND_NE_BOLD,
    BORDER_ROUND_SW_BOLD,
    BORDER_ROUND_SE_BOLD,
    BORDER_ROUND_NS_BOLD,
    BORDER_ROUND_WE_BOLD,
    BORDER_ROUND_NSW_BOLD,
    BORDER_ROUND_NSE_BOLD,
    BORDER_ROUND_SWE_BOLD,
    BORDER_ROUND_NWE_BOLD,
    BORDER_ROUND_NSWE_BOLD,
    BORDER_ROUND_NONE_BOLD,

    PUNCT_SPACE,       // 
    PUNCT_FSTOP,       // .
    PUNCT_COMMA,       // ,
    PUNCT_LQUOT,       // “
    PUNCT_RQUOT,       // ”
    PUNCT_EMARK,       // !
    PUNCT_QMARK,       // ?
    PUNCT_ELLIPSIS,    // ...
    PUNCT_COLON,       // :
    PUNCT_LPAREN,      // (
    PUNCT_RPAREN,      // )
    PUNCT_EEEMARK,     // !!!
    PUNCT_EQMARK,      // !?
    PUNCT_PLUS,        // +
    PUNCT_MINUS,       // -
    PUNCT_DIVIDE,      // ÷
    PUNCT_MULTIPLY,    // ✖
    PUNCT_LTSIGN,      // <
    PUNCT_GTSIGN,      // >
    PUNCT_SOLIDUS,     // /
    PUNCT_MULTSMALL,   // ×
    PUNCT_MONEY,       // $
    PUNCT_MONEY_BOLD,  
    
    ALPHANUM_A,
    ALPHANUM_B,
    ALPHANUM_C,
    ALPHANUM_D,
    ALPHANUM_E,
    ALPHANUM_F,
    ALPHANUM_G,
    ALPHANUM_H,
    ALPHANUM_I,
    ALPHANUM_J,
    ALPHANUM_K,
    ALPHANUM_L,
    ALPHANUM_M,
    ALPHANUM_N,
    ALPHANUM_O,
    ALPHANUM_P,
    ALPHANUM_Q,
    ALPHANUM_R,
    ALPHANUM_S,
    ALPHANUM_T,
    ALPHANUM_U,
    ALPHANUM_V,
    ALPHANUM_W,
    ALPHANUM_X,
    ALPHANUM_Y,
    ALPHANUM_Z,
    
    ALPHANUM_A_BOLD,
    ALPHANUM_B_BOLD,
    ALPHANUM_C_BOLD,
    ALPHANUM_D_BOLD,
    ALPHANUM_E_BOLD,
    ALPHANUM_F_BOLD,
    ALPHANUM_G_BOLD,
    ALPHANUM_H_BOLD,
    ALPHANUM_I_BOLD,
    ALPHANUM_J_BOLD,
    ALPHANUM_K_BOLD,
    ALPHANUM_L_BOLD,
    ALPHANUM_M_BOLD,
    ALPHANUM_N_BOLD,
    ALPHANUM_O_BOLD,
    ALPHANUM_P_BOLD,
    ALPHANUM_Q_BOLD,
    ALPHANUM_R_BOLD,
    ALPHANUM_S_BOLD,
    ALPHANUM_T_BOLD,
    ALPHANUM_U_BOLD,
    ALPHANUM_V_BOLD,
    ALPHANUM_W_BOLD,
    ALPHANUM_X_BOLD,
    ALPHANUM_Y_BOLD,
    ALPHANUM_Z_BOLD,

    ALPHANUM_0,
    ALPHANUM_1,
    ALPHANUM_2,
    ALPHANUM_3,
    ALPHANUM_4,
    ALPHANUM_5,
    ALPHANUM_6,
    ALPHANUM_7,
    ALPHANUM_8,
    ALPHANUM_9,

    MENU_CURSOR_SELECTED,
    MENU_CURSOR_UNSELECTED,

    // just a kind of hacky way to get the length of the enum
    IDS_COUNT, 
};



/* An enumeration of layers in the palette */
enum class PaletteColor : uint8_t {
    BG_1,
    BG_2,
    FG_1,
    FG_2,
    COLOR_COUNT,
};


