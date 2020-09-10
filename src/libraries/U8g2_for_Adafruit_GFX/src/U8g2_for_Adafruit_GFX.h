/*

  U8g2_for_Adafruit_GFX.h

  Add unicode support and U8g2 fonts to Adafruit GFX libraries.

  U8g2 for Adafruit GFX Lib (https://github.com/olikraus/U8g2_for_Adafruit_GFX)

  Copyright (c) 2018, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef U8g2_for_Adafruit_GFX_h
#define U8g2_for_Adafruit_GFX_h

#include "../../TFT_eSPI/TFT_eSPI.h"
#include "../../Adafruit-GFX-Library/Adafruit_GFX.h"
#include "u8g2_fonts.h"


//========================================================



struct _u8g2_font_info_t {
    /* offset 0 */
    uint8_t glyph_cnt;
    uint8_t bbx_mode;
    uint8_t bits_per_0;
    uint8_t bits_per_1;

    /* offset 4 */
    uint8_t bits_per_char_width;
    uint8_t bits_per_char_height;
    uint8_t bits_per_char_x;
    uint8_t bits_per_char_y;
    uint8_t bits_per_delta_x;

    /* offset 9 */
    int8_t max_char_width;
    int8_t max_char_height; /* overall height, NOT ascent. Instead ascent = max_char_height + y_offset */
    int8_t x_offset;
    int8_t y_offset;

    /* offset 13 */
    int8_t  ascent_A;
    int8_t  descent_g;  /* usually a negative value */
    int8_t  ascent_para;
    int8_t  descent_para;

    /* offset 17 */
    uint16_t start_pos_upper_A;
    uint16_t start_pos_lower_a;

    /* offset 21 */
    uint16_t start_pos_unicode;
};
typedef struct _u8g2_font_info_t u8g2_font_info_t;

struct _u8g2_font_decode_t {
    const uint8_t *decode_ptr;      /* pointer to the compressed data */

    int16_t target_x;
    int16_t target_y;
    uint16_t fg_color;
    uint16_t bg_color;

    int8_t x;           /* local coordinates, (0,0) is upper left */
    int8_t y;
    int8_t glyph_width;
    int8_t glyph_height;

    uint8_t decode_bit_pos;     /* bitpos inside a byte of the compressed data */
    uint8_t is_transparent;
    uint8_t dir;        /* direction */
};
typedef struct _u8g2_font_decode_t u8g2_font_decode_t;


struct _u8g2_font_t {
    Adafruit_GFX *gfx;
    TFT_eSPI *tft;

    /* information about the current font */
    const uint8_t *font;             /* current font for all text procedures */

    //u8g2_font_calc_vref_fnptr font_calc_vref;
    u8g2_font_decode_t font_decode;   /* new font decode structure */
    u8g2_font_info_t font_info;     /* new font info structure */

    //uint8_t font_height_mode;
    //int8_t font_ref_ascent;
    //int8_t font_ref_descent;

    int8_t glyph_x_offset;    /* set by u8g2_GetGlyphWidth as a side effect */
};
typedef struct _u8g2_font_t u8g2_font_t;


uint8_t u8g2_IsGlyph(u8g2_font_t *u8g2, uint16_t requested_encoding);
int8_t u8g2_GetGlyphWidth(u8g2_font_t *u8g2, uint16_t requested_encoding);
void u8g2_SetFontMode(u8g2_font_t *u8g2, uint8_t is_transparent);
void u8g2_SetFontDirection(u8g2_font_t *u8g2, uint8_t dir);
int16_t u8g2_DrawGlyph(u8g2_font_t *u8g2, int16_t x, int16_t y, uint16_t encoding);
int16_t u8g2_DrawStr(u8g2_font_t *u8g2, int16_t x, int16_t y, const char *s);
void u8g2_SetFont(u8g2_font_t *u8g2, const uint8_t  *font);
void u8g2_SetForegroundColor(u8g2_font_t *u8g2, uint16_t fg);
void u8g2_SetBackgroundColor(u8g2_font_t *u8g2, uint16_t bg);


class U8G2_FOR_ADAFRUIT_GFX : public Print
{
public:
    u8g2_font_t u8g2;
    int16_t tx, ty;       // current position for the print command
    uint16_t encoding;    // the unicode, detected by the utf-8 decoder
    uint8_t utf8_state;   // current state of the utf-8 decoder, contains the remaining bytes for a detected unicode glyph

    void home(void)
    {
        tx = 0;
        ty = 0;
        utf8_state = 0;
    }
    void setCursor(int16_t x, int16_t y)
    {
        tx = x;
        ty = y;
        utf8_state = 0;
    }
    int16_t getCursorX(void)
    {
        return tx;
    }
    int16_t getCursorY(void)
    {
        return ty;
    }

    U8G2_FOR_ADAFRUIT_GFX(void)
    {
        u8g2.font = NULL;
        u8g2.font_decode.fg_color = 1;
        u8g2.font_decode.is_transparent = 1;
        u8g2.font_decode.dir = 0;
        home();
    }

    void begin(Adafruit_GFX &gfx)
    {
        u8g2.gfx = &gfx;
        u8g2.tft = NULL;
    }

    void begin(TFT_eSPI &tft)
    {
        u8g2.gfx = NULL;
        u8g2.tft = &tft;
    }
    void setFont(const uint8_t *font)             // set u8g2 font
    {
        u8g2_SetFont(&u8g2, font);
    }
    void setFontMode(uint8_t is_transparent)      // is_transparent==0: Background is not drawn
    {
        u8g2_SetFontMode(&u8g2, is_transparent);
    }
    void setFontDirection(uint8_t d)              // 0; 0 degree, 1: 90 degree, 2: 180 degree, 3: 270 degree
    {
        u8g2_SetFontDirection(&u8g2, d);
    }
    void setForegroundColor(uint16_t fg)           // Use this color to draw the text
    {
        u8g2_SetForegroundColor(&u8g2, fg);
    }
    void setBackgroundColor(uint16_t bg)           // only used for setFontMode(0)
    {
        u8g2_SetBackgroundColor(&u8g2, bg);
    }
    int8_t getFontAscent(void)
    {
        return u8g2.font_info.ascent_A;
    }
    int8_t getFontDescent(void)
    {
        return u8g2.font_info.descent_g;
    }
    int16_t drawGlyph(int16_t x, int16_t y, uint16_t e)
    {
        return u8g2_DrawGlyph(&u8g2, x, y, e);    // draw a signle char (e == Unicode)
    }
    int16_t drawStr(int16_t x, int16_t y, const char *s)
    {
        return u8g2_DrawStr(&u8g2, x, y, s);
    }

    int16_t drawUTF8(int16_t x, int16_t y, const char *str);
    int16_t getUTF8Width(const char *str);


    uint16_t utf8_next(uint8_t b);

    size_t write(uint8_t v)
    {
        uint16_t e = utf8_next(v);
        int16_t delta;

        if ( e == '\n' ) {
            tx = 0;
            ty += u8g2.font_info.ascent_para - u8g2.font_info.descent_para;
        } else if ( e == '\r' ) {
            tx = 0;
        } else if ( e < 0x0fffe ) {
            delta = drawGlyph(tx, ty, e);
            switch (u8g2.font_decode.dir) {
            case 0:
                tx += delta;
                break;
            case 1:
                ty += delta;
                break;
            case 2:
                tx -= delta;
                break;
            case 3:
                ty -= delta;
                break;
            }
        }
        return 1;
    }

    size_t write(const uint8_t *buffer, size_t size)
    {
        size_t cnt = 0;
        while ( size > 0 ) {
            cnt += write(*buffer++);
            size--;
        }
        return cnt;
    }
};


/*==========================================*/

/* start font list */
extern const uint8_t u8g2_font_u8glib_4_tf[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_tf");
extern const uint8_t u8g2_font_u8glib_4_tr[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_tr");
extern const uint8_t u8g2_font_u8glib_4_hf[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_hf");
extern const uint8_t u8g2_font_u8glib_4_hr[] U8G2_FONT_SECTION("u8g2_font_u8glib_4_hr");
extern const uint8_t u8g2_font_m2icon_5_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_5_tf");
extern const uint8_t u8g2_font_m2icon_7_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_7_tf");
extern const uint8_t u8g2_font_m2icon_9_tf[] U8G2_FONT_SECTION("u8g2_font_m2icon_9_tf");
extern const uint8_t u8g2_font_emoticons21_tr[] U8G2_FONT_SECTION("u8g2_font_emoticons21_tr");
extern const uint8_t u8g2_font_battery19_tn[] U8G2_FONT_SECTION("u8g2_font_battery19_tn");
extern const uint8_t u8g2_font_freedoomr10_tu[] U8G2_FONT_SECTION("u8g2_font_freedoomr10_tu");
extern const uint8_t u8g2_font_freedoomr10_mu[] U8G2_FONT_SECTION("u8g2_font_freedoomr10_mu");
extern const uint8_t u8g2_font_freedoomr25_tn[] U8G2_FONT_SECTION("u8g2_font_freedoomr25_tn");
extern const uint8_t u8g2_font_freedoomr25_mn[] U8G2_FONT_SECTION("u8g2_font_freedoomr25_mn");
extern const uint8_t u8g2_font_7Segments_26x42_mn[] U8G2_FONT_SECTION("u8g2_font_7Segments_26x42_mn");
extern const uint8_t u8g2_font_amstrad_cpc_extended_8f[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_extended_8f");
extern const uint8_t u8g2_font_amstrad_cpc_extended_8r[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_extended_8r");
extern const uint8_t u8g2_font_amstrad_cpc_extended_8n[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_extended_8n");
extern const uint8_t u8g2_font_amstrad_cpc_extended_8u[] U8G2_FONT_SECTION("u8g2_font_amstrad_cpc_extended_8u");
extern const uint8_t u8g2_font_cursor_tf[] U8G2_FONT_SECTION("u8g2_font_cursor_tf");
extern const uint8_t u8g2_font_cursor_tr[] U8G2_FONT_SECTION("u8g2_font_cursor_tr");
extern const uint8_t u8g2_font_micro_tr[] U8G2_FONT_SECTION("u8g2_font_micro_tr");
extern const uint8_t u8g2_font_micro_tn[] U8G2_FONT_SECTION("u8g2_font_micro_tn");
extern const uint8_t u8g2_font_micro_mr[] U8G2_FONT_SECTION("u8g2_font_micro_mr");
extern const uint8_t u8g2_font_micro_mn[] U8G2_FONT_SECTION("u8g2_font_micro_mn");
extern const uint8_t u8g2_font_4x6_tf[] U8G2_FONT_SECTION("u8g2_font_4x6_tf");
extern const uint8_t u8g2_font_4x6_tr[] U8G2_FONT_SECTION("u8g2_font_4x6_tr");
extern const uint8_t u8g2_font_4x6_tn[] U8G2_FONT_SECTION("u8g2_font_4x6_tn");
extern const uint8_t u8g2_font_4x6_mf[] U8G2_FONT_SECTION("u8g2_font_4x6_mf");
extern const uint8_t u8g2_font_4x6_mr[] U8G2_FONT_SECTION("u8g2_font_4x6_mr");
extern const uint8_t u8g2_font_4x6_mn[] U8G2_FONT_SECTION("u8g2_font_4x6_mn");
extern const uint8_t u8g2_font_4x6_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_4x6_t_cyrillic");
extern const uint8_t u8g2_font_5x7_tf[] U8G2_FONT_SECTION("u8g2_font_5x7_tf");
extern const uint8_t u8g2_font_5x7_tr[] U8G2_FONT_SECTION("u8g2_font_5x7_tr");
extern const uint8_t u8g2_font_5x7_tn[] U8G2_FONT_SECTION("u8g2_font_5x7_tn");
extern const uint8_t u8g2_font_5x7_mf[] U8G2_FONT_SECTION("u8g2_font_5x7_mf");
extern const uint8_t u8g2_font_5x7_mr[] U8G2_FONT_SECTION("u8g2_font_5x7_mr");
extern const uint8_t u8g2_font_5x7_mn[] U8G2_FONT_SECTION("u8g2_font_5x7_mn");
extern const uint8_t u8g2_font_5x7_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_5x7_t_cyrillic");
extern const uint8_t u8g2_font_5x8_tf[] U8G2_FONT_SECTION("u8g2_font_5x8_tf");
extern const uint8_t u8g2_font_5x8_tr[] U8G2_FONT_SECTION("u8g2_font_5x8_tr");
extern const uint8_t u8g2_font_5x8_tn[] U8G2_FONT_SECTION("u8g2_font_5x8_tn");
extern const uint8_t u8g2_font_5x8_mf[] U8G2_FONT_SECTION("u8g2_font_5x8_mf");
extern const uint8_t u8g2_font_5x8_mr[] U8G2_FONT_SECTION("u8g2_font_5x8_mr");
extern const uint8_t u8g2_font_5x8_mn[] U8G2_FONT_SECTION("u8g2_font_5x8_mn");
extern const uint8_t u8g2_font_5x8_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_5x8_t_cyrillic");
extern const uint8_t u8g2_font_6x10_tf[] U8G2_FONT_SECTION("u8g2_font_6x10_tf");
extern const uint8_t u8g2_font_6x10_tr[] U8G2_FONT_SECTION("u8g2_font_6x10_tr");
extern const uint8_t u8g2_font_6x10_tn[] U8G2_FONT_SECTION("u8g2_font_6x10_tn");
extern const uint8_t u8g2_font_6x10_mf[] U8G2_FONT_SECTION("u8g2_font_6x10_mf");
extern const uint8_t u8g2_font_6x10_mr[] U8G2_FONT_SECTION("u8g2_font_6x10_mr");
extern const uint8_t u8g2_font_6x10_mn[] U8G2_FONT_SECTION("u8g2_font_6x10_mn");
extern const uint8_t u8g2_font_6x12_tf[] U8G2_FONT_SECTION("u8g2_font_6x12_tf");
extern const uint8_t u8g2_font_6x12_tr[] U8G2_FONT_SECTION("u8g2_font_6x12_tr");
extern const uint8_t u8g2_font_6x12_tn[] U8G2_FONT_SECTION("u8g2_font_6x12_tn");
extern const uint8_t u8g2_font_6x12_te[] U8G2_FONT_SECTION("u8g2_font_6x12_te");
extern const uint8_t u8g2_font_6x12_mf[] U8G2_FONT_SECTION("u8g2_font_6x12_mf");
extern const uint8_t u8g2_font_6x12_mr[] U8G2_FONT_SECTION("u8g2_font_6x12_mr");
extern const uint8_t u8g2_font_6x12_mn[] U8G2_FONT_SECTION("u8g2_font_6x12_mn");
extern const uint8_t u8g2_font_6x12_me[] U8G2_FONT_SECTION("u8g2_font_6x12_me");
extern const uint8_t u8g2_font_6x12_t_symbols[] U8G2_FONT_SECTION("u8g2_font_6x12_t_symbols");
extern const uint8_t u8g2_font_6x12_m_symbols[] U8G2_FONT_SECTION("u8g2_font_6x12_m_symbols");
extern const uint8_t u8g2_font_6x12_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_6x12_t_cyrillic");
extern const uint8_t u8g2_font_6x13_tf[] U8G2_FONT_SECTION("u8g2_font_6x13_tf");
extern const uint8_t u8g2_font_6x13_tr[] U8G2_FONT_SECTION("u8g2_font_6x13_tr");
extern const uint8_t u8g2_font_6x13_tn[] U8G2_FONT_SECTION("u8g2_font_6x13_tn");
extern const uint8_t u8g2_font_6x13_te[] U8G2_FONT_SECTION("u8g2_font_6x13_te");
extern const uint8_t u8g2_font_6x13_mf[] U8G2_FONT_SECTION("u8g2_font_6x13_mf");
extern const uint8_t u8g2_font_6x13_mr[] U8G2_FONT_SECTION("u8g2_font_6x13_mr");
extern const uint8_t u8g2_font_6x13_mn[] U8G2_FONT_SECTION("u8g2_font_6x13_mn");
extern const uint8_t u8g2_font_6x13_me[] U8G2_FONT_SECTION("u8g2_font_6x13_me");
extern const uint8_t u8g2_font_6x13_t_hebrew[] U8G2_FONT_SECTION("u8g2_font_6x13_t_hebrew");
extern const uint8_t u8g2_font_6x13_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_6x13_t_cyrillic");
extern const uint8_t u8g2_font_6x13B_tf[] U8G2_FONT_SECTION("u8g2_font_6x13B_tf");
extern const uint8_t u8g2_font_6x13B_tr[] U8G2_FONT_SECTION("u8g2_font_6x13B_tr");
extern const uint8_t u8g2_font_6x13B_tn[] U8G2_FONT_SECTION("u8g2_font_6x13B_tn");
extern const uint8_t u8g2_font_6x13B_mf[] U8G2_FONT_SECTION("u8g2_font_6x13B_mf");
extern const uint8_t u8g2_font_6x13B_mr[] U8G2_FONT_SECTION("u8g2_font_6x13B_mr");
extern const uint8_t u8g2_font_6x13B_mn[] U8G2_FONT_SECTION("u8g2_font_6x13B_mn");
extern const uint8_t u8g2_font_6x13B_t_hebrew[] U8G2_FONT_SECTION("u8g2_font_6x13B_t_hebrew");
extern const uint8_t u8g2_font_6x13B_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_6x13B_t_cyrillic");
extern const uint8_t u8g2_font_6x13O_tf[] U8G2_FONT_SECTION("u8g2_font_6x13O_tf");
extern const uint8_t u8g2_font_6x13O_tr[] U8G2_FONT_SECTION("u8g2_font_6x13O_tr");
extern const uint8_t u8g2_font_6x13O_tn[] U8G2_FONT_SECTION("u8g2_font_6x13O_tn");
extern const uint8_t u8g2_font_6x13O_mf[] U8G2_FONT_SECTION("u8g2_font_6x13O_mf");
extern const uint8_t u8g2_font_6x13O_mr[] U8G2_FONT_SECTION("u8g2_font_6x13O_mr");
extern const uint8_t u8g2_font_6x13O_mn[] U8G2_FONT_SECTION("u8g2_font_6x13O_mn");
extern const uint8_t u8g2_font_7x13_tf[] U8G2_FONT_SECTION("u8g2_font_7x13_tf");
extern const uint8_t u8g2_font_7x13_tr[] U8G2_FONT_SECTION("u8g2_font_7x13_tr");
extern const uint8_t u8g2_font_7x13_tn[] U8G2_FONT_SECTION("u8g2_font_7x13_tn");
extern const uint8_t u8g2_font_7x13_te[] U8G2_FONT_SECTION("u8g2_font_7x13_te");
extern const uint8_t u8g2_font_7x13_mf[] U8G2_FONT_SECTION("u8g2_font_7x13_mf");
extern const uint8_t u8g2_font_7x13_mr[] U8G2_FONT_SECTION("u8g2_font_7x13_mr");
extern const uint8_t u8g2_font_7x13_mn[] U8G2_FONT_SECTION("u8g2_font_7x13_mn");
extern const uint8_t u8g2_font_7x13_me[] U8G2_FONT_SECTION("u8g2_font_7x13_me");
extern const uint8_t u8g2_font_7x13_t_symbols[] U8G2_FONT_SECTION("u8g2_font_7x13_t_symbols");
extern const uint8_t u8g2_font_7x13_m_symbols[] U8G2_FONT_SECTION("u8g2_font_7x13_m_symbols");
extern const uint8_t u8g2_font_7x13_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_7x13_t_cyrillic");
extern const uint8_t u8g2_font_7x13B_tf[] U8G2_FONT_SECTION("u8g2_font_7x13B_tf");
extern const uint8_t u8g2_font_7x13B_tr[] U8G2_FONT_SECTION("u8g2_font_7x13B_tr");
extern const uint8_t u8g2_font_7x13B_tn[] U8G2_FONT_SECTION("u8g2_font_7x13B_tn");
extern const uint8_t u8g2_font_7x13B_mf[] U8G2_FONT_SECTION("u8g2_font_7x13B_mf");
extern const uint8_t u8g2_font_7x13B_mr[] U8G2_FONT_SECTION("u8g2_font_7x13B_mr");
extern const uint8_t u8g2_font_7x13B_mn[] U8G2_FONT_SECTION("u8g2_font_7x13B_mn");
extern const uint8_t u8g2_font_7x13O_tf[] U8G2_FONT_SECTION("u8g2_font_7x13O_tf");
extern const uint8_t u8g2_font_7x13O_tr[] U8G2_FONT_SECTION("u8g2_font_7x13O_tr");
extern const uint8_t u8g2_font_7x13O_tn[] U8G2_FONT_SECTION("u8g2_font_7x13O_tn");
extern const uint8_t u8g2_font_7x13O_mf[] U8G2_FONT_SECTION("u8g2_font_7x13O_mf");
extern const uint8_t u8g2_font_7x13O_mr[] U8G2_FONT_SECTION("u8g2_font_7x13O_mr");
extern const uint8_t u8g2_font_7x13O_mn[] U8G2_FONT_SECTION("u8g2_font_7x13O_mn");
extern const uint8_t u8g2_font_7x14_tf[] U8G2_FONT_SECTION("u8g2_font_7x14_tf");
extern const uint8_t u8g2_font_7x14_tr[] U8G2_FONT_SECTION("u8g2_font_7x14_tr");
extern const uint8_t u8g2_font_7x14_tn[] U8G2_FONT_SECTION("u8g2_font_7x14_tn");
extern const uint8_t u8g2_font_7x14_mf[] U8G2_FONT_SECTION("u8g2_font_7x14_mf");
extern const uint8_t u8g2_font_7x14_mr[] U8G2_FONT_SECTION("u8g2_font_7x14_mr");
extern const uint8_t u8g2_font_7x14_mn[] U8G2_FONT_SECTION("u8g2_font_7x14_mn");
extern const uint8_t u8g2_font_7x14B_tf[] U8G2_FONT_SECTION("u8g2_font_7x14B_tf");
extern const uint8_t u8g2_font_7x14B_tr[] U8G2_FONT_SECTION("u8g2_font_7x14B_tr");
extern const uint8_t u8g2_font_7x14B_tn[] U8G2_FONT_SECTION("u8g2_font_7x14B_tn");
extern const uint8_t u8g2_font_7x14B_mf[] U8G2_FONT_SECTION("u8g2_font_7x14B_mf");
extern const uint8_t u8g2_font_7x14B_mr[] U8G2_FONT_SECTION("u8g2_font_7x14B_mr");
extern const uint8_t u8g2_font_7x14B_mn[] U8G2_FONT_SECTION("u8g2_font_7x14B_mn");
extern const uint8_t u8g2_font_8x13_tf[] U8G2_FONT_SECTION("u8g2_font_8x13_tf");
extern const uint8_t u8g2_font_8x13_tr[] U8G2_FONT_SECTION("u8g2_font_8x13_tr");
extern const uint8_t u8g2_font_8x13_tn[] U8G2_FONT_SECTION("u8g2_font_8x13_tn");
extern const uint8_t u8g2_font_8x13_te[] U8G2_FONT_SECTION("u8g2_font_8x13_te");
extern const uint8_t u8g2_font_8x13_mf[] U8G2_FONT_SECTION("u8g2_font_8x13_mf");
extern const uint8_t u8g2_font_8x13_mr[] U8G2_FONT_SECTION("u8g2_font_8x13_mr");
extern const uint8_t u8g2_font_8x13_mn[] U8G2_FONT_SECTION("u8g2_font_8x13_mn");
extern const uint8_t u8g2_font_8x13_me[] U8G2_FONT_SECTION("u8g2_font_8x13_me");
extern const uint8_t u8g2_font_8x13_t_symbols[] U8G2_FONT_SECTION("u8g2_font_8x13_t_symbols");
extern const uint8_t u8g2_font_8x13_m_symbols[] U8G2_FONT_SECTION("u8g2_font_8x13_m_symbols");
extern const uint8_t u8g2_font_8x13_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_8x13_t_cyrillic");
extern const uint8_t u8g2_font_8x13B_tf[] U8G2_FONT_SECTION("u8g2_font_8x13B_tf");
extern const uint8_t u8g2_font_8x13B_tr[] U8G2_FONT_SECTION("u8g2_font_8x13B_tr");
extern const uint8_t u8g2_font_8x13B_tn[] U8G2_FONT_SECTION("u8g2_font_8x13B_tn");
extern const uint8_t u8g2_font_8x13B_mf[] U8G2_FONT_SECTION("u8g2_font_8x13B_mf");
extern const uint8_t u8g2_font_8x13B_mr[] U8G2_FONT_SECTION("u8g2_font_8x13B_mr");
extern const uint8_t u8g2_font_8x13B_mn[] U8G2_FONT_SECTION("u8g2_font_8x13B_mn");
extern const uint8_t u8g2_font_8x13O_tf[] U8G2_FONT_SECTION("u8g2_font_8x13O_tf");
extern const uint8_t u8g2_font_8x13O_tr[] U8G2_FONT_SECTION("u8g2_font_8x13O_tr");
extern const uint8_t u8g2_font_8x13O_tn[] U8G2_FONT_SECTION("u8g2_font_8x13O_tn");
extern const uint8_t u8g2_font_8x13O_mf[] U8G2_FONT_SECTION("u8g2_font_8x13O_mf");
extern const uint8_t u8g2_font_8x13O_mr[] U8G2_FONT_SECTION("u8g2_font_8x13O_mr");
extern const uint8_t u8g2_font_8x13O_mn[] U8G2_FONT_SECTION("u8g2_font_8x13O_mn");
extern const uint8_t u8g2_font_9x15_tf[] U8G2_FONT_SECTION("u8g2_font_9x15_tf");
extern const uint8_t u8g2_font_9x15_tr[] U8G2_FONT_SECTION("u8g2_font_9x15_tr");
extern const uint8_t u8g2_font_9x15_tn[] U8G2_FONT_SECTION("u8g2_font_9x15_tn");
extern const uint8_t u8g2_font_9x15_te[] U8G2_FONT_SECTION("u8g2_font_9x15_te");
extern const uint8_t u8g2_font_9x15_mf[] U8G2_FONT_SECTION("u8g2_font_9x15_mf");
extern const uint8_t u8g2_font_9x15_mr[] U8G2_FONT_SECTION("u8g2_font_9x15_mr");
extern const uint8_t u8g2_font_9x15_mn[] U8G2_FONT_SECTION("u8g2_font_9x15_mn");
extern const uint8_t u8g2_font_9x15_me[] U8G2_FONT_SECTION("u8g2_font_9x15_me");
extern const uint8_t u8g2_font_9x15_t_symbols[] U8G2_FONT_SECTION("u8g2_font_9x15_t_symbols");
extern const uint8_t u8g2_font_9x15_m_symbols[] U8G2_FONT_SECTION("u8g2_font_9x15_m_symbols");
extern const uint8_t u8g2_font_9x15_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_9x15_t_cyrillic");
extern const uint8_t u8g2_font_9x15B_tf[] U8G2_FONT_SECTION("u8g2_font_9x15B_tf");
extern const uint8_t u8g2_font_9x15B_tr[] U8G2_FONT_SECTION("u8g2_font_9x15B_tr");
extern const uint8_t u8g2_font_9x15B_tn[] U8G2_FONT_SECTION("u8g2_font_9x15B_tn");
extern const uint8_t u8g2_font_9x15B_mf[] U8G2_FONT_SECTION("u8g2_font_9x15B_mf");
extern const uint8_t u8g2_font_9x15B_mr[] U8G2_FONT_SECTION("u8g2_font_9x15B_mr");
extern const uint8_t u8g2_font_9x15B_mn[] U8G2_FONT_SECTION("u8g2_font_9x15B_mn");
extern const uint8_t u8g2_font_9x18_tf[] U8G2_FONT_SECTION("u8g2_font_9x18_tf");
extern const uint8_t u8g2_font_9x18_tr[] U8G2_FONT_SECTION("u8g2_font_9x18_tr");
extern const uint8_t u8g2_font_9x18_tn[] U8G2_FONT_SECTION("u8g2_font_9x18_tn");
extern const uint8_t u8g2_font_9x18_mf[] U8G2_FONT_SECTION("u8g2_font_9x18_mf");
extern const uint8_t u8g2_font_9x18_mr[] U8G2_FONT_SECTION("u8g2_font_9x18_mr");
extern const uint8_t u8g2_font_9x18_mn[] U8G2_FONT_SECTION("u8g2_font_9x18_mn");
extern const uint8_t u8g2_font_9x18B_tf[] U8G2_FONT_SECTION("u8g2_font_9x18B_tf");
extern const uint8_t u8g2_font_9x18B_tr[] U8G2_FONT_SECTION("u8g2_font_9x18B_tr");
extern const uint8_t u8g2_font_9x18B_tn[] U8G2_FONT_SECTION("u8g2_font_9x18B_tn");
extern const uint8_t u8g2_font_9x18B_mf[] U8G2_FONT_SECTION("u8g2_font_9x18B_mf");
extern const uint8_t u8g2_font_9x18B_mr[] U8G2_FONT_SECTION("u8g2_font_9x18B_mr");
extern const uint8_t u8g2_font_9x18B_mn[] U8G2_FONT_SECTION("u8g2_font_9x18B_mn");
extern const uint8_t u8g2_font_10x20_tf[] U8G2_FONT_SECTION("u8g2_font_10x20_tf");
extern const uint8_t u8g2_font_10x20_tr[] U8G2_FONT_SECTION("u8g2_font_10x20_tr");
extern const uint8_t u8g2_font_10x20_tn[] U8G2_FONT_SECTION("u8g2_font_10x20_tn");
extern const uint8_t u8g2_font_10x20_te[] U8G2_FONT_SECTION("u8g2_font_10x20_te");
extern const uint8_t u8g2_font_10x20_mf[] U8G2_FONT_SECTION("u8g2_font_10x20_mf");
extern const uint8_t u8g2_font_10x20_mr[] U8G2_FONT_SECTION("u8g2_font_10x20_mr");
extern const uint8_t u8g2_font_10x20_mn[] U8G2_FONT_SECTION("u8g2_font_10x20_mn");
extern const uint8_t u8g2_font_10x20_me[] U8G2_FONT_SECTION("u8g2_font_10x20_me");
extern const uint8_t u8g2_font_10x20_t_greek[] U8G2_FONT_SECTION("u8g2_font_10x20_t_greek");
extern const uint8_t u8g2_font_10x20_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_10x20_t_cyrillic");
extern const uint8_t u8g2_font_10x20_t_arabic[] U8G2_FONT_SECTION("u8g2_font_10x20_t_arabic");
extern const uint8_t u8g2_font_siji_t_6x10[] U8G2_FONT_SECTION("u8g2_font_siji_t_6x10");
extern const uint8_t u8g2_font_tom_thumb_4x6_t_all[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_t_all");
extern const uint8_t u8g2_font_tom_thumb_4x6_tf[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_tf");
extern const uint8_t u8g2_font_tom_thumb_4x6_tr[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_tr");
extern const uint8_t u8g2_font_tom_thumb_4x6_tn[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_tn");
extern const uint8_t u8g2_font_tom_thumb_4x6_te[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_te");
extern const uint8_t u8g2_font_tom_thumb_4x6_mf[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_mf");
extern const uint8_t u8g2_font_tom_thumb_4x6_mr[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_mr");
extern const uint8_t u8g2_font_tom_thumb_4x6_mn[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_mn");
extern const uint8_t u8g2_font_tom_thumb_4x6_me[] U8G2_FONT_SECTION("u8g2_font_tom_thumb_4x6_me");
extern const uint8_t u8g2_font_t0_11_tf[] U8G2_FONT_SECTION("u8g2_font_t0_11_tf");
extern const uint8_t u8g2_font_t0_11_tr[] U8G2_FONT_SECTION("u8g2_font_t0_11_tr");
extern const uint8_t u8g2_font_t0_11_tn[] U8G2_FONT_SECTION("u8g2_font_t0_11_tn");
extern const uint8_t u8g2_font_t0_11_te[] U8G2_FONT_SECTION("u8g2_font_t0_11_te");
extern const uint8_t u8g2_font_t0_11_mf[] U8G2_FONT_SECTION("u8g2_font_t0_11_mf");
extern const uint8_t u8g2_font_t0_11_mr[] U8G2_FONT_SECTION("u8g2_font_t0_11_mr");
extern const uint8_t u8g2_font_t0_11_mn[] U8G2_FONT_SECTION("u8g2_font_t0_11_mn");
extern const uint8_t u8g2_font_t0_11_me[] U8G2_FONT_SECTION("u8g2_font_t0_11_me");
extern const uint8_t u8g2_font_t0_11_t_all[] U8G2_FONT_SECTION("u8g2_font_t0_11_t_all");
extern const uint8_t u8g2_font_t0_11b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_11b_tf");
extern const uint8_t u8g2_font_t0_11b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_11b_tr");
extern const uint8_t u8g2_font_t0_11b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_11b_tn");
extern const uint8_t u8g2_font_t0_11b_te[] U8G2_FONT_SECTION("u8g2_font_t0_11b_te");
extern const uint8_t u8g2_font_t0_11b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_11b_mf");
extern const uint8_t u8g2_font_t0_11b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_11b_mr");
extern const uint8_t u8g2_font_t0_11b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_11b_mn");
extern const uint8_t u8g2_font_t0_11b_me[] U8G2_FONT_SECTION("u8g2_font_t0_11b_me");
extern const uint8_t u8g2_font_t0_12_tf[] U8G2_FONT_SECTION("u8g2_font_t0_12_tf");
extern const uint8_t u8g2_font_t0_12_tr[] U8G2_FONT_SECTION("u8g2_font_t0_12_tr");
extern const uint8_t u8g2_font_t0_12_tn[] U8G2_FONT_SECTION("u8g2_font_t0_12_tn");
extern const uint8_t u8g2_font_t0_12_te[] U8G2_FONT_SECTION("u8g2_font_t0_12_te");
extern const uint8_t u8g2_font_t0_12_mf[] U8G2_FONT_SECTION("u8g2_font_t0_12_mf");
extern const uint8_t u8g2_font_t0_12_mr[] U8G2_FONT_SECTION("u8g2_font_t0_12_mr");
extern const uint8_t u8g2_font_t0_12_mn[] U8G2_FONT_SECTION("u8g2_font_t0_12_mn");
extern const uint8_t u8g2_font_t0_12_me[] U8G2_FONT_SECTION("u8g2_font_t0_12_me");
extern const uint8_t u8g2_font_t0_12b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_12b_tf");
extern const uint8_t u8g2_font_t0_12b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_12b_tr");
extern const uint8_t u8g2_font_t0_12b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_12b_tn");
extern const uint8_t u8g2_font_t0_12b_te[] U8G2_FONT_SECTION("u8g2_font_t0_12b_te");
extern const uint8_t u8g2_font_t0_12b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_12b_mf");
extern const uint8_t u8g2_font_t0_12b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_12b_mr");
extern const uint8_t u8g2_font_t0_12b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_12b_mn");
extern const uint8_t u8g2_font_t0_12b_me[] U8G2_FONT_SECTION("u8g2_font_t0_12b_me");
extern const uint8_t u8g2_font_t0_13_tf[] U8G2_FONT_SECTION("u8g2_font_t0_13_tf");
extern const uint8_t u8g2_font_t0_13_tr[] U8G2_FONT_SECTION("u8g2_font_t0_13_tr");
extern const uint8_t u8g2_font_t0_13_tn[] U8G2_FONT_SECTION("u8g2_font_t0_13_tn");
extern const uint8_t u8g2_font_t0_13_te[] U8G2_FONT_SECTION("u8g2_font_t0_13_te");
extern const uint8_t u8g2_font_t0_13_mf[] U8G2_FONT_SECTION("u8g2_font_t0_13_mf");
extern const uint8_t u8g2_font_t0_13_mr[] U8G2_FONT_SECTION("u8g2_font_t0_13_mr");
extern const uint8_t u8g2_font_t0_13_mn[] U8G2_FONT_SECTION("u8g2_font_t0_13_mn");
extern const uint8_t u8g2_font_t0_13_me[] U8G2_FONT_SECTION("u8g2_font_t0_13_me");
extern const uint8_t u8g2_font_t0_13b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_13b_tf");
extern const uint8_t u8g2_font_t0_13b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_13b_tr");
extern const uint8_t u8g2_font_t0_13b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_13b_tn");
extern const uint8_t u8g2_font_t0_13b_te[] U8G2_FONT_SECTION("u8g2_font_t0_13b_te");
extern const uint8_t u8g2_font_t0_13b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_13b_mf");
extern const uint8_t u8g2_font_t0_13b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_13b_mr");
extern const uint8_t u8g2_font_t0_13b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_13b_mn");
extern const uint8_t u8g2_font_t0_13b_me[] U8G2_FONT_SECTION("u8g2_font_t0_13b_me");
extern const uint8_t u8g2_font_t0_14_tf[] U8G2_FONT_SECTION("u8g2_font_t0_14_tf");
extern const uint8_t u8g2_font_t0_14_tr[] U8G2_FONT_SECTION("u8g2_font_t0_14_tr");
extern const uint8_t u8g2_font_t0_14_tn[] U8G2_FONT_SECTION("u8g2_font_t0_14_tn");
extern const uint8_t u8g2_font_t0_14_te[] U8G2_FONT_SECTION("u8g2_font_t0_14_te");
extern const uint8_t u8g2_font_t0_14_mf[] U8G2_FONT_SECTION("u8g2_font_t0_14_mf");
extern const uint8_t u8g2_font_t0_14_mr[] U8G2_FONT_SECTION("u8g2_font_t0_14_mr");
extern const uint8_t u8g2_font_t0_14_mn[] U8G2_FONT_SECTION("u8g2_font_t0_14_mn");
extern const uint8_t u8g2_font_t0_14_me[] U8G2_FONT_SECTION("u8g2_font_t0_14_me");
extern const uint8_t u8g2_font_t0_14b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_14b_tf");
extern const uint8_t u8g2_font_t0_14b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_14b_tr");
extern const uint8_t u8g2_font_t0_14b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_14b_tn");
extern const uint8_t u8g2_font_t0_14b_te[] U8G2_FONT_SECTION("u8g2_font_t0_14b_te");
extern const uint8_t u8g2_font_t0_14b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_14b_mf");
extern const uint8_t u8g2_font_t0_14b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_14b_mr");
extern const uint8_t u8g2_font_t0_14b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_14b_mn");
extern const uint8_t u8g2_font_t0_14b_me[] U8G2_FONT_SECTION("u8g2_font_t0_14b_me");
extern const uint8_t u8g2_font_t0_15_tf[] U8G2_FONT_SECTION("u8g2_font_t0_15_tf");
extern const uint8_t u8g2_font_t0_15_tr[] U8G2_FONT_SECTION("u8g2_font_t0_15_tr");
extern const uint8_t u8g2_font_t0_15_tn[] U8G2_FONT_SECTION("u8g2_font_t0_15_tn");
extern const uint8_t u8g2_font_t0_15_te[] U8G2_FONT_SECTION("u8g2_font_t0_15_te");
extern const uint8_t u8g2_font_t0_15_mf[] U8G2_FONT_SECTION("u8g2_font_t0_15_mf");
extern const uint8_t u8g2_font_t0_15_mr[] U8G2_FONT_SECTION("u8g2_font_t0_15_mr");
extern const uint8_t u8g2_font_t0_15_mn[] U8G2_FONT_SECTION("u8g2_font_t0_15_mn");
extern const uint8_t u8g2_font_t0_15_me[] U8G2_FONT_SECTION("u8g2_font_t0_15_me");
extern const uint8_t u8g2_font_t0_15b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_15b_tf");
extern const uint8_t u8g2_font_t0_15b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_15b_tr");
extern const uint8_t u8g2_font_t0_15b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_15b_tn");
extern const uint8_t u8g2_font_t0_15b_te[] U8G2_FONT_SECTION("u8g2_font_t0_15b_te");
extern const uint8_t u8g2_font_t0_15b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_15b_mf");
extern const uint8_t u8g2_font_t0_15b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_15b_mr");
extern const uint8_t u8g2_font_t0_15b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_15b_mn");
extern const uint8_t u8g2_font_t0_15b_me[] U8G2_FONT_SECTION("u8g2_font_t0_15b_me");
extern const uint8_t u8g2_font_t0_16_tf[] U8G2_FONT_SECTION("u8g2_font_t0_16_tf");
extern const uint8_t u8g2_font_t0_16_tr[] U8G2_FONT_SECTION("u8g2_font_t0_16_tr");
extern const uint8_t u8g2_font_t0_16_tn[] U8G2_FONT_SECTION("u8g2_font_t0_16_tn");
extern const uint8_t u8g2_font_t0_16_te[] U8G2_FONT_SECTION("u8g2_font_t0_16_te");
extern const uint8_t u8g2_font_t0_16_mf[] U8G2_FONT_SECTION("u8g2_font_t0_16_mf");
extern const uint8_t u8g2_font_t0_16_mr[] U8G2_FONT_SECTION("u8g2_font_t0_16_mr");
extern const uint8_t u8g2_font_t0_16_mn[] U8G2_FONT_SECTION("u8g2_font_t0_16_mn");
extern const uint8_t u8g2_font_t0_16_me[] U8G2_FONT_SECTION("u8g2_font_t0_16_me");
extern const uint8_t u8g2_font_t0_16b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_16b_tf");
extern const uint8_t u8g2_font_t0_16b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_16b_tr");
extern const uint8_t u8g2_font_t0_16b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_16b_tn");
extern const uint8_t u8g2_font_t0_16b_te[] U8G2_FONT_SECTION("u8g2_font_t0_16b_te");
extern const uint8_t u8g2_font_t0_16b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_16b_mf");
extern const uint8_t u8g2_font_t0_16b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_16b_mr");
extern const uint8_t u8g2_font_t0_16b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_16b_mn");
extern const uint8_t u8g2_font_t0_16b_me[] U8G2_FONT_SECTION("u8g2_font_t0_16b_me");
extern const uint8_t u8g2_font_t0_17_tf[] U8G2_FONT_SECTION("u8g2_font_t0_17_tf");
extern const uint8_t u8g2_font_t0_17_tr[] U8G2_FONT_SECTION("u8g2_font_t0_17_tr");
extern const uint8_t u8g2_font_t0_17_tn[] U8G2_FONT_SECTION("u8g2_font_t0_17_tn");
extern const uint8_t u8g2_font_t0_17_te[] U8G2_FONT_SECTION("u8g2_font_t0_17_te");
extern const uint8_t u8g2_font_t0_17_mf[] U8G2_FONT_SECTION("u8g2_font_t0_17_mf");
extern const uint8_t u8g2_font_t0_17_mr[] U8G2_FONT_SECTION("u8g2_font_t0_17_mr");
extern const uint8_t u8g2_font_t0_17_mn[] U8G2_FONT_SECTION("u8g2_font_t0_17_mn");
extern const uint8_t u8g2_font_t0_17_me[] U8G2_FONT_SECTION("u8g2_font_t0_17_me");
extern const uint8_t u8g2_font_t0_17b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_17b_tf");
extern const uint8_t u8g2_font_t0_17b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_17b_tr");
extern const uint8_t u8g2_font_t0_17b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_17b_tn");
extern const uint8_t u8g2_font_t0_17b_te[] U8G2_FONT_SECTION("u8g2_font_t0_17b_te");
extern const uint8_t u8g2_font_t0_17b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_17b_mf");
extern const uint8_t u8g2_font_t0_17b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_17b_mr");
extern const uint8_t u8g2_font_t0_17b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_17b_mn");
extern const uint8_t u8g2_font_t0_17b_me[] U8G2_FONT_SECTION("u8g2_font_t0_17b_me");
extern const uint8_t u8g2_font_t0_18_tf[] U8G2_FONT_SECTION("u8g2_font_t0_18_tf");
extern const uint8_t u8g2_font_t0_18_tr[] U8G2_FONT_SECTION("u8g2_font_t0_18_tr");
extern const uint8_t u8g2_font_t0_18_tn[] U8G2_FONT_SECTION("u8g2_font_t0_18_tn");
extern const uint8_t u8g2_font_t0_18_te[] U8G2_FONT_SECTION("u8g2_font_t0_18_te");
extern const uint8_t u8g2_font_t0_18_mf[] U8G2_FONT_SECTION("u8g2_font_t0_18_mf");
extern const uint8_t u8g2_font_t0_18_mr[] U8G2_FONT_SECTION("u8g2_font_t0_18_mr");
extern const uint8_t u8g2_font_t0_18_mn[] U8G2_FONT_SECTION("u8g2_font_t0_18_mn");
extern const uint8_t u8g2_font_t0_18_me[] U8G2_FONT_SECTION("u8g2_font_t0_18_me");
extern const uint8_t u8g2_font_t0_18b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_18b_tf");
extern const uint8_t u8g2_font_t0_18b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_18b_tr");
extern const uint8_t u8g2_font_t0_18b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_18b_tn");
extern const uint8_t u8g2_font_t0_18b_te[] U8G2_FONT_SECTION("u8g2_font_t0_18b_te");
extern const uint8_t u8g2_font_t0_18b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_18b_mf");
extern const uint8_t u8g2_font_t0_18b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_18b_mr");
extern const uint8_t u8g2_font_t0_18b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_18b_mn");
extern const uint8_t u8g2_font_t0_18b_me[] U8G2_FONT_SECTION("u8g2_font_t0_18b_me");
extern const uint8_t u8g2_font_t0_22_tf[] U8G2_FONT_SECTION("u8g2_font_t0_22_tf");
extern const uint8_t u8g2_font_t0_22_tr[] U8G2_FONT_SECTION("u8g2_font_t0_22_tr");
extern const uint8_t u8g2_font_t0_22_tn[] U8G2_FONT_SECTION("u8g2_font_t0_22_tn");
extern const uint8_t u8g2_font_t0_22_te[] U8G2_FONT_SECTION("u8g2_font_t0_22_te");
extern const uint8_t u8g2_font_t0_22_mf[] U8G2_FONT_SECTION("u8g2_font_t0_22_mf");
extern const uint8_t u8g2_font_t0_22_mr[] U8G2_FONT_SECTION("u8g2_font_t0_22_mr");
extern const uint8_t u8g2_font_t0_22_mn[] U8G2_FONT_SECTION("u8g2_font_t0_22_mn");
extern const uint8_t u8g2_font_t0_22_me[] U8G2_FONT_SECTION("u8g2_font_t0_22_me");
extern const uint8_t u8g2_font_t0_22b_tf[] U8G2_FONT_SECTION("u8g2_font_t0_22b_tf");
extern const uint8_t u8g2_font_t0_22b_tr[] U8G2_FONT_SECTION("u8g2_font_t0_22b_tr");
extern const uint8_t u8g2_font_t0_22b_tn[] U8G2_FONT_SECTION("u8g2_font_t0_22b_tn");
extern const uint8_t u8g2_font_t0_22b_te[] U8G2_FONT_SECTION("u8g2_font_t0_22b_te");
extern const uint8_t u8g2_font_t0_22b_mf[] U8G2_FONT_SECTION("u8g2_font_t0_22b_mf");
extern const uint8_t u8g2_font_t0_22b_mr[] U8G2_FONT_SECTION("u8g2_font_t0_22b_mr");
extern const uint8_t u8g2_font_t0_22b_mn[] U8G2_FONT_SECTION("u8g2_font_t0_22b_mn");
extern const uint8_t u8g2_font_t0_22b_me[] U8G2_FONT_SECTION("u8g2_font_t0_22b_me");
extern const uint8_t u8g2_font_open_iconic_all_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_all_1x_t");
extern const uint8_t u8g2_font_open_iconic_app_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_app_1x_t");
extern const uint8_t u8g2_font_open_iconic_arrow_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_arrow_1x_t");
extern const uint8_t u8g2_font_open_iconic_check_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_check_1x_t");
extern const uint8_t u8g2_font_open_iconic_email_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_email_1x_t");
extern const uint8_t u8g2_font_open_iconic_embedded_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_embedded_1x_t");
extern const uint8_t u8g2_font_open_iconic_gui_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_gui_1x_t");
extern const uint8_t u8g2_font_open_iconic_human_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_human_1x_t");
extern const uint8_t u8g2_font_open_iconic_mime_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_mime_1x_t");
extern const uint8_t u8g2_font_open_iconic_other_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_other_1x_t");
extern const uint8_t u8g2_font_open_iconic_play_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_play_1x_t");
extern const uint8_t u8g2_font_open_iconic_text_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_text_1x_t");
extern const uint8_t u8g2_font_open_iconic_thing_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_thing_1x_t");
extern const uint8_t u8g2_font_open_iconic_weather_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_weather_1x_t");
extern const uint8_t u8g2_font_open_iconic_www_1x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_www_1x_t");
extern const uint8_t u8g2_font_open_iconic_all_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_all_2x_t");
extern const uint8_t u8g2_font_open_iconic_app_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_app_2x_t");
extern const uint8_t u8g2_font_open_iconic_arrow_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_arrow_2x_t");
extern const uint8_t u8g2_font_open_iconic_check_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_check_2x_t");
extern const uint8_t u8g2_font_open_iconic_email_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_email_2x_t");
extern const uint8_t u8g2_font_open_iconic_embedded_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_embedded_2x_t");
extern const uint8_t u8g2_font_open_iconic_gui_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_gui_2x_t");
extern const uint8_t u8g2_font_open_iconic_human_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_human_2x_t");
extern const uint8_t u8g2_font_open_iconic_mime_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_mime_2x_t");
extern const uint8_t u8g2_font_open_iconic_other_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_other_2x_t");
extern const uint8_t u8g2_font_open_iconic_play_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_play_2x_t");
extern const uint8_t u8g2_font_open_iconic_text_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_text_2x_t");
extern const uint8_t u8g2_font_open_iconic_thing_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_thing_2x_t");
extern const uint8_t u8g2_font_open_iconic_weather_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_weather_2x_t");
extern const uint8_t u8g2_font_open_iconic_www_2x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_www_2x_t");
extern const uint8_t u8g2_font_open_iconic_all_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_all_4x_t");
extern const uint8_t u8g2_font_open_iconic_app_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_app_4x_t");
extern const uint8_t u8g2_font_open_iconic_arrow_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_arrow_4x_t");
extern const uint8_t u8g2_font_open_iconic_check_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_check_4x_t");
extern const uint8_t u8g2_font_open_iconic_email_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_email_4x_t");
extern const uint8_t u8g2_font_open_iconic_embedded_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_embedded_4x_t");
extern const uint8_t u8g2_font_open_iconic_gui_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_gui_4x_t");
extern const uint8_t u8g2_font_open_iconic_human_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_human_4x_t");
extern const uint8_t u8g2_font_open_iconic_mime_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_mime_4x_t");
extern const uint8_t u8g2_font_open_iconic_other_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_other_4x_t");
extern const uint8_t u8g2_font_open_iconic_play_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_play_4x_t");
extern const uint8_t u8g2_font_open_iconic_text_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_text_4x_t");
extern const uint8_t u8g2_font_open_iconic_thing_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_thing_4x_t");
extern const uint8_t u8g2_font_open_iconic_weather_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_weather_4x_t");
extern const uint8_t u8g2_font_open_iconic_www_4x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_www_4x_t");
extern const uint8_t u8g2_font_open_iconic_all_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_all_6x_t");
extern const uint8_t u8g2_font_open_iconic_app_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_app_6x_t");
extern const uint8_t u8g2_font_open_iconic_arrow_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_arrow_6x_t");
extern const uint8_t u8g2_font_open_iconic_check_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_check_6x_t");
extern const uint8_t u8g2_font_open_iconic_email_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_email_6x_t");
extern const uint8_t u8g2_font_open_iconic_embedded_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_embedded_6x_t");
extern const uint8_t u8g2_font_open_iconic_gui_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_gui_6x_t");
extern const uint8_t u8g2_font_open_iconic_human_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_human_6x_t");
extern const uint8_t u8g2_font_open_iconic_mime_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_mime_6x_t");
extern const uint8_t u8g2_font_open_iconic_other_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_other_6x_t");
extern const uint8_t u8g2_font_open_iconic_play_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_play_6x_t");
extern const uint8_t u8g2_font_open_iconic_text_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_text_6x_t");
extern const uint8_t u8g2_font_open_iconic_thing_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_thing_6x_t");
extern const uint8_t u8g2_font_open_iconic_weather_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_weather_6x_t");
extern const uint8_t u8g2_font_open_iconic_www_6x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_www_6x_t");
extern const uint8_t u8g2_font_open_iconic_all_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_all_8x_t");
extern const uint8_t u8g2_font_open_iconic_app_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_app_8x_t");
extern const uint8_t u8g2_font_open_iconic_arrow_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_arrow_8x_t");
extern const uint8_t u8g2_font_open_iconic_check_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_check_8x_t");
extern const uint8_t u8g2_font_open_iconic_email_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_email_8x_t");
extern const uint8_t u8g2_font_open_iconic_embedded_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_embedded_8x_t");
extern const uint8_t u8g2_font_open_iconic_gui_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_gui_8x_t");
extern const uint8_t u8g2_font_open_iconic_human_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_human_8x_t");
extern const uint8_t u8g2_font_open_iconic_mime_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_mime_8x_t");
extern const uint8_t u8g2_font_open_iconic_other_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_other_8x_t");
extern const uint8_t u8g2_font_open_iconic_play_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_play_8x_t");
extern const uint8_t u8g2_font_open_iconic_text_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_text_8x_t");
extern const uint8_t u8g2_font_open_iconic_thing_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_thing_8x_t");
extern const uint8_t u8g2_font_open_iconic_weather_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_weather_8x_t");
extern const uint8_t u8g2_font_open_iconic_www_8x_t[] U8G2_FONT_SECTION("u8g2_font_open_iconic_www_8x_t");
extern const uint8_t u8g2_font_profont10_tf[] U8G2_FONT_SECTION("u8g2_font_profont10_tf");
extern const uint8_t u8g2_font_profont10_tr[] U8G2_FONT_SECTION("u8g2_font_profont10_tr");
extern const uint8_t u8g2_font_profont10_tn[] U8G2_FONT_SECTION("u8g2_font_profont10_tn");
extern const uint8_t u8g2_font_profont10_mf[] U8G2_FONT_SECTION("u8g2_font_profont10_mf");
extern const uint8_t u8g2_font_profont10_mr[] U8G2_FONT_SECTION("u8g2_font_profont10_mr");
extern const uint8_t u8g2_font_profont10_mn[] U8G2_FONT_SECTION("u8g2_font_profont10_mn");
extern const uint8_t u8g2_font_profont11_tf[] U8G2_FONT_SECTION("u8g2_font_profont11_tf");
extern const uint8_t u8g2_font_profont11_tr[] U8G2_FONT_SECTION("u8g2_font_profont11_tr");
extern const uint8_t u8g2_font_profont11_tn[] U8G2_FONT_SECTION("u8g2_font_profont11_tn");
extern const uint8_t u8g2_font_profont11_mf[] U8G2_FONT_SECTION("u8g2_font_profont11_mf");
extern const uint8_t u8g2_font_profont11_mr[] U8G2_FONT_SECTION("u8g2_font_profont11_mr");
extern const uint8_t u8g2_font_profont11_mn[] U8G2_FONT_SECTION("u8g2_font_profont11_mn");
extern const uint8_t u8g2_font_profont12_tf[] U8G2_FONT_SECTION("u8g2_font_profont12_tf");
extern const uint8_t u8g2_font_profont12_tr[] U8G2_FONT_SECTION("u8g2_font_profont12_tr");
extern const uint8_t u8g2_font_profont12_tn[] U8G2_FONT_SECTION("u8g2_font_profont12_tn");
extern const uint8_t u8g2_font_profont12_mf[] U8G2_FONT_SECTION("u8g2_font_profont12_mf");
extern const uint8_t u8g2_font_profont12_mr[] U8G2_FONT_SECTION("u8g2_font_profont12_mr");
extern const uint8_t u8g2_font_profont12_mn[] U8G2_FONT_SECTION("u8g2_font_profont12_mn");
extern const uint8_t u8g2_font_profont15_tf[] U8G2_FONT_SECTION("u8g2_font_profont15_tf");
extern const uint8_t u8g2_font_profont15_tr[] U8G2_FONT_SECTION("u8g2_font_profont15_tr");
extern const uint8_t u8g2_font_profont15_tn[] U8G2_FONT_SECTION("u8g2_font_profont15_tn");
extern const uint8_t u8g2_font_profont15_mf[] U8G2_FONT_SECTION("u8g2_font_profont15_mf");
extern const uint8_t u8g2_font_profont15_mr[] U8G2_FONT_SECTION("u8g2_font_profont15_mr");
extern const uint8_t u8g2_font_profont15_mn[] U8G2_FONT_SECTION("u8g2_font_profont15_mn");
extern const uint8_t u8g2_font_profont17_tf[] U8G2_FONT_SECTION("u8g2_font_profont17_tf");
extern const uint8_t u8g2_font_profont17_tr[] U8G2_FONT_SECTION("u8g2_font_profont17_tr");
extern const uint8_t u8g2_font_profont17_tn[] U8G2_FONT_SECTION("u8g2_font_profont17_tn");
extern const uint8_t u8g2_font_profont17_mf[] U8G2_FONT_SECTION("u8g2_font_profont17_mf");
extern const uint8_t u8g2_font_profont17_mr[] U8G2_FONT_SECTION("u8g2_font_profont17_mr");
extern const uint8_t u8g2_font_profont17_mn[] U8G2_FONT_SECTION("u8g2_font_profont17_mn");
extern const uint8_t u8g2_font_profont22_tf[] U8G2_FONT_SECTION("u8g2_font_profont22_tf");
extern const uint8_t u8g2_font_profont22_tr[] U8G2_FONT_SECTION("u8g2_font_profont22_tr");
extern const uint8_t u8g2_font_profont22_tn[] U8G2_FONT_SECTION("u8g2_font_profont22_tn");
extern const uint8_t u8g2_font_profont22_mf[] U8G2_FONT_SECTION("u8g2_font_profont22_mf");
extern const uint8_t u8g2_font_profont22_mr[] U8G2_FONT_SECTION("u8g2_font_profont22_mr");
extern const uint8_t u8g2_font_profont22_mn[] U8G2_FONT_SECTION("u8g2_font_profont22_mn");
extern const uint8_t u8g2_font_profont29_tf[] U8G2_FONT_SECTION("u8g2_font_profont29_tf");
extern const uint8_t u8g2_font_profont29_tr[] U8G2_FONT_SECTION("u8g2_font_profont29_tr");
extern const uint8_t u8g2_font_profont29_tn[] U8G2_FONT_SECTION("u8g2_font_profont29_tn");
extern const uint8_t u8g2_font_profont29_mf[] U8G2_FONT_SECTION("u8g2_font_profont29_mf");
extern const uint8_t u8g2_font_profont29_mr[] U8G2_FONT_SECTION("u8g2_font_profont29_mr");
extern const uint8_t u8g2_font_profont29_mn[] U8G2_FONT_SECTION("u8g2_font_profont29_mn");
extern const uint8_t u8g2_font_samim_10_t_all[] U8G2_FONT_SECTION("u8g2_font_samim_10_t_all");
extern const uint8_t u8g2_font_samim_12_t_all[] U8G2_FONT_SECTION("u8g2_font_samim_12_t_all");
extern const uint8_t u8g2_font_samim_14_t_all[] U8G2_FONT_SECTION("u8g2_font_samim_14_t_all");
extern const uint8_t u8g2_font_samim_16_t_all[] U8G2_FONT_SECTION("u8g2_font_samim_16_t_all");
extern const uint8_t u8g2_font_samim_fd_10_t_all[] U8G2_FONT_SECTION("u8g2_font_samim_fd_10_t_all");
extern const uint8_t u8g2_font_samim_fd_12_t_all[] U8G2_FONT_SECTION("u8g2_font_samim_fd_12_t_all");
extern const uint8_t u8g2_font_samim_fd_14_t_all[] U8G2_FONT_SECTION("u8g2_font_samim_fd_14_t_all");
extern const uint8_t u8g2_font_samim_fd_16_t_all[] U8G2_FONT_SECTION("u8g2_font_samim_fd_16_t_all");
extern const uint8_t u8g2_font_ganj_nameh_sans10_t_all[] U8G2_FONT_SECTION("u8g2_font_ganj_nameh_sans10_t_all");
extern const uint8_t u8g2_font_ganj_nameh_sans12_t_all[] U8G2_FONT_SECTION("u8g2_font_ganj_nameh_sans12_t_all");
extern const uint8_t u8g2_font_ganj_nameh_sans14_t_all[] U8G2_FONT_SECTION("u8g2_font_ganj_nameh_sans14_t_all");
extern const uint8_t u8g2_font_ganj_nameh_sans16_t_all[] U8G2_FONT_SECTION("u8g2_font_ganj_nameh_sans16_t_all");
extern const uint8_t u8g2_font_iranian_sans_8_t_all[] U8G2_FONT_SECTION("u8g2_font_iranian_sans_8_t_all");
extern const uint8_t u8g2_font_iranian_sans_10_t_all[] U8G2_FONT_SECTION("u8g2_font_iranian_sans_10_t_all");
extern const uint8_t u8g2_font_iranian_sans_12_t_all[] U8G2_FONT_SECTION("u8g2_font_iranian_sans_12_t_all");
extern const uint8_t u8g2_font_iranian_sans_14_t_all[] U8G2_FONT_SECTION("u8g2_font_iranian_sans_14_t_all");
extern const uint8_t u8g2_font_iranian_sans_16_t_all[] U8G2_FONT_SECTION("u8g2_font_iranian_sans_16_t_all");
extern const uint8_t u8g2_font_mozart_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_tf");
extern const uint8_t u8g2_font_mozart_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_tr");
extern const uint8_t u8g2_font_mozart_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_tn");
extern const uint8_t u8g2_font_mozart_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_t_all");
extern const uint8_t u8g2_font_mozart_nbp_h_all[] U8G2_FONT_SECTION("u8g2_font_mozart_nbp_h_all");
extern const uint8_t u8g2_font_glasstown_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_glasstown_nbp_tf");
extern const uint8_t u8g2_font_glasstown_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_glasstown_nbp_tr");
extern const uint8_t u8g2_font_glasstown_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_glasstown_nbp_tn");
extern const uint8_t u8g2_font_glasstown_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_glasstown_nbp_t_all");
extern const uint8_t u8g2_font_shylock_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_shylock_nbp_tf");
extern const uint8_t u8g2_font_shylock_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_shylock_nbp_tr");
extern const uint8_t u8g2_font_shylock_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_shylock_nbp_tn");
extern const uint8_t u8g2_font_shylock_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_shylock_nbp_t_all");
extern const uint8_t u8g2_font_roentgen_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_tf");
extern const uint8_t u8g2_font_roentgen_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_tr");
extern const uint8_t u8g2_font_roentgen_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_tn");
extern const uint8_t u8g2_font_roentgen_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_t_all");
extern const uint8_t u8g2_font_roentgen_nbp_h_all[] U8G2_FONT_SECTION("u8g2_font_roentgen_nbp_h_all");
extern const uint8_t u8g2_font_calibration_gothic_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_calibration_gothic_nbp_tf");
extern const uint8_t u8g2_font_calibration_gothic_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_calibration_gothic_nbp_tr");
extern const uint8_t u8g2_font_calibration_gothic_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_calibration_gothic_nbp_tn");
extern const uint8_t u8g2_font_calibration_gothic_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_calibration_gothic_nbp_t_all");
extern const uint8_t u8g2_font_smart_patrol_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_smart_patrol_nbp_tf");
extern const uint8_t u8g2_font_smart_patrol_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_smart_patrol_nbp_tr");
extern const uint8_t u8g2_font_smart_patrol_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_smart_patrol_nbp_tn");
extern const uint8_t u8g2_font_prospero_bold_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_prospero_bold_nbp_tf");
extern const uint8_t u8g2_font_prospero_bold_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_prospero_bold_nbp_tr");
extern const uint8_t u8g2_font_prospero_bold_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_prospero_bold_nbp_tn");
extern const uint8_t u8g2_font_prospero_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_prospero_nbp_tf");
extern const uint8_t u8g2_font_prospero_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_prospero_nbp_tr");
extern const uint8_t u8g2_font_prospero_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_prospero_nbp_tn");
extern const uint8_t u8g2_font_balthasar_regular_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_balthasar_regular_nbp_tf");
extern const uint8_t u8g2_font_balthasar_regular_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_balthasar_regular_nbp_tr");
extern const uint8_t u8g2_font_balthasar_regular_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_balthasar_regular_nbp_tn");
extern const uint8_t u8g2_font_balthasar_titling_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_balthasar_titling_nbp_tf");
extern const uint8_t u8g2_font_balthasar_titling_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_balthasar_titling_nbp_tr");
extern const uint8_t u8g2_font_balthasar_titling_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_balthasar_titling_nbp_tn");
extern const uint8_t u8g2_font_synchronizer_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_synchronizer_nbp_tf");
extern const uint8_t u8g2_font_synchronizer_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_synchronizer_nbp_tr");
extern const uint8_t u8g2_font_synchronizer_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_synchronizer_nbp_tn");
extern const uint8_t u8g2_font_mercutio_basic_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_mercutio_basic_nbp_tf");
extern const uint8_t u8g2_font_mercutio_basic_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_mercutio_basic_nbp_tr");
extern const uint8_t u8g2_font_mercutio_basic_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_mercutio_basic_nbp_tn");
extern const uint8_t u8g2_font_mercutio_basic_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_mercutio_basic_nbp_t_all");
extern const uint8_t u8g2_font_mercutio_sc_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_mercutio_sc_nbp_tf");
extern const uint8_t u8g2_font_mercutio_sc_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_mercutio_sc_nbp_tr");
extern const uint8_t u8g2_font_mercutio_sc_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_mercutio_sc_nbp_tn");
extern const uint8_t u8g2_font_mercutio_sc_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_mercutio_sc_nbp_t_all");
extern const uint8_t u8g2_font_miranda_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_miranda_nbp_tf");
extern const uint8_t u8g2_font_miranda_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_miranda_nbp_tr");
extern const uint8_t u8g2_font_miranda_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_miranda_nbp_tn");
extern const uint8_t u8g2_font_nine_by_five_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_nine_by_five_nbp_tf");
extern const uint8_t u8g2_font_nine_by_five_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_nine_by_five_nbp_tr");
extern const uint8_t u8g2_font_nine_by_five_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_nine_by_five_nbp_tn");
extern const uint8_t u8g2_font_nine_by_five_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_nine_by_five_nbp_t_all");
extern const uint8_t u8g2_font_rosencrantz_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_rosencrantz_nbp_tf");
extern const uint8_t u8g2_font_rosencrantz_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_rosencrantz_nbp_tr");
extern const uint8_t u8g2_font_rosencrantz_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_rosencrantz_nbp_tn");
extern const uint8_t u8g2_font_rosencrantz_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_rosencrantz_nbp_t_all");
extern const uint8_t u8g2_font_guildenstern_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_guildenstern_nbp_tf");
extern const uint8_t u8g2_font_guildenstern_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_guildenstern_nbp_tr");
extern const uint8_t u8g2_font_guildenstern_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_guildenstern_nbp_tn");
extern const uint8_t u8g2_font_guildenstern_nbp_t_all[] U8G2_FONT_SECTION("u8g2_font_guildenstern_nbp_t_all");
extern const uint8_t u8g2_font_astragal_nbp_tf[] U8G2_FONT_SECTION("u8g2_font_astragal_nbp_tf");
extern const uint8_t u8g2_font_astragal_nbp_tr[] U8G2_FONT_SECTION("u8g2_font_astragal_nbp_tr");
extern const uint8_t u8g2_font_astragal_nbp_tn[] U8G2_FONT_SECTION("u8g2_font_astragal_nbp_tn");
extern const uint8_t u8g2_font_habsburgchancery_tf[] U8G2_FONT_SECTION("u8g2_font_habsburgchancery_tf");
extern const uint8_t u8g2_font_habsburgchancery_tr[] U8G2_FONT_SECTION("u8g2_font_habsburgchancery_tr");
extern const uint8_t u8g2_font_habsburgchancery_tn[] U8G2_FONT_SECTION("u8g2_font_habsburgchancery_tn");
extern const uint8_t u8g2_font_habsburgchancery_t_all[] U8G2_FONT_SECTION("u8g2_font_habsburgchancery_t_all");
extern const uint8_t u8g2_font_missingplanet_tf[] U8G2_FONT_SECTION("u8g2_font_missingplanet_tf");
extern const uint8_t u8g2_font_missingplanet_tr[] U8G2_FONT_SECTION("u8g2_font_missingplanet_tr");
extern const uint8_t u8g2_font_missingplanet_tn[] U8G2_FONT_SECTION("u8g2_font_missingplanet_tn");
extern const uint8_t u8g2_font_missingplanet_t_all[] U8G2_FONT_SECTION("u8g2_font_missingplanet_t_all");
extern const uint8_t u8g2_font_ordinarybasis_tf[] U8G2_FONT_SECTION("u8g2_font_ordinarybasis_tf");
extern const uint8_t u8g2_font_ordinarybasis_tr[] U8G2_FONT_SECTION("u8g2_font_ordinarybasis_tr");
extern const uint8_t u8g2_font_ordinarybasis_tn[] U8G2_FONT_SECTION("u8g2_font_ordinarybasis_tn");
extern const uint8_t u8g2_font_ordinarybasis_t_all[] U8G2_FONT_SECTION("u8g2_font_ordinarybasis_t_all");
extern const uint8_t u8g2_font_pixelmordred_tf[] U8G2_FONT_SECTION("u8g2_font_pixelmordred_tf");
extern const uint8_t u8g2_font_pixelmordred_tr[] U8G2_FONT_SECTION("u8g2_font_pixelmordred_tr");
extern const uint8_t u8g2_font_pixelmordred_tn[] U8G2_FONT_SECTION("u8g2_font_pixelmordred_tn");
extern const uint8_t u8g2_font_pixelmordred_t_all[] U8G2_FONT_SECTION("u8g2_font_pixelmordred_t_all");
extern const uint8_t u8g2_font_secretaryhand_tf[] U8G2_FONT_SECTION("u8g2_font_secretaryhand_tf");
extern const uint8_t u8g2_font_secretaryhand_tr[] U8G2_FONT_SECTION("u8g2_font_secretaryhand_tr");
extern const uint8_t u8g2_font_secretaryhand_tn[] U8G2_FONT_SECTION("u8g2_font_secretaryhand_tn");
extern const uint8_t u8g2_font_secretaryhand_t_all[] U8G2_FONT_SECTION("u8g2_font_secretaryhand_t_all");
extern const uint8_t u8g2_font_beanstalk_mel_tr[] U8G2_FONT_SECTION("u8g2_font_beanstalk_mel_tr");
extern const uint8_t u8g2_font_beanstalk_mel_tn[] U8G2_FONT_SECTION("u8g2_font_beanstalk_mel_tn");
extern const uint8_t u8g2_font_cube_mel_tr[] U8G2_FONT_SECTION("u8g2_font_cube_mel_tr");
extern const uint8_t u8g2_font_cube_mel_tn[] U8G2_FONT_SECTION("u8g2_font_cube_mel_tn");
extern const uint8_t u8g2_font_mademoiselle_mel_tr[] U8G2_FONT_SECTION("u8g2_font_mademoiselle_mel_tr");
extern const uint8_t u8g2_font_mademoiselle_mel_tn[] U8G2_FONT_SECTION("u8g2_font_mademoiselle_mel_tn");
extern const uint8_t u8g2_font_pieceofcake_mel_tr[] U8G2_FONT_SECTION("u8g2_font_pieceofcake_mel_tr");
extern const uint8_t u8g2_font_pieceofcake_mel_tn[] U8G2_FONT_SECTION("u8g2_font_pieceofcake_mel_tn");
extern const uint8_t u8g2_font_press_mel_tr[] U8G2_FONT_SECTION("u8g2_font_press_mel_tr");
extern const uint8_t u8g2_font_press_mel_tn[] U8G2_FONT_SECTION("u8g2_font_press_mel_tn");
extern const uint8_t u8g2_font_repress_mel_tr[] U8G2_FONT_SECTION("u8g2_font_repress_mel_tr");
extern const uint8_t u8g2_font_repress_mel_tn[] U8G2_FONT_SECTION("u8g2_font_repress_mel_tn");
extern const uint8_t u8g2_font_sticker_mel_tr[] U8G2_FONT_SECTION("u8g2_font_sticker_mel_tr");
extern const uint8_t u8g2_font_sticker_mel_tn[] U8G2_FONT_SECTION("u8g2_font_sticker_mel_tn");
extern const uint8_t u8g2_font_celibatemonk_tr[] U8G2_FONT_SECTION("u8g2_font_celibatemonk_tr");
extern const uint8_t u8g2_font_disrespectfulteenager_tu[] U8G2_FONT_SECTION("u8g2_font_disrespectfulteenager_tu");
extern const uint8_t u8g2_font_michaelmouse_tu[] U8G2_FONT_SECTION("u8g2_font_michaelmouse_tu");
extern const uint8_t u8g2_font_sandyforest_tr[] U8G2_FONT_SECTION("u8g2_font_sandyforest_tr");
extern const uint8_t u8g2_font_sandyforest_tn[] U8G2_FONT_SECTION("u8g2_font_sandyforest_tn");
extern const uint8_t u8g2_font_sandyforest_tu[] U8G2_FONT_SECTION("u8g2_font_sandyforest_tu");
extern const uint8_t u8g2_font_cupcakemetoyourleader_tr[] U8G2_FONT_SECTION("u8g2_font_cupcakemetoyourleader_tr");
extern const uint8_t u8g2_font_cupcakemetoyourleader_tn[] U8G2_FONT_SECTION("u8g2_font_cupcakemetoyourleader_tn");
extern const uint8_t u8g2_font_cupcakemetoyourleader_tu[] U8G2_FONT_SECTION("u8g2_font_cupcakemetoyourleader_tu");
extern const uint8_t u8g2_font_oldwizard_tf[] U8G2_FONT_SECTION("u8g2_font_oldwizard_tf");
extern const uint8_t u8g2_font_oldwizard_tr[] U8G2_FONT_SECTION("u8g2_font_oldwizard_tr");
extern const uint8_t u8g2_font_oldwizard_tn[] U8G2_FONT_SECTION("u8g2_font_oldwizard_tn");
extern const uint8_t u8g2_font_oldwizard_tu[] U8G2_FONT_SECTION("u8g2_font_oldwizard_tu");
extern const uint8_t u8g2_font_squirrel_tr[] U8G2_FONT_SECTION("u8g2_font_squirrel_tr");
extern const uint8_t u8g2_font_squirrel_tn[] U8G2_FONT_SECTION("u8g2_font_squirrel_tn");
extern const uint8_t u8g2_font_squirrel_tu[] U8G2_FONT_SECTION("u8g2_font_squirrel_tu");
extern const uint8_t u8g2_font_diodesemimono_tr[] U8G2_FONT_SECTION("u8g2_font_diodesemimono_tr");
extern const uint8_t u8g2_font_questgiver_tr[] U8G2_FONT_SECTION("u8g2_font_questgiver_tr");
extern const uint8_t u8g2_font_seraphimb1_tr[] U8G2_FONT_SECTION("u8g2_font_seraphimb1_tr");
extern const uint8_t u8g2_font_jinxedwizards_tr[] U8G2_FONT_SECTION("u8g2_font_jinxedwizards_tr");
extern const uint8_t u8g2_font_lastpriestess_tr[] U8G2_FONT_SECTION("u8g2_font_lastpriestess_tr");
extern const uint8_t u8g2_font_lastpriestess_tu[] U8G2_FONT_SECTION("u8g2_font_lastpriestess_tu");
extern const uint8_t u8g2_font_bitcasual_tf[] U8G2_FONT_SECTION("u8g2_font_bitcasual_tf");
extern const uint8_t u8g2_font_bitcasual_tr[] U8G2_FONT_SECTION("u8g2_font_bitcasual_tr");
extern const uint8_t u8g2_font_bitcasual_tn[] U8G2_FONT_SECTION("u8g2_font_bitcasual_tn");
extern const uint8_t u8g2_font_bitcasual_tu[] U8G2_FONT_SECTION("u8g2_font_bitcasual_tu");
extern const uint8_t u8g2_font_bitcasual_t_all[] U8G2_FONT_SECTION("u8g2_font_bitcasual_t_all");
extern const uint8_t u8g2_font_koleeko_tf[] U8G2_FONT_SECTION("u8g2_font_koleeko_tf");
extern const uint8_t u8g2_font_koleeko_tr[] U8G2_FONT_SECTION("u8g2_font_koleeko_tr");
extern const uint8_t u8g2_font_koleeko_tn[] U8G2_FONT_SECTION("u8g2_font_koleeko_tn");
extern const uint8_t u8g2_font_koleeko_tu[] U8G2_FONT_SECTION("u8g2_font_koleeko_tu");
extern const uint8_t u8g2_font_tenfatguys_tf[] U8G2_FONT_SECTION("u8g2_font_tenfatguys_tf");
extern const uint8_t u8g2_font_tenfatguys_tr[] U8G2_FONT_SECTION("u8g2_font_tenfatguys_tr");
extern const uint8_t u8g2_font_tenfatguys_tn[] U8G2_FONT_SECTION("u8g2_font_tenfatguys_tn");
extern const uint8_t u8g2_font_tenfatguys_tu[] U8G2_FONT_SECTION("u8g2_font_tenfatguys_tu");
extern const uint8_t u8g2_font_tenfatguys_t_all[] U8G2_FONT_SECTION("u8g2_font_tenfatguys_t_all");
extern const uint8_t u8g2_font_tenstamps_mf[] U8G2_FONT_SECTION("u8g2_font_tenstamps_mf");
extern const uint8_t u8g2_font_tenstamps_mr[] U8G2_FONT_SECTION("u8g2_font_tenstamps_mr");
extern const uint8_t u8g2_font_tenstamps_mn[] U8G2_FONT_SECTION("u8g2_font_tenstamps_mn");
extern const uint8_t u8g2_font_tenstamps_mu[] U8G2_FONT_SECTION("u8g2_font_tenstamps_mu");
extern const uint8_t u8g2_font_tenthinguys_tf[] U8G2_FONT_SECTION("u8g2_font_tenthinguys_tf");
extern const uint8_t u8g2_font_tenthinguys_tr[] U8G2_FONT_SECTION("u8g2_font_tenthinguys_tr");
extern const uint8_t u8g2_font_tenthinguys_tn[] U8G2_FONT_SECTION("u8g2_font_tenthinguys_tn");
extern const uint8_t u8g2_font_tenthinguys_tu[] U8G2_FONT_SECTION("u8g2_font_tenthinguys_tu");
extern const uint8_t u8g2_font_tenthinguys_t_all[] U8G2_FONT_SECTION("u8g2_font_tenthinguys_t_all");
extern const uint8_t u8g2_font_tenthinnerguys_tf[] U8G2_FONT_SECTION("u8g2_font_tenthinnerguys_tf");
extern const uint8_t u8g2_font_tenthinnerguys_tr[] U8G2_FONT_SECTION("u8g2_font_tenthinnerguys_tr");
extern const uint8_t u8g2_font_tenthinnerguys_tn[] U8G2_FONT_SECTION("u8g2_font_tenthinnerguys_tn");
extern const uint8_t u8g2_font_tenthinnerguys_tu[] U8G2_FONT_SECTION("u8g2_font_tenthinnerguys_tu");
extern const uint8_t u8g2_font_tenthinnerguys_t_all[] U8G2_FONT_SECTION("u8g2_font_tenthinnerguys_t_all");
extern const uint8_t u8g2_font_twelvedings_t_all[] U8G2_FONT_SECTION("u8g2_font_twelvedings_t_all");
extern const uint8_t u8g2_font_fewture_tf[] U8G2_FONT_SECTION("u8g2_font_fewture_tf");
extern const uint8_t u8g2_font_fewture_tr[] U8G2_FONT_SECTION("u8g2_font_fewture_tr");
extern const uint8_t u8g2_font_fewture_tn[] U8G2_FONT_SECTION("u8g2_font_fewture_tn");
extern const uint8_t u8g2_font_halftone_tf[] U8G2_FONT_SECTION("u8g2_font_halftone_tf");
extern const uint8_t u8g2_font_halftone_tr[] U8G2_FONT_SECTION("u8g2_font_halftone_tr");
extern const uint8_t u8g2_font_halftone_tn[] U8G2_FONT_SECTION("u8g2_font_halftone_tn");
extern const uint8_t u8g2_font_nerhoe_tf[] U8G2_FONT_SECTION("u8g2_font_nerhoe_tf");
extern const uint8_t u8g2_font_nerhoe_tr[] U8G2_FONT_SECTION("u8g2_font_nerhoe_tr");
extern const uint8_t u8g2_font_nerhoe_tn[] U8G2_FONT_SECTION("u8g2_font_nerhoe_tn");
extern const uint8_t u8g2_font_oskool_tf[] U8G2_FONT_SECTION("u8g2_font_oskool_tf");
extern const uint8_t u8g2_font_oskool_tr[] U8G2_FONT_SECTION("u8g2_font_oskool_tr");
extern const uint8_t u8g2_font_oskool_tn[] U8G2_FONT_SECTION("u8g2_font_oskool_tn");
extern const uint8_t u8g2_font_tinytim_tf[] U8G2_FONT_SECTION("u8g2_font_tinytim_tf");
extern const uint8_t u8g2_font_tinytim_tr[] U8G2_FONT_SECTION("u8g2_font_tinytim_tr");
extern const uint8_t u8g2_font_tinytim_tn[] U8G2_FONT_SECTION("u8g2_font_tinytim_tn");
extern const uint8_t u8g2_font_tooseornament_tf[] U8G2_FONT_SECTION("u8g2_font_tooseornament_tf");
extern const uint8_t u8g2_font_tooseornament_tr[] U8G2_FONT_SECTION("u8g2_font_tooseornament_tr");
extern const uint8_t u8g2_font_tooseornament_tn[] U8G2_FONT_SECTION("u8g2_font_tooseornament_tn");
extern const uint8_t u8g2_font_bauhaus2015_tr[] U8G2_FONT_SECTION("u8g2_font_bauhaus2015_tr");
extern const uint8_t u8g2_font_bauhaus2015_tn[] U8G2_FONT_SECTION("u8g2_font_bauhaus2015_tn");
extern const uint8_t u8g2_font_finderskeepers_tf[] U8G2_FONT_SECTION("u8g2_font_finderskeepers_tf");
extern const uint8_t u8g2_font_finderskeepers_tr[] U8G2_FONT_SECTION("u8g2_font_finderskeepers_tr");
extern const uint8_t u8g2_font_finderskeepers_tn[] U8G2_FONT_SECTION("u8g2_font_finderskeepers_tn");
extern const uint8_t u8g2_font_sirclivethebold_tr[] U8G2_FONT_SECTION("u8g2_font_sirclivethebold_tr");
extern const uint8_t u8g2_font_sirclivethebold_tn[] U8G2_FONT_SECTION("u8g2_font_sirclivethebold_tn");
extern const uint8_t u8g2_font_sirclive_tr[] U8G2_FONT_SECTION("u8g2_font_sirclive_tr");
extern const uint8_t u8g2_font_sirclive_tn[] U8G2_FONT_SECTION("u8g2_font_sirclive_tn");
extern const uint8_t u8g2_font_adventurer_tf[] U8G2_FONT_SECTION("u8g2_font_adventurer_tf");
extern const uint8_t u8g2_font_adventurer_tr[] U8G2_FONT_SECTION("u8g2_font_adventurer_tr");
extern const uint8_t u8g2_font_adventurer_t_all[] U8G2_FONT_SECTION("u8g2_font_adventurer_t_all");
extern const uint8_t u8g2_font_bracketedbabies_tr[] U8G2_FONT_SECTION("u8g2_font_bracketedbabies_tr");
extern const uint8_t u8g2_font_frikativ_tf[] U8G2_FONT_SECTION("u8g2_font_frikativ_tf");
extern const uint8_t u8g2_font_frikativ_tr[] U8G2_FONT_SECTION("u8g2_font_frikativ_tr");
extern const uint8_t u8g2_font_frikativ_t_all[] U8G2_FONT_SECTION("u8g2_font_frikativ_t_all");
extern const uint8_t u8g2_font_fancypixels_tf[] U8G2_FONT_SECTION("u8g2_font_fancypixels_tf");
extern const uint8_t u8g2_font_fancypixels_tr[] U8G2_FONT_SECTION("u8g2_font_fancypixels_tr");
extern const uint8_t u8g2_font_heavybottom_tr[] U8G2_FONT_SECTION("u8g2_font_heavybottom_tr");
extern const uint8_t u8g2_font_iconquadpix_m_all[] U8G2_FONT_SECTION("u8g2_font_iconquadpix_m_all");
extern const uint8_t u8g2_font_lastapprenticebold_tr[] U8G2_FONT_SECTION("u8g2_font_lastapprenticebold_tr");
extern const uint8_t u8g2_font_lastapprenticethin_tr[] U8G2_FONT_SECTION("u8g2_font_lastapprenticethin_tr");
extern const uint8_t u8g2_font_tallpix_tr[] U8G2_FONT_SECTION("u8g2_font_tallpix_tr");
extern const uint8_t u8g2_font_BBSesque_tf[] U8G2_FONT_SECTION("u8g2_font_BBSesque_tf");
extern const uint8_t u8g2_font_BBSesque_tr[] U8G2_FONT_SECTION("u8g2_font_BBSesque_tr");
extern const uint8_t u8g2_font_BBSesque_te[] U8G2_FONT_SECTION("u8g2_font_BBSesque_te");
extern const uint8_t u8g2_font_Born2bSportySlab_tf[] U8G2_FONT_SECTION("u8g2_font_Born2bSportySlab_tf");
extern const uint8_t u8g2_font_Born2bSportySlab_tr[] U8G2_FONT_SECTION("u8g2_font_Born2bSportySlab_tr");
extern const uint8_t u8g2_font_Born2bSportySlab_te[] U8G2_FONT_SECTION("u8g2_font_Born2bSportySlab_te");
extern const uint8_t u8g2_font_Born2bSportySlab_t_all[] U8G2_FONT_SECTION("u8g2_font_Born2bSportySlab_t_all");
extern const uint8_t u8g2_font_Born2bSportyV2_tf[] U8G2_FONT_SECTION("u8g2_font_Born2bSportyV2_tf");
extern const uint8_t u8g2_font_Born2bSportyV2_tr[] U8G2_FONT_SECTION("u8g2_font_Born2bSportyV2_tr");
extern const uint8_t u8g2_font_Born2bSportyV2_te[] U8G2_FONT_SECTION("u8g2_font_Born2bSportyV2_te");
extern const uint8_t u8g2_font_CursivePixel_tr[] U8G2_FONT_SECTION("u8g2_font_CursivePixel_tr");
extern const uint8_t u8g2_font_Engrish_tf[] U8G2_FONT_SECTION("u8g2_font_Engrish_tf");
extern const uint8_t u8g2_font_Engrish_tr[] U8G2_FONT_SECTION("u8g2_font_Engrish_tr");
extern const uint8_t u8g2_font_ImpactBits_tr[] U8G2_FONT_SECTION("u8g2_font_ImpactBits_tr");
extern const uint8_t u8g2_font_IPAandRUSLCD_tf[] U8G2_FONT_SECTION("u8g2_font_IPAandRUSLCD_tf");
extern const uint8_t u8g2_font_IPAandRUSLCD_tr[] U8G2_FONT_SECTION("u8g2_font_IPAandRUSLCD_tr");
extern const uint8_t u8g2_font_IPAandRUSLCD_te[] U8G2_FONT_SECTION("u8g2_font_IPAandRUSLCD_te");
extern const uint8_t u8g2_font_HelvetiPixel_tr[] U8G2_FONT_SECTION("u8g2_font_HelvetiPixel_tr");
extern const uint8_t u8g2_font_TimesNewPixel_tr[] U8G2_FONT_SECTION("u8g2_font_TimesNewPixel_tr");
extern const uint8_t u8g2_font_BitTypeWriter_tr[] U8G2_FONT_SECTION("u8g2_font_BitTypeWriter_tr");
extern const uint8_t u8g2_font_BitTypeWriter_te[] U8G2_FONT_SECTION("u8g2_font_BitTypeWriter_te");
extern const uint8_t u8g2_font_Georgia7px_tf[] U8G2_FONT_SECTION("u8g2_font_Georgia7px_tf");
extern const uint8_t u8g2_font_Georgia7px_tr[] U8G2_FONT_SECTION("u8g2_font_Georgia7px_tr");
extern const uint8_t u8g2_font_Georgia7px_te[] U8G2_FONT_SECTION("u8g2_font_Georgia7px_te");
extern const uint8_t u8g2_font_Wizzard_tr[] U8G2_FONT_SECTION("u8g2_font_Wizzard_tr");
extern const uint8_t u8g2_font_HelvetiPixelOutline_tr[] U8G2_FONT_SECTION("u8g2_font_HelvetiPixelOutline_tr");
extern const uint8_t u8g2_font_HelvetiPixelOutline_te[] U8G2_FONT_SECTION("u8g2_font_HelvetiPixelOutline_te");
extern const uint8_t u8g2_font_Untitled16PixelSansSerifBitmap_tr[] U8G2_FONT_SECTION("u8g2_font_Untitled16PixelSansSerifBitmap_tr");
extern const uint8_t u8g2_font_etl14thai_t[] U8G2_FONT_SECTION("u8g2_font_etl14thai_t");
extern const uint8_t u8g2_font_etl16thai_t[] U8G2_FONT_SECTION("u8g2_font_etl16thai_t");
extern const uint8_t u8g2_font_etl24thai_t[] U8G2_FONT_SECTION("u8g2_font_etl24thai_t");
extern const uint8_t u8g2_font_crox1cb_tf[] U8G2_FONT_SECTION("u8g2_font_crox1cb_tf");
extern const uint8_t u8g2_font_crox1cb_tr[] U8G2_FONT_SECTION("u8g2_font_crox1cb_tr");
extern const uint8_t u8g2_font_crox1cb_tn[] U8G2_FONT_SECTION("u8g2_font_crox1cb_tn");
extern const uint8_t u8g2_font_crox1cb_mf[] U8G2_FONT_SECTION("u8g2_font_crox1cb_mf");
extern const uint8_t u8g2_font_crox1cb_mr[] U8G2_FONT_SECTION("u8g2_font_crox1cb_mr");
extern const uint8_t u8g2_font_crox1cb_mn[] U8G2_FONT_SECTION("u8g2_font_crox1cb_mn");
extern const uint8_t u8g2_font_crox1c_tf[] U8G2_FONT_SECTION("u8g2_font_crox1c_tf");
extern const uint8_t u8g2_font_crox1c_tr[] U8G2_FONT_SECTION("u8g2_font_crox1c_tr");
extern const uint8_t u8g2_font_crox1c_tn[] U8G2_FONT_SECTION("u8g2_font_crox1c_tn");
extern const uint8_t u8g2_font_crox1c_mf[] U8G2_FONT_SECTION("u8g2_font_crox1c_mf");
extern const uint8_t u8g2_font_crox1c_mr[] U8G2_FONT_SECTION("u8g2_font_crox1c_mr");
extern const uint8_t u8g2_font_crox1c_mn[] U8G2_FONT_SECTION("u8g2_font_crox1c_mn");
extern const uint8_t u8g2_font_crox1hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox1hb_tf");
extern const uint8_t u8g2_font_crox1hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox1hb_tr");
extern const uint8_t u8g2_font_crox1hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox1hb_tn");
extern const uint8_t u8g2_font_crox1h_tf[] U8G2_FONT_SECTION("u8g2_font_crox1h_tf");
extern const uint8_t u8g2_font_crox1h_tr[] U8G2_FONT_SECTION("u8g2_font_crox1h_tr");
extern const uint8_t u8g2_font_crox1h_tn[] U8G2_FONT_SECTION("u8g2_font_crox1h_tn");
extern const uint8_t u8g2_font_crox1tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox1tb_tf");
extern const uint8_t u8g2_font_crox1tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox1tb_tr");
extern const uint8_t u8g2_font_crox1tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox1tb_tn");
extern const uint8_t u8g2_font_crox1t_tf[] U8G2_FONT_SECTION("u8g2_font_crox1t_tf");
extern const uint8_t u8g2_font_crox1t_tr[] U8G2_FONT_SECTION("u8g2_font_crox1t_tr");
extern const uint8_t u8g2_font_crox1t_tn[] U8G2_FONT_SECTION("u8g2_font_crox1t_tn");
extern const uint8_t u8g2_font_crox2cb_tf[] U8G2_FONT_SECTION("u8g2_font_crox2cb_tf");
extern const uint8_t u8g2_font_crox2cb_tr[] U8G2_FONT_SECTION("u8g2_font_crox2cb_tr");
extern const uint8_t u8g2_font_crox2cb_tn[] U8G2_FONT_SECTION("u8g2_font_crox2cb_tn");
extern const uint8_t u8g2_font_crox2cb_mf[] U8G2_FONT_SECTION("u8g2_font_crox2cb_mf");
extern const uint8_t u8g2_font_crox2cb_mr[] U8G2_FONT_SECTION("u8g2_font_crox2cb_mr");
extern const uint8_t u8g2_font_crox2cb_mn[] U8G2_FONT_SECTION("u8g2_font_crox2cb_mn");
extern const uint8_t u8g2_font_crox2c_tf[] U8G2_FONT_SECTION("u8g2_font_crox2c_tf");
extern const uint8_t u8g2_font_crox2c_tr[] U8G2_FONT_SECTION("u8g2_font_crox2c_tr");
extern const uint8_t u8g2_font_crox2c_tn[] U8G2_FONT_SECTION("u8g2_font_crox2c_tn");
extern const uint8_t u8g2_font_crox2c_mf[] U8G2_FONT_SECTION("u8g2_font_crox2c_mf");
extern const uint8_t u8g2_font_crox2c_mr[] U8G2_FONT_SECTION("u8g2_font_crox2c_mr");
extern const uint8_t u8g2_font_crox2c_mn[] U8G2_FONT_SECTION("u8g2_font_crox2c_mn");
extern const uint8_t u8g2_font_crox2hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox2hb_tf");
extern const uint8_t u8g2_font_crox2hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox2hb_tr");
extern const uint8_t u8g2_font_crox2hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox2hb_tn");
extern const uint8_t u8g2_font_crox2h_tf[] U8G2_FONT_SECTION("u8g2_font_crox2h_tf");
extern const uint8_t u8g2_font_crox2h_tr[] U8G2_FONT_SECTION("u8g2_font_crox2h_tr");
extern const uint8_t u8g2_font_crox2h_tn[] U8G2_FONT_SECTION("u8g2_font_crox2h_tn");
extern const uint8_t u8g2_font_crox2tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox2tb_tf");
extern const uint8_t u8g2_font_crox2tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox2tb_tr");
extern const uint8_t u8g2_font_crox2tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox2tb_tn");
extern const uint8_t u8g2_font_crox2t_tf[] U8G2_FONT_SECTION("u8g2_font_crox2t_tf");
extern const uint8_t u8g2_font_crox2t_tr[] U8G2_FONT_SECTION("u8g2_font_crox2t_tr");
extern const uint8_t u8g2_font_crox2t_tn[] U8G2_FONT_SECTION("u8g2_font_crox2t_tn");
extern const uint8_t u8g2_font_crox3cb_tf[] U8G2_FONT_SECTION("u8g2_font_crox3cb_tf");
extern const uint8_t u8g2_font_crox3cb_tr[] U8G2_FONT_SECTION("u8g2_font_crox3cb_tr");
extern const uint8_t u8g2_font_crox3cb_tn[] U8G2_FONT_SECTION("u8g2_font_crox3cb_tn");
extern const uint8_t u8g2_font_crox3cb_mf[] U8G2_FONT_SECTION("u8g2_font_crox3cb_mf");
extern const uint8_t u8g2_font_crox3cb_mr[] U8G2_FONT_SECTION("u8g2_font_crox3cb_mr");
extern const uint8_t u8g2_font_crox3cb_mn[] U8G2_FONT_SECTION("u8g2_font_crox3cb_mn");
extern const uint8_t u8g2_font_crox3c_tf[] U8G2_FONT_SECTION("u8g2_font_crox3c_tf");
extern const uint8_t u8g2_font_crox3c_tr[] U8G2_FONT_SECTION("u8g2_font_crox3c_tr");
extern const uint8_t u8g2_font_crox3c_tn[] U8G2_FONT_SECTION("u8g2_font_crox3c_tn");
extern const uint8_t u8g2_font_crox3c_mf[] U8G2_FONT_SECTION("u8g2_font_crox3c_mf");
extern const uint8_t u8g2_font_crox3c_mr[] U8G2_FONT_SECTION("u8g2_font_crox3c_mr");
extern const uint8_t u8g2_font_crox3c_mn[] U8G2_FONT_SECTION("u8g2_font_crox3c_mn");
extern const uint8_t u8g2_font_crox3hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox3hb_tf");
extern const uint8_t u8g2_font_crox3hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox3hb_tr");
extern const uint8_t u8g2_font_crox3hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox3hb_tn");
extern const uint8_t u8g2_font_crox3h_tf[] U8G2_FONT_SECTION("u8g2_font_crox3h_tf");
extern const uint8_t u8g2_font_crox3h_tr[] U8G2_FONT_SECTION("u8g2_font_crox3h_tr");
extern const uint8_t u8g2_font_crox3h_tn[] U8G2_FONT_SECTION("u8g2_font_crox3h_tn");
extern const uint8_t u8g2_font_crox3tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox3tb_tf");
extern const uint8_t u8g2_font_crox3tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox3tb_tr");
extern const uint8_t u8g2_font_crox3tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox3tb_tn");
extern const uint8_t u8g2_font_crox3t_tf[] U8G2_FONT_SECTION("u8g2_font_crox3t_tf");
extern const uint8_t u8g2_font_crox3t_tr[] U8G2_FONT_SECTION("u8g2_font_crox3t_tr");
extern const uint8_t u8g2_font_crox3t_tn[] U8G2_FONT_SECTION("u8g2_font_crox3t_tn");
extern const uint8_t u8g2_font_crox4hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox4hb_tf");
extern const uint8_t u8g2_font_crox4hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox4hb_tr");
extern const uint8_t u8g2_font_crox4hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox4hb_tn");
extern const uint8_t u8g2_font_crox4h_tf[] U8G2_FONT_SECTION("u8g2_font_crox4h_tf");
extern const uint8_t u8g2_font_crox4h_tr[] U8G2_FONT_SECTION("u8g2_font_crox4h_tr");
extern const uint8_t u8g2_font_crox4h_tn[] U8G2_FONT_SECTION("u8g2_font_crox4h_tn");
extern const uint8_t u8g2_font_crox4tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox4tb_tf");
extern const uint8_t u8g2_font_crox4tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox4tb_tr");
extern const uint8_t u8g2_font_crox4tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox4tb_tn");
extern const uint8_t u8g2_font_crox4t_tf[] U8G2_FONT_SECTION("u8g2_font_crox4t_tf");
extern const uint8_t u8g2_font_crox4t_tr[] U8G2_FONT_SECTION("u8g2_font_crox4t_tr");
extern const uint8_t u8g2_font_crox4t_tn[] U8G2_FONT_SECTION("u8g2_font_crox4t_tn");
extern const uint8_t u8g2_font_crox5hb_tf[] U8G2_FONT_SECTION("u8g2_font_crox5hb_tf");
extern const uint8_t u8g2_font_crox5hb_tr[] U8G2_FONT_SECTION("u8g2_font_crox5hb_tr");
extern const uint8_t u8g2_font_crox5hb_tn[] U8G2_FONT_SECTION("u8g2_font_crox5hb_tn");
extern const uint8_t u8g2_font_crox5h_tf[] U8G2_FONT_SECTION("u8g2_font_crox5h_tf");
extern const uint8_t u8g2_font_crox5h_tr[] U8G2_FONT_SECTION("u8g2_font_crox5h_tr");
extern const uint8_t u8g2_font_crox5h_tn[] U8G2_FONT_SECTION("u8g2_font_crox5h_tn");
extern const uint8_t u8g2_font_crox5tb_tf[] U8G2_FONT_SECTION("u8g2_font_crox5tb_tf");
extern const uint8_t u8g2_font_crox5tb_tr[] U8G2_FONT_SECTION("u8g2_font_crox5tb_tr");
extern const uint8_t u8g2_font_crox5tb_tn[] U8G2_FONT_SECTION("u8g2_font_crox5tb_tn");
extern const uint8_t u8g2_font_crox5t_tf[] U8G2_FONT_SECTION("u8g2_font_crox5t_tf");
extern const uint8_t u8g2_font_crox5t_tr[] U8G2_FONT_SECTION("u8g2_font_crox5t_tr");
extern const uint8_t u8g2_font_crox5t_tn[] U8G2_FONT_SECTION("u8g2_font_crox5t_tn");
extern const uint8_t u8g2_font_cu12_tf[] U8G2_FONT_SECTION("u8g2_font_cu12_tf");
extern const uint8_t u8g2_font_cu12_tr[] U8G2_FONT_SECTION("u8g2_font_cu12_tr");
extern const uint8_t u8g2_font_cu12_tn[] U8G2_FONT_SECTION("u8g2_font_cu12_tn");
extern const uint8_t u8g2_font_cu12_te[] U8G2_FONT_SECTION("u8g2_font_cu12_te");
extern const uint8_t u8g2_font_cu12_hf[] U8G2_FONT_SECTION("u8g2_font_cu12_hf");
extern const uint8_t u8g2_font_cu12_hr[] U8G2_FONT_SECTION("u8g2_font_cu12_hr");
extern const uint8_t u8g2_font_cu12_hn[] U8G2_FONT_SECTION("u8g2_font_cu12_hn");
extern const uint8_t u8g2_font_cu12_he[] U8G2_FONT_SECTION("u8g2_font_cu12_he");
extern const uint8_t u8g2_font_cu12_mf[] U8G2_FONT_SECTION("u8g2_font_cu12_mf");
extern const uint8_t u8g2_font_cu12_mr[] U8G2_FONT_SECTION("u8g2_font_cu12_mr");
extern const uint8_t u8g2_font_cu12_mn[] U8G2_FONT_SECTION("u8g2_font_cu12_mn");
extern const uint8_t u8g2_font_cu12_me[] U8G2_FONT_SECTION("u8g2_font_cu12_me");
extern const uint8_t u8g2_font_cu12_t_symbols[] U8G2_FONT_SECTION("u8g2_font_cu12_t_symbols");
extern const uint8_t u8g2_font_cu12_h_symbols[] U8G2_FONT_SECTION("u8g2_font_cu12_h_symbols");
extern const uint8_t u8g2_font_cu12_t_greek[] U8G2_FONT_SECTION("u8g2_font_cu12_t_greek");
extern const uint8_t u8g2_font_cu12_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_cu12_t_cyrillic");
extern const uint8_t u8g2_font_cu12_t_tibetan[] U8G2_FONT_SECTION("u8g2_font_cu12_t_tibetan");
extern const uint8_t u8g2_font_cu12_t_hebrew[] U8G2_FONT_SECTION("u8g2_font_cu12_t_hebrew");
extern const uint8_t u8g2_font_cu12_t_arabic[] U8G2_FONT_SECTION("u8g2_font_cu12_t_arabic");
extern const uint8_t u8g2_font_unifont_tf[] U8G2_FONT_SECTION("u8g2_font_unifont_tf");
extern const uint8_t u8g2_font_unifont_tr[] U8G2_FONT_SECTION("u8g2_font_unifont_tr");
extern const uint8_t u8g2_font_unifont_te[] U8G2_FONT_SECTION("u8g2_font_unifont_te");
extern const uint8_t u8g2_font_unifont_t_latin[] U8G2_FONT_SECTION("u8g2_font_unifont_t_latin");
extern const uint8_t u8g2_font_unifont_t_extended[] U8G2_FONT_SECTION("u8g2_font_unifont_t_extended");
extern const uint8_t u8g2_font_unifont_t_72_73[] U8G2_FONT_SECTION("u8g2_font_unifont_t_72_73");
extern const uint8_t u8g2_font_unifont_t_0_72_73[] U8G2_FONT_SECTION("u8g2_font_unifont_t_0_72_73");
extern const uint8_t u8g2_font_unifont_t_75[] U8G2_FONT_SECTION("u8g2_font_unifont_t_75");
extern const uint8_t u8g2_font_unifont_t_0_75[] U8G2_FONT_SECTION("u8g2_font_unifont_t_0_75");
extern const uint8_t u8g2_font_unifont_t_76[] U8G2_FONT_SECTION("u8g2_font_unifont_t_76");
extern const uint8_t u8g2_font_unifont_t_0_76[] U8G2_FONT_SECTION("u8g2_font_unifont_t_0_76");
extern const uint8_t u8g2_font_unifont_t_77[] U8G2_FONT_SECTION("u8g2_font_unifont_t_77");
extern const uint8_t u8g2_font_unifont_t_0_77[] U8G2_FONT_SECTION("u8g2_font_unifont_t_0_77");
extern const uint8_t u8g2_font_unifont_t_78_79[] U8G2_FONT_SECTION("u8g2_font_unifont_t_78_79");
extern const uint8_t u8g2_font_unifont_t_0_78_79[] U8G2_FONT_SECTION("u8g2_font_unifont_t_0_78_79");
extern const uint8_t u8g2_font_unifont_t_86[] U8G2_FONT_SECTION("u8g2_font_unifont_t_86");
extern const uint8_t u8g2_font_unifont_t_0_86[] U8G2_FONT_SECTION("u8g2_font_unifont_t_0_86");
extern const uint8_t u8g2_font_unifont_t_greek[] U8G2_FONT_SECTION("u8g2_font_unifont_t_greek");
extern const uint8_t u8g2_font_unifont_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_unifont_t_cyrillic");
extern const uint8_t u8g2_font_unifont_t_hebrew[] U8G2_FONT_SECTION("u8g2_font_unifont_t_hebrew");
extern const uint8_t u8g2_font_unifont_t_bengali[] U8G2_FONT_SECTION("u8g2_font_unifont_t_bengali");
extern const uint8_t u8g2_font_unifont_t_tibetan[] U8G2_FONT_SECTION("u8g2_font_unifont_t_tibetan");
extern const uint8_t u8g2_font_unifont_t_urdu[] U8G2_FONT_SECTION("u8g2_font_unifont_t_urdu");
extern const uint8_t u8g2_font_unifont_t_polish[] U8G2_FONT_SECTION("u8g2_font_unifont_t_polish");
extern const uint8_t u8g2_font_unifont_t_devanagari[] U8G2_FONT_SECTION("u8g2_font_unifont_t_devanagari");
extern const uint8_t u8g2_font_unifont_t_arabic[] U8G2_FONT_SECTION("u8g2_font_unifont_t_arabic");
extern const uint8_t u8g2_font_unifont_t_symbols[] U8G2_FONT_SECTION("u8g2_font_unifont_t_symbols");
extern const uint8_t u8g2_font_unifont_h_symbols[] U8G2_FONT_SECTION("u8g2_font_unifont_h_symbols");
extern const uint8_t u8g2_font_unifont_t_emoticons[] U8G2_FONT_SECTION("u8g2_font_unifont_t_emoticons");
extern const uint8_t u8g2_font_unifont_t_animals[] U8G2_FONT_SECTION("u8g2_font_unifont_t_animals");
extern const uint8_t u8g2_font_unifont_t_domino[] U8G2_FONT_SECTION("u8g2_font_unifont_t_domino");
extern const uint8_t u8g2_font_unifont_t_cards[] U8G2_FONT_SECTION("u8g2_font_unifont_t_cards");
extern const uint8_t u8g2_font_unifont_t_weather[] U8G2_FONT_SECTION("u8g2_font_unifont_t_weather");
extern const uint8_t u8g2_font_unifont_t_chinese1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese1");
extern const uint8_t u8g2_font_unifont_t_chinese2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese2");
extern const uint8_t u8g2_font_unifont_t_chinese3[] U8G2_FONT_SECTION("u8g2_font_unifont_t_chinese3");
extern const uint8_t u8g2_font_unifont_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_japanese1");
extern const uint8_t u8g2_font_unifont_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_japanese2");
extern const uint8_t u8g2_font_unifont_t_japanese3[] U8G2_FONT_SECTION("u8g2_font_unifont_t_japanese3");
extern const uint8_t u8g2_font_unifont_t_korean1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_korean1");
extern const uint8_t u8g2_font_unifont_t_korean2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_korean2");
extern const uint8_t u8g2_font_unifont_t_vietnamese1[] U8G2_FONT_SECTION("u8g2_font_unifont_t_vietnamese1");
extern const uint8_t u8g2_font_unifont_t_vietnamese2[] U8G2_FONT_SECTION("u8g2_font_unifont_t_vietnamese2");
extern const uint8_t u8g2_font_gb16st_t_1[] U8G2_FONT_SECTION("u8g2_font_gb16st_t_1");
extern const uint8_t u8g2_font_gb16st_t_2[] U8G2_FONT_SECTION("u8g2_font_gb16st_t_2");
extern const uint8_t u8g2_font_gb16st_t_3[] U8G2_FONT_SECTION("u8g2_font_gb16st_t_3");
extern const uint8_t u8g2_font_gb24st_t_1[] U8G2_FONT_SECTION("u8g2_font_gb24st_t_1");
extern const uint8_t u8g2_font_gb24st_t_2[] U8G2_FONT_SECTION("u8g2_font_gb24st_t_2");
extern const uint8_t u8g2_font_gb24st_t_3[] U8G2_FONT_SECTION("u8g2_font_gb24st_t_3");
extern const uint8_t u8g2_font_wqy12_t_chinese1[] U8G2_FONT_SECTION("u8g2_font_wqy12_t_chinese1");
extern const uint8_t u8g2_font_wqy12_t_chinese2[] U8G2_FONT_SECTION("u8g2_font_wqy12_t_chinese2");
extern const uint8_t u8g2_font_wqy12_t_chinese3[] U8G2_FONT_SECTION("u8g2_font_wqy12_t_chinese3");
extern const uint8_t u8g2_font_wqy12_t_gb2312[] U8G2_FONT_SECTION("u8g2_font_wqy12_t_gb2312");
extern const uint8_t u8g2_font_wqy12_t_gb2312a[] U8G2_FONT_SECTION("u8g2_font_wqy12_t_gb2312a");
extern const uint8_t u8g2_font_wqy12_t_gb2312b[] U8G2_FONT_SECTION("u8g2_font_wqy12_t_gb2312b");
extern const uint8_t u8g2_font_wqy13_t_chinese1[] U8G2_FONT_SECTION("u8g2_font_wqy13_t_chinese1");
extern const uint8_t u8g2_font_wqy13_t_chinese2[] U8G2_FONT_SECTION("u8g2_font_wqy13_t_chinese2");
extern const uint8_t u8g2_font_wqy13_t_chinese3[] U8G2_FONT_SECTION("u8g2_font_wqy13_t_chinese3");
extern const uint8_t u8g2_font_wqy13_t_gb2312[] U8G2_FONT_SECTION("u8g2_font_wqy13_t_gb2312");
extern const uint8_t u8g2_font_wqy13_t_gb2312a[] U8G2_FONT_SECTION("u8g2_font_wqy13_t_gb2312a");
extern const uint8_t u8g2_font_wqy13_t_gb2312b[] U8G2_FONT_SECTION("u8g2_font_wqy13_t_gb2312b");
extern const uint8_t u8g2_font_wqy14_t_chinese1[] U8G2_FONT_SECTION("u8g2_font_wqy14_t_chinese1");
extern const uint8_t u8g2_font_wqy14_t_chinese2[] U8G2_FONT_SECTION("u8g2_font_wqy14_t_chinese2");
extern const uint8_t u8g2_font_wqy14_t_chinese3[] U8G2_FONT_SECTION("u8g2_font_wqy14_t_chinese3");
extern const uint8_t u8g2_font_wqy14_t_gb2312[] U8G2_FONT_SECTION("u8g2_font_wqy14_t_gb2312");
extern const uint8_t u8g2_font_wqy14_t_gb2312a[] U8G2_FONT_SECTION("u8g2_font_wqy14_t_gb2312a");
extern const uint8_t u8g2_font_wqy14_t_gb2312b[] U8G2_FONT_SECTION("u8g2_font_wqy14_t_gb2312b");
extern const uint8_t u8g2_font_wqy15_t_chinese1[] U8G2_FONT_SECTION("u8g2_font_wqy15_t_chinese1");
extern const uint8_t u8g2_font_wqy15_t_chinese2[] U8G2_FONT_SECTION("u8g2_font_wqy15_t_chinese2");
extern const uint8_t u8g2_font_wqy15_t_chinese3[] U8G2_FONT_SECTION("u8g2_font_wqy15_t_chinese3");
extern const uint8_t u8g2_font_wqy15_t_gb2312[] U8G2_FONT_SECTION("u8g2_font_wqy15_t_gb2312");
extern const uint8_t u8g2_font_wqy15_t_gb2312a[] U8G2_FONT_SECTION("u8g2_font_wqy15_t_gb2312a");
extern const uint8_t u8g2_font_wqy15_t_gb2312b[] U8G2_FONT_SECTION("u8g2_font_wqy15_t_gb2312b");
extern const uint8_t u8g2_font_wqy16_t_chinese1[] U8G2_FONT_SECTION("u8g2_font_wqy16_t_chinese1");
extern const uint8_t u8g2_font_wqy16_t_chinese2[] U8G2_FONT_SECTION("u8g2_font_wqy16_t_chinese2");
extern const uint8_t u8g2_font_wqy16_t_chinese3[] U8G2_FONT_SECTION("u8g2_font_wqy16_t_chinese3");
extern const uint8_t u8g2_font_wqy16_t_gb2312[] U8G2_FONT_SECTION("u8g2_font_wqy16_t_gb2312");
extern const uint8_t u8g2_font_wqy16_t_gb2312a[] U8G2_FONT_SECTION("u8g2_font_wqy16_t_gb2312a");
extern const uint8_t u8g2_font_wqy16_t_gb2312b[] U8G2_FONT_SECTION("u8g2_font_wqy16_t_gb2312b");
extern const uint8_t u8g2_font_b10_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b10_t_japanese1");
extern const uint8_t u8g2_font_b10_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b10_t_japanese2");
extern const uint8_t u8g2_font_b10_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b10_b_t_japanese1");
extern const uint8_t u8g2_font_b10_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b10_b_t_japanese2");
extern const uint8_t u8g2_font_f10_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f10_t_japanese1");
extern const uint8_t u8g2_font_f10_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f10_t_japanese2");
extern const uint8_t u8g2_font_f10_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f10_b_t_japanese1");
extern const uint8_t u8g2_font_f10_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f10_b_t_japanese2");
extern const uint8_t u8g2_font_b12_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b12_t_japanese1");
extern const uint8_t u8g2_font_b12_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b12_t_japanese2");
extern const uint8_t u8g2_font_b12_t_japanese3[] U8G2_FONT_SECTION("u8g2_font_b12_t_japanese3");
extern const uint8_t u8g2_font_b12_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b12_b_t_japanese1");
extern const uint8_t u8g2_font_b12_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b12_b_t_japanese2");
extern const uint8_t u8g2_font_b12_b_t_japanese3[] U8G2_FONT_SECTION("u8g2_font_b12_b_t_japanese3");
extern const uint8_t u8g2_font_f12_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f12_t_japanese1");
extern const uint8_t u8g2_font_f12_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f12_t_japanese2");
extern const uint8_t u8g2_font_f12_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f12_b_t_japanese1");
extern const uint8_t u8g2_font_f12_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f12_b_t_japanese2");
extern const uint8_t u8g2_font_b16_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b16_t_japanese1");
extern const uint8_t u8g2_font_b16_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b16_t_japanese2");
extern const uint8_t u8g2_font_b16_t_japanese3[] U8G2_FONT_SECTION("u8g2_font_b16_t_japanese3");
extern const uint8_t u8g2_font_b16_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_b16_b_t_japanese1");
extern const uint8_t u8g2_font_b16_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_b16_b_t_japanese2");
extern const uint8_t u8g2_font_b16_b_t_japanese3[] U8G2_FONT_SECTION("u8g2_font_b16_b_t_japanese3");
extern const uint8_t u8g2_font_f16_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f16_t_japanese1");
extern const uint8_t u8g2_font_f16_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f16_t_japanese2");
extern const uint8_t u8g2_font_f16_b_t_japanese1[] U8G2_FONT_SECTION("u8g2_font_f16_b_t_japanese1");
extern const uint8_t u8g2_font_f16_b_t_japanese2[] U8G2_FONT_SECTION("u8g2_font_f16_b_t_japanese2");
extern const uint8_t u8g2_font_artossans8_8r[] U8G2_FONT_SECTION("u8g2_font_artossans8_8r");
extern const uint8_t u8g2_font_artossans8_8n[] U8G2_FONT_SECTION("u8g2_font_artossans8_8n");
extern const uint8_t u8g2_font_artossans8_8u[] U8G2_FONT_SECTION("u8g2_font_artossans8_8u");
extern const uint8_t u8g2_font_artosserif8_8r[] U8G2_FONT_SECTION("u8g2_font_artosserif8_8r");
extern const uint8_t u8g2_font_artosserif8_8n[] U8G2_FONT_SECTION("u8g2_font_artosserif8_8n");
extern const uint8_t u8g2_font_artosserif8_8u[] U8G2_FONT_SECTION("u8g2_font_artosserif8_8u");
extern const uint8_t u8g2_font_chroma48medium8_8r[] U8G2_FONT_SECTION("u8g2_font_chroma48medium8_8r");
extern const uint8_t u8g2_font_chroma48medium8_8n[] U8G2_FONT_SECTION("u8g2_font_chroma48medium8_8n");
extern const uint8_t u8g2_font_chroma48medium8_8u[] U8G2_FONT_SECTION("u8g2_font_chroma48medium8_8u");
extern const uint8_t u8g2_font_saikyosansbold8_8n[] U8G2_FONT_SECTION("u8g2_font_saikyosansbold8_8n");
extern const uint8_t u8g2_font_saikyosansbold8_8u[] U8G2_FONT_SECTION("u8g2_font_saikyosansbold8_8u");
extern const uint8_t u8g2_font_torussansbold8_8r[] U8G2_FONT_SECTION("u8g2_font_torussansbold8_8r");
extern const uint8_t u8g2_font_torussansbold8_8n[] U8G2_FONT_SECTION("u8g2_font_torussansbold8_8n");
extern const uint8_t u8g2_font_torussansbold8_8u[] U8G2_FONT_SECTION("u8g2_font_torussansbold8_8u");
extern const uint8_t u8g2_font_victoriabold8_8r[] U8G2_FONT_SECTION("u8g2_font_victoriabold8_8r");
extern const uint8_t u8g2_font_victoriabold8_8n[] U8G2_FONT_SECTION("u8g2_font_victoriabold8_8n");
extern const uint8_t u8g2_font_victoriabold8_8u[] U8G2_FONT_SECTION("u8g2_font_victoriabold8_8u");
extern const uint8_t u8g2_font_victoriamedium8_8r[] U8G2_FONT_SECTION("u8g2_font_victoriamedium8_8r");
extern const uint8_t u8g2_font_victoriamedium8_8n[] U8G2_FONT_SECTION("u8g2_font_victoriamedium8_8n");
extern const uint8_t u8g2_font_victoriamedium8_8u[] U8G2_FONT_SECTION("u8g2_font_victoriamedium8_8u");
extern const uint8_t u8g2_font_courB08_tf[] U8G2_FONT_SECTION("u8g2_font_courB08_tf");
extern const uint8_t u8g2_font_courB08_tr[] U8G2_FONT_SECTION("u8g2_font_courB08_tr");
extern const uint8_t u8g2_font_courB08_tn[] U8G2_FONT_SECTION("u8g2_font_courB08_tn");
extern const uint8_t u8g2_font_courB10_tf[] U8G2_FONT_SECTION("u8g2_font_courB10_tf");
extern const uint8_t u8g2_font_courB10_tr[] U8G2_FONT_SECTION("u8g2_font_courB10_tr");
extern const uint8_t u8g2_font_courB10_tn[] U8G2_FONT_SECTION("u8g2_font_courB10_tn");
extern const uint8_t u8g2_font_courB12_tf[] U8G2_FONT_SECTION("u8g2_font_courB12_tf");
extern const uint8_t u8g2_font_courB12_tr[] U8G2_FONT_SECTION("u8g2_font_courB12_tr");
extern const uint8_t u8g2_font_courB12_tn[] U8G2_FONT_SECTION("u8g2_font_courB12_tn");
extern const uint8_t u8g2_font_courB14_tf[] U8G2_FONT_SECTION("u8g2_font_courB14_tf");
extern const uint8_t u8g2_font_courB14_tr[] U8G2_FONT_SECTION("u8g2_font_courB14_tr");
extern const uint8_t u8g2_font_courB14_tn[] U8G2_FONT_SECTION("u8g2_font_courB14_tn");
extern const uint8_t u8g2_font_courB18_tf[] U8G2_FONT_SECTION("u8g2_font_courB18_tf");
extern const uint8_t u8g2_font_courB18_tr[] U8G2_FONT_SECTION("u8g2_font_courB18_tr");
extern const uint8_t u8g2_font_courB18_tn[] U8G2_FONT_SECTION("u8g2_font_courB18_tn");
extern const uint8_t u8g2_font_courB24_tf[] U8G2_FONT_SECTION("u8g2_font_courB24_tf");
extern const uint8_t u8g2_font_courB24_tr[] U8G2_FONT_SECTION("u8g2_font_courB24_tr");
extern const uint8_t u8g2_font_courB24_tn[] U8G2_FONT_SECTION("u8g2_font_courB24_tn");
extern const uint8_t u8g2_font_courR08_tf[] U8G2_FONT_SECTION("u8g2_font_courR08_tf");
extern const uint8_t u8g2_font_courR08_tr[] U8G2_FONT_SECTION("u8g2_font_courR08_tr");
extern const uint8_t u8g2_font_courR08_tn[] U8G2_FONT_SECTION("u8g2_font_courR08_tn");
extern const uint8_t u8g2_font_courR10_tf[] U8G2_FONT_SECTION("u8g2_font_courR10_tf");
extern const uint8_t u8g2_font_courR10_tr[] U8G2_FONT_SECTION("u8g2_font_courR10_tr");
extern const uint8_t u8g2_font_courR10_tn[] U8G2_FONT_SECTION("u8g2_font_courR10_tn");
extern const uint8_t u8g2_font_courR12_tf[] U8G2_FONT_SECTION("u8g2_font_courR12_tf");
extern const uint8_t u8g2_font_courR12_tr[] U8G2_FONT_SECTION("u8g2_font_courR12_tr");
extern const uint8_t u8g2_font_courR12_tn[] U8G2_FONT_SECTION("u8g2_font_courR12_tn");
extern const uint8_t u8g2_font_courR14_tf[] U8G2_FONT_SECTION("u8g2_font_courR14_tf");
extern const uint8_t u8g2_font_courR14_tr[] U8G2_FONT_SECTION("u8g2_font_courR14_tr");
extern const uint8_t u8g2_font_courR14_tn[] U8G2_FONT_SECTION("u8g2_font_courR14_tn");
extern const uint8_t u8g2_font_courR18_tf[] U8G2_FONT_SECTION("u8g2_font_courR18_tf");
extern const uint8_t u8g2_font_courR18_tr[] U8G2_FONT_SECTION("u8g2_font_courR18_tr");
extern const uint8_t u8g2_font_courR18_tn[] U8G2_FONT_SECTION("u8g2_font_courR18_tn");
extern const uint8_t u8g2_font_courR24_tf[] U8G2_FONT_SECTION("u8g2_font_courR24_tf");
extern const uint8_t u8g2_font_courR24_tr[] U8G2_FONT_SECTION("u8g2_font_courR24_tr");
extern const uint8_t u8g2_font_courR24_tn[] U8G2_FONT_SECTION("u8g2_font_courR24_tn");
extern const uint8_t u8g2_font_helvB08_tf[] U8G2_FONT_SECTION("u8g2_font_helvB08_tf");
extern const uint8_t u8g2_font_helvB08_tr[] U8G2_FONT_SECTION("u8g2_font_helvB08_tr");
extern const uint8_t u8g2_font_helvB08_tn[] U8G2_FONT_SECTION("u8g2_font_helvB08_tn");
extern const uint8_t u8g2_font_helvB08_te[] U8G2_FONT_SECTION("u8g2_font_helvB08_te");
extern const uint8_t u8g2_font_helvB10_tf[] U8G2_FONT_SECTION("u8g2_font_helvB10_tf");
extern const uint8_t u8g2_font_helvB10_tr[] U8G2_FONT_SECTION("u8g2_font_helvB10_tr");
extern const uint8_t u8g2_font_helvB10_tn[] U8G2_FONT_SECTION("u8g2_font_helvB10_tn");
extern const uint8_t u8g2_font_helvB10_te[] U8G2_FONT_SECTION("u8g2_font_helvB10_te");
extern const uint8_t u8g2_font_helvB12_tf[] U8G2_FONT_SECTION("u8g2_font_helvB12_tf");
extern const uint8_t u8g2_font_helvB12_tr[] U8G2_FONT_SECTION("u8g2_font_helvB12_tr");
extern const uint8_t u8g2_font_helvB12_tn[] U8G2_FONT_SECTION("u8g2_font_helvB12_tn");
extern const uint8_t u8g2_font_helvB12_te[] U8G2_FONT_SECTION("u8g2_font_helvB12_te");
extern const uint8_t u8g2_font_helvB14_tf[] U8G2_FONT_SECTION("u8g2_font_helvB14_tf");
extern const uint8_t u8g2_font_helvB14_tr[] U8G2_FONT_SECTION("u8g2_font_helvB14_tr");
extern const uint8_t u8g2_font_helvB14_tn[] U8G2_FONT_SECTION("u8g2_font_helvB14_tn");
extern const uint8_t u8g2_font_helvB14_te[] U8G2_FONT_SECTION("u8g2_font_helvB14_te");
extern const uint8_t u8g2_font_helvB18_tf[] U8G2_FONT_SECTION("u8g2_font_helvB18_tf");
extern const uint8_t u8g2_font_helvB18_tr[] U8G2_FONT_SECTION("u8g2_font_helvB18_tr");
extern const uint8_t u8g2_font_helvB18_tn[] U8G2_FONT_SECTION("u8g2_font_helvB18_tn");
extern const uint8_t u8g2_font_helvB18_te[] U8G2_FONT_SECTION("u8g2_font_helvB18_te");
extern const uint8_t u8g2_font_helvB24_tf[] U8G2_FONT_SECTION("u8g2_font_helvB24_tf");
extern const uint8_t u8g2_font_helvB24_tr[] U8G2_FONT_SECTION("u8g2_font_helvB24_tr");
extern const uint8_t u8g2_font_helvB24_tn[] U8G2_FONT_SECTION("u8g2_font_helvB24_tn");
extern const uint8_t u8g2_font_helvB24_te[] U8G2_FONT_SECTION("u8g2_font_helvB24_te");
extern const uint8_t u8g2_font_helvR08_tf[] U8G2_FONT_SECTION("u8g2_font_helvR08_tf");
extern const uint8_t u8g2_font_helvR08_tr[] U8G2_FONT_SECTION("u8g2_font_helvR08_tr");
extern const uint8_t u8g2_font_helvR08_tn[] U8G2_FONT_SECTION("u8g2_font_helvR08_tn");
extern const uint8_t u8g2_font_helvR08_te[] U8G2_FONT_SECTION("u8g2_font_helvR08_te");
extern const uint8_t u8g2_font_helvR10_tf[] U8G2_FONT_SECTION("u8g2_font_helvR10_tf");
extern const uint8_t u8g2_font_helvR10_tr[] U8G2_FONT_SECTION("u8g2_font_helvR10_tr");
extern const uint8_t u8g2_font_helvR10_tn[] U8G2_FONT_SECTION("u8g2_font_helvR10_tn");
extern const uint8_t u8g2_font_helvR10_te[] U8G2_FONT_SECTION("u8g2_font_helvR10_te");
extern const uint8_t u8g2_font_helvR12_tf[] U8G2_FONT_SECTION("u8g2_font_helvR12_tf");
extern const uint8_t u8g2_font_helvR12_tr[] U8G2_FONT_SECTION("u8g2_font_helvR12_tr");
extern const uint8_t u8g2_font_helvR12_tn[] U8G2_FONT_SECTION("u8g2_font_helvR12_tn");
extern const uint8_t u8g2_font_helvR12_te[] U8G2_FONT_SECTION("u8g2_font_helvR12_te");
extern const uint8_t u8g2_font_helvR14_tf[] U8G2_FONT_SECTION("u8g2_font_helvR14_tf");
extern const uint8_t u8g2_font_helvR14_tr[] U8G2_FONT_SECTION("u8g2_font_helvR14_tr");
extern const uint8_t u8g2_font_helvR14_tn[] U8G2_FONT_SECTION("u8g2_font_helvR14_tn");
extern const uint8_t u8g2_font_helvR14_te[] U8G2_FONT_SECTION("u8g2_font_helvR14_te");
extern const uint8_t u8g2_font_helvR18_tf[] U8G2_FONT_SECTION("u8g2_font_helvR18_tf");
extern const uint8_t u8g2_font_helvR18_tr[] U8G2_FONT_SECTION("u8g2_font_helvR18_tr");
extern const uint8_t u8g2_font_helvR18_tn[] U8G2_FONT_SECTION("u8g2_font_helvR18_tn");
extern const uint8_t u8g2_font_helvR18_te[] U8G2_FONT_SECTION("u8g2_font_helvR18_te");
extern const uint8_t u8g2_font_helvR24_tf[] U8G2_FONT_SECTION("u8g2_font_helvR24_tf");
extern const uint8_t u8g2_font_helvR24_tr[] U8G2_FONT_SECTION("u8g2_font_helvR24_tr");
extern const uint8_t u8g2_font_helvR24_tn[] U8G2_FONT_SECTION("u8g2_font_helvR24_tn");
extern const uint8_t u8g2_font_helvR24_te[] U8G2_FONT_SECTION("u8g2_font_helvR24_te");
extern const uint8_t u8g2_font_ncenB08_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB08_tf");
extern const uint8_t u8g2_font_ncenB08_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB08_tr");
extern const uint8_t u8g2_font_ncenB08_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB08_tn");
extern const uint8_t u8g2_font_ncenB08_te[] U8G2_FONT_SECTION("u8g2_font_ncenB08_te");
extern const uint8_t u8g2_font_ncenB10_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB10_tf");
extern const uint8_t u8g2_font_ncenB10_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB10_tr");
extern const uint8_t u8g2_font_ncenB10_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB10_tn");
extern const uint8_t u8g2_font_ncenB10_te[] U8G2_FONT_SECTION("u8g2_font_ncenB10_te");
extern const uint8_t u8g2_font_ncenB12_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB12_tf");
extern const uint8_t u8g2_font_ncenB12_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB12_tr");
extern const uint8_t u8g2_font_ncenB12_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB12_tn");
extern const uint8_t u8g2_font_ncenB12_te[] U8G2_FONT_SECTION("u8g2_font_ncenB12_te");
extern const uint8_t u8g2_font_ncenB14_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB14_tf");
extern const uint8_t u8g2_font_ncenB14_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB14_tr");
extern const uint8_t u8g2_font_ncenB14_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB14_tn");
extern const uint8_t u8g2_font_ncenB14_te[] U8G2_FONT_SECTION("u8g2_font_ncenB14_te");
extern const uint8_t u8g2_font_ncenB18_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB18_tf");
extern const uint8_t u8g2_font_ncenB18_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB18_tr");
extern const uint8_t u8g2_font_ncenB18_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB18_tn");
extern const uint8_t u8g2_font_ncenB18_te[] U8G2_FONT_SECTION("u8g2_font_ncenB18_te");
extern const uint8_t u8g2_font_ncenB24_tf[] U8G2_FONT_SECTION("u8g2_font_ncenB24_tf");
extern const uint8_t u8g2_font_ncenB24_tr[] U8G2_FONT_SECTION("u8g2_font_ncenB24_tr");
extern const uint8_t u8g2_font_ncenB24_tn[] U8G2_FONT_SECTION("u8g2_font_ncenB24_tn");
extern const uint8_t u8g2_font_ncenB24_te[] U8G2_FONT_SECTION("u8g2_font_ncenB24_te");
extern const uint8_t u8g2_font_ncenR08_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR08_tf");
extern const uint8_t u8g2_font_ncenR08_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR08_tr");
extern const uint8_t u8g2_font_ncenR08_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR08_tn");
extern const uint8_t u8g2_font_ncenR08_te[] U8G2_FONT_SECTION("u8g2_font_ncenR08_te");
extern const uint8_t u8g2_font_ncenR10_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR10_tf");
extern const uint8_t u8g2_font_ncenR10_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR10_tr");
extern const uint8_t u8g2_font_ncenR10_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR10_tn");
extern const uint8_t u8g2_font_ncenR10_te[] U8G2_FONT_SECTION("u8g2_font_ncenR10_te");
extern const uint8_t u8g2_font_ncenR12_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR12_tf");
extern const uint8_t u8g2_font_ncenR12_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR12_tr");
extern const uint8_t u8g2_font_ncenR12_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR12_tn");
extern const uint8_t u8g2_font_ncenR12_te[] U8G2_FONT_SECTION("u8g2_font_ncenR12_te");
extern const uint8_t u8g2_font_ncenR14_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR14_tf");
extern const uint8_t u8g2_font_ncenR14_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR14_tr");
extern const uint8_t u8g2_font_ncenR14_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR14_tn");
extern const uint8_t u8g2_font_ncenR14_te[] U8G2_FONT_SECTION("u8g2_font_ncenR14_te");
extern const uint8_t u8g2_font_ncenR18_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR18_tf");
extern const uint8_t u8g2_font_ncenR18_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR18_tr");
extern const uint8_t u8g2_font_ncenR18_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR18_tn");
extern const uint8_t u8g2_font_ncenR18_te[] U8G2_FONT_SECTION("u8g2_font_ncenR18_te");
extern const uint8_t u8g2_font_ncenR24_tf[] U8G2_FONT_SECTION("u8g2_font_ncenR24_tf");
extern const uint8_t u8g2_font_ncenR24_tr[] U8G2_FONT_SECTION("u8g2_font_ncenR24_tr");
extern const uint8_t u8g2_font_ncenR24_tn[] U8G2_FONT_SECTION("u8g2_font_ncenR24_tn");
extern const uint8_t u8g2_font_ncenR24_te[] U8G2_FONT_SECTION("u8g2_font_ncenR24_te");
extern const uint8_t u8g2_font_timB08_tf[] U8G2_FONT_SECTION("u8g2_font_timB08_tf");
extern const uint8_t u8g2_font_timB08_tr[] U8G2_FONT_SECTION("u8g2_font_timB08_tr");
extern const uint8_t u8g2_font_timB08_tn[] U8G2_FONT_SECTION("u8g2_font_timB08_tn");
extern const uint8_t u8g2_font_timB10_tf[] U8G2_FONT_SECTION("u8g2_font_timB10_tf");
extern const uint8_t u8g2_font_timB10_tr[] U8G2_FONT_SECTION("u8g2_font_timB10_tr");
extern const uint8_t u8g2_font_timB10_tn[] U8G2_FONT_SECTION("u8g2_font_timB10_tn");
extern const uint8_t u8g2_font_timB12_tf[] U8G2_FONT_SECTION("u8g2_font_timB12_tf");
extern const uint8_t u8g2_font_timB12_tr[] U8G2_FONT_SECTION("u8g2_font_timB12_tr");
extern const uint8_t u8g2_font_timB12_tn[] U8G2_FONT_SECTION("u8g2_font_timB12_tn");
extern const uint8_t u8g2_font_timB14_tf[] U8G2_FONT_SECTION("u8g2_font_timB14_tf");
extern const uint8_t u8g2_font_timB14_tr[] U8G2_FONT_SECTION("u8g2_font_timB14_tr");
extern const uint8_t u8g2_font_timB14_tn[] U8G2_FONT_SECTION("u8g2_font_timB14_tn");
extern const uint8_t u8g2_font_timB18_tf[] U8G2_FONT_SECTION("u8g2_font_timB18_tf");
extern const uint8_t u8g2_font_timB18_tr[] U8G2_FONT_SECTION("u8g2_font_timB18_tr");
extern const uint8_t u8g2_font_timB18_tn[] U8G2_FONT_SECTION("u8g2_font_timB18_tn");
extern const uint8_t u8g2_font_timB24_tf[] U8G2_FONT_SECTION("u8g2_font_timB24_tf");
extern const uint8_t u8g2_font_timB24_tr[] U8G2_FONT_SECTION("u8g2_font_timB24_tr");
extern const uint8_t u8g2_font_timB24_tn[] U8G2_FONT_SECTION("u8g2_font_timB24_tn");
extern const uint8_t u8g2_font_timR08_tf[] U8G2_FONT_SECTION("u8g2_font_timR08_tf");
extern const uint8_t u8g2_font_timR08_tr[] U8G2_FONT_SECTION("u8g2_font_timR08_tr");
extern const uint8_t u8g2_font_timR08_tn[] U8G2_FONT_SECTION("u8g2_font_timR08_tn");
extern const uint8_t u8g2_font_timR10_tf[] U8G2_FONT_SECTION("u8g2_font_timR10_tf");
extern const uint8_t u8g2_font_timR10_tr[] U8G2_FONT_SECTION("u8g2_font_timR10_tr");
extern const uint8_t u8g2_font_timR10_tn[] U8G2_FONT_SECTION("u8g2_font_timR10_tn");
extern const uint8_t u8g2_font_timR12_tf[] U8G2_FONT_SECTION("u8g2_font_timR12_tf");
extern const uint8_t u8g2_font_timR12_tr[] U8G2_FONT_SECTION("u8g2_font_timR12_tr");
extern const uint8_t u8g2_font_timR12_tn[] U8G2_FONT_SECTION("u8g2_font_timR12_tn");
extern const uint8_t u8g2_font_timR14_tf[] U8G2_FONT_SECTION("u8g2_font_timR14_tf");
extern const uint8_t u8g2_font_timR14_tr[] U8G2_FONT_SECTION("u8g2_font_timR14_tr");
extern const uint8_t u8g2_font_timR14_tn[] U8G2_FONT_SECTION("u8g2_font_timR14_tn");
extern const uint8_t u8g2_font_timR18_tf[] U8G2_FONT_SECTION("u8g2_font_timR18_tf");
extern const uint8_t u8g2_font_timR18_tr[] U8G2_FONT_SECTION("u8g2_font_timR18_tr");
extern const uint8_t u8g2_font_timR18_tn[] U8G2_FONT_SECTION("u8g2_font_timR18_tn");
extern const uint8_t u8g2_font_timR24_tf[] U8G2_FONT_SECTION("u8g2_font_timR24_tf");
extern const uint8_t u8g2_font_timR24_tr[] U8G2_FONT_SECTION("u8g2_font_timR24_tr");
extern const uint8_t u8g2_font_timR24_tn[] U8G2_FONT_SECTION("u8g2_font_timR24_tn");
extern const uint8_t u8g2_font_baby_tf[] U8G2_FONT_SECTION("u8g2_font_baby_tf");
extern const uint8_t u8g2_font_baby_tr[] U8G2_FONT_SECTION("u8g2_font_baby_tr");
extern const uint8_t u8g2_font_baby_tn[] U8G2_FONT_SECTION("u8g2_font_baby_tn");
extern const uint8_t u8g2_font_blipfest_07_tr[] U8G2_FONT_SECTION("u8g2_font_blipfest_07_tr");
extern const uint8_t u8g2_font_blipfest_07_tn[] U8G2_FONT_SECTION("u8g2_font_blipfest_07_tn");
extern const uint8_t u8g2_font_chikita_tf[] U8G2_FONT_SECTION("u8g2_font_chikita_tf");
extern const uint8_t u8g2_font_chikita_tr[] U8G2_FONT_SECTION("u8g2_font_chikita_tr");
extern const uint8_t u8g2_font_chikita_tn[] U8G2_FONT_SECTION("u8g2_font_chikita_tn");
extern const uint8_t u8g2_font_lucasfont_alternate_tf[] U8G2_FONT_SECTION("u8g2_font_lucasfont_alternate_tf");
extern const uint8_t u8g2_font_lucasfont_alternate_tr[] U8G2_FONT_SECTION("u8g2_font_lucasfont_alternate_tr");
extern const uint8_t u8g2_font_lucasfont_alternate_tn[] U8G2_FONT_SECTION("u8g2_font_lucasfont_alternate_tn");
extern const uint8_t u8g2_font_p01type_tf[] U8G2_FONT_SECTION("u8g2_font_p01type_tf");
extern const uint8_t u8g2_font_p01type_tr[] U8G2_FONT_SECTION("u8g2_font_p01type_tr");
extern const uint8_t u8g2_font_p01type_tn[] U8G2_FONT_SECTION("u8g2_font_p01type_tn");
extern const uint8_t u8g2_font_pixelle_micro_tr[] U8G2_FONT_SECTION("u8g2_font_pixelle_micro_tr");
extern const uint8_t u8g2_font_pixelle_micro_tn[] U8G2_FONT_SECTION("u8g2_font_pixelle_micro_tn");
extern const uint8_t u8g2_font_robot_de_niro_tf[] U8G2_FONT_SECTION("u8g2_font_robot_de_niro_tf");
extern const uint8_t u8g2_font_robot_de_niro_tr[] U8G2_FONT_SECTION("u8g2_font_robot_de_niro_tr");
extern const uint8_t u8g2_font_robot_de_niro_tn[] U8G2_FONT_SECTION("u8g2_font_robot_de_niro_tn");
extern const uint8_t u8g2_font_trixel_square_tf[] U8G2_FONT_SECTION("u8g2_font_trixel_square_tf");
extern const uint8_t u8g2_font_trixel_square_tr[] U8G2_FONT_SECTION("u8g2_font_trixel_square_tr");
extern const uint8_t u8g2_font_trixel_square_tn[] U8G2_FONT_SECTION("u8g2_font_trixel_square_tn");
extern const uint8_t u8g2_font_haxrcorp4089_tr[] U8G2_FONT_SECTION("u8g2_font_haxrcorp4089_tr");
extern const uint8_t u8g2_font_haxrcorp4089_tn[] U8G2_FONT_SECTION("u8g2_font_haxrcorp4089_tn");
extern const uint8_t u8g2_font_haxrcorp4089_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_haxrcorp4089_t_cyrillic");
extern const uint8_t u8g2_font_bubble_tr[] U8G2_FONT_SECTION("u8g2_font_bubble_tr");
extern const uint8_t u8g2_font_bubble_tn[] U8G2_FONT_SECTION("u8g2_font_bubble_tn");
extern const uint8_t u8g2_font_cardimon_pixel_tf[] U8G2_FONT_SECTION("u8g2_font_cardimon_pixel_tf");
extern const uint8_t u8g2_font_cardimon_pixel_tr[] U8G2_FONT_SECTION("u8g2_font_cardimon_pixel_tr");
extern const uint8_t u8g2_font_cardimon_pixel_tn[] U8G2_FONT_SECTION("u8g2_font_cardimon_pixel_tn");
extern const uint8_t u8g2_font_maniac_tf[] U8G2_FONT_SECTION("u8g2_font_maniac_tf");
extern const uint8_t u8g2_font_maniac_tr[] U8G2_FONT_SECTION("u8g2_font_maniac_tr");
extern const uint8_t u8g2_font_maniac_tn[] U8G2_FONT_SECTION("u8g2_font_maniac_tn");
extern const uint8_t u8g2_font_maniac_te[] U8G2_FONT_SECTION("u8g2_font_maniac_te");
extern const uint8_t u8g2_font_lucasarts_scumm_subtitle_o_tf[] U8G2_FONT_SECTION("u8g2_font_lucasarts_scumm_subtitle_o_tf");
extern const uint8_t u8g2_font_lucasarts_scumm_subtitle_o_tr[] U8G2_FONT_SECTION("u8g2_font_lucasarts_scumm_subtitle_o_tr");
extern const uint8_t u8g2_font_lucasarts_scumm_subtitle_o_tn[] U8G2_FONT_SECTION("u8g2_font_lucasarts_scumm_subtitle_o_tn");
extern const uint8_t u8g2_font_lucasarts_scumm_subtitle_r_tf[] U8G2_FONT_SECTION("u8g2_font_lucasarts_scumm_subtitle_r_tf");
extern const uint8_t u8g2_font_lucasarts_scumm_subtitle_r_tr[] U8G2_FONT_SECTION("u8g2_font_lucasarts_scumm_subtitle_r_tr");
extern const uint8_t u8g2_font_lucasarts_scumm_subtitle_r_tn[] U8G2_FONT_SECTION("u8g2_font_lucasarts_scumm_subtitle_r_tn");
extern const uint8_t u8g2_font_fub11_tf[] U8G2_FONT_SECTION("u8g2_font_fub11_tf");
extern const uint8_t u8g2_font_fub11_tr[] U8G2_FONT_SECTION("u8g2_font_fub11_tr");
extern const uint8_t u8g2_font_fub11_tn[] U8G2_FONT_SECTION("u8g2_font_fub11_tn");
extern const uint8_t u8g2_font_fub14_tf[] U8G2_FONT_SECTION("u8g2_font_fub14_tf");
extern const uint8_t u8g2_font_fub14_tr[] U8G2_FONT_SECTION("u8g2_font_fub14_tr");
extern const uint8_t u8g2_font_fub14_tn[] U8G2_FONT_SECTION("u8g2_font_fub14_tn");
extern const uint8_t u8g2_font_fub17_tf[] U8G2_FONT_SECTION("u8g2_font_fub17_tf");
extern const uint8_t u8g2_font_fub17_tr[] U8G2_FONT_SECTION("u8g2_font_fub17_tr");
extern const uint8_t u8g2_font_fub17_tn[] U8G2_FONT_SECTION("u8g2_font_fub17_tn");
extern const uint8_t u8g2_font_fub20_tf[] U8G2_FONT_SECTION("u8g2_font_fub20_tf");
extern const uint8_t u8g2_font_fub20_tr[] U8G2_FONT_SECTION("u8g2_font_fub20_tr");
extern const uint8_t u8g2_font_fub20_tn[] U8G2_FONT_SECTION("u8g2_font_fub20_tn");
extern const uint8_t u8g2_font_fub25_tf[] U8G2_FONT_SECTION("u8g2_font_fub25_tf");
extern const uint8_t u8g2_font_fub25_tr[] U8G2_FONT_SECTION("u8g2_font_fub25_tr");
extern const uint8_t u8g2_font_fub25_tn[] U8G2_FONT_SECTION("u8g2_font_fub25_tn");
extern const uint8_t u8g2_font_fub30_tf[] U8G2_FONT_SECTION("u8g2_font_fub30_tf");
extern const uint8_t u8g2_font_fub30_tr[] U8G2_FONT_SECTION("u8g2_font_fub30_tr");
extern const uint8_t u8g2_font_fub30_tn[] U8G2_FONT_SECTION("u8g2_font_fub30_tn");
extern const uint8_t u8g2_font_fub35_tf[] U8G2_FONT_SECTION("u8g2_font_fub35_tf");
extern const uint8_t u8g2_font_fub35_tr[] U8G2_FONT_SECTION("u8g2_font_fub35_tr");
extern const uint8_t u8g2_font_fub35_tn[] U8G2_FONT_SECTION("u8g2_font_fub35_tn");
extern const uint8_t u8g2_font_fub42_tf[] U8G2_FONT_SECTION("u8g2_font_fub42_tf");
extern const uint8_t u8g2_font_fub42_tr[] U8G2_FONT_SECTION("u8g2_font_fub42_tr");
extern const uint8_t u8g2_font_fub42_tn[] U8G2_FONT_SECTION("u8g2_font_fub42_tn");
extern const uint8_t u8g2_font_fub49_tn[] U8G2_FONT_SECTION("u8g2_font_fub49_tn");
extern const uint8_t u8g2_font_fub11_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub11_t_symbol");
extern const uint8_t u8g2_font_fub14_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub14_t_symbol");
extern const uint8_t u8g2_font_fub17_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub17_t_symbol");
extern const uint8_t u8g2_font_fub20_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub20_t_symbol");
extern const uint8_t u8g2_font_fub25_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub25_t_symbol");
extern const uint8_t u8g2_font_fub30_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub30_t_symbol");
extern const uint8_t u8g2_font_fub35_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub35_t_symbol");
extern const uint8_t u8g2_font_fub42_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub42_t_symbol");
extern const uint8_t u8g2_font_fub49_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fub49_t_symbol");
extern const uint8_t u8g2_font_fur11_tf[] U8G2_FONT_SECTION("u8g2_font_fur11_tf");
extern const uint8_t u8g2_font_fur11_tr[] U8G2_FONT_SECTION("u8g2_font_fur11_tr");
extern const uint8_t u8g2_font_fur11_tn[] U8G2_FONT_SECTION("u8g2_font_fur11_tn");
extern const uint8_t u8g2_font_fur14_tf[] U8G2_FONT_SECTION("u8g2_font_fur14_tf");
extern const uint8_t u8g2_font_fur14_tr[] U8G2_FONT_SECTION("u8g2_font_fur14_tr");
extern const uint8_t u8g2_font_fur14_tn[] U8G2_FONT_SECTION("u8g2_font_fur14_tn");
extern const uint8_t u8g2_font_fur17_tf[] U8G2_FONT_SECTION("u8g2_font_fur17_tf");
extern const uint8_t u8g2_font_fur17_tr[] U8G2_FONT_SECTION("u8g2_font_fur17_tr");
extern const uint8_t u8g2_font_fur17_tn[] U8G2_FONT_SECTION("u8g2_font_fur17_tn");
extern const uint8_t u8g2_font_fur20_tf[] U8G2_FONT_SECTION("u8g2_font_fur20_tf");
extern const uint8_t u8g2_font_fur20_tr[] U8G2_FONT_SECTION("u8g2_font_fur20_tr");
extern const uint8_t u8g2_font_fur20_tn[] U8G2_FONT_SECTION("u8g2_font_fur20_tn");
extern const uint8_t u8g2_font_fur25_tf[] U8G2_FONT_SECTION("u8g2_font_fur25_tf");
extern const uint8_t u8g2_font_fur25_tr[] U8G2_FONT_SECTION("u8g2_font_fur25_tr");
extern const uint8_t u8g2_font_fur25_tn[] U8G2_FONT_SECTION("u8g2_font_fur25_tn");
extern const uint8_t u8g2_font_fur30_tf[] U8G2_FONT_SECTION("u8g2_font_fur30_tf");
extern const uint8_t u8g2_font_fur30_tr[] U8G2_FONT_SECTION("u8g2_font_fur30_tr");
extern const uint8_t u8g2_font_fur30_tn[] U8G2_FONT_SECTION("u8g2_font_fur30_tn");
extern const uint8_t u8g2_font_fur35_tf[] U8G2_FONT_SECTION("u8g2_font_fur35_tf");
extern const uint8_t u8g2_font_fur35_tr[] U8G2_FONT_SECTION("u8g2_font_fur35_tr");
extern const uint8_t u8g2_font_fur35_tn[] U8G2_FONT_SECTION("u8g2_font_fur35_tn");
extern const uint8_t u8g2_font_fur42_tf[] U8G2_FONT_SECTION("u8g2_font_fur42_tf");
extern const uint8_t u8g2_font_fur42_tr[] U8G2_FONT_SECTION("u8g2_font_fur42_tr");
extern const uint8_t u8g2_font_fur42_tn[] U8G2_FONT_SECTION("u8g2_font_fur42_tn");
extern const uint8_t u8g2_font_fur49_tn[] U8G2_FONT_SECTION("u8g2_font_fur49_tn");
extern const uint8_t u8g2_font_fur11_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur11_t_symbol");
extern const uint8_t u8g2_font_fur14_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur14_t_symbol");
extern const uint8_t u8g2_font_fur17_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur17_t_symbol");
extern const uint8_t u8g2_font_fur20_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur20_t_symbol");
extern const uint8_t u8g2_font_fur25_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur25_t_symbol");
extern const uint8_t u8g2_font_fur30_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur30_t_symbol");
extern const uint8_t u8g2_font_fur35_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur35_t_symbol");
extern const uint8_t u8g2_font_fur42_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur42_t_symbol");
extern const uint8_t u8g2_font_fur49_t_symbol[] U8G2_FONT_SECTION("u8g2_font_fur49_t_symbol");
extern const uint8_t u8g2_font_osb18_tf[] U8G2_FONT_SECTION("u8g2_font_osb18_tf");
extern const uint8_t u8g2_font_osb18_tr[] U8G2_FONT_SECTION("u8g2_font_osb18_tr");
extern const uint8_t u8g2_font_osb18_tn[] U8G2_FONT_SECTION("u8g2_font_osb18_tn");
extern const uint8_t u8g2_font_osb21_tf[] U8G2_FONT_SECTION("u8g2_font_osb21_tf");
extern const uint8_t u8g2_font_osb21_tr[] U8G2_FONT_SECTION("u8g2_font_osb21_tr");
extern const uint8_t u8g2_font_osb21_tn[] U8G2_FONT_SECTION("u8g2_font_osb21_tn");
extern const uint8_t u8g2_font_osb26_tf[] U8G2_FONT_SECTION("u8g2_font_osb26_tf");
extern const uint8_t u8g2_font_osb26_tr[] U8G2_FONT_SECTION("u8g2_font_osb26_tr");
extern const uint8_t u8g2_font_osb26_tn[] U8G2_FONT_SECTION("u8g2_font_osb26_tn");
extern const uint8_t u8g2_font_osb29_tf[] U8G2_FONT_SECTION("u8g2_font_osb29_tf");
extern const uint8_t u8g2_font_osb29_tr[] U8G2_FONT_SECTION("u8g2_font_osb29_tr");
extern const uint8_t u8g2_font_osb29_tn[] U8G2_FONT_SECTION("u8g2_font_osb29_tn");
extern const uint8_t u8g2_font_osb35_tf[] U8G2_FONT_SECTION("u8g2_font_osb35_tf");
extern const uint8_t u8g2_font_osb35_tr[] U8G2_FONT_SECTION("u8g2_font_osb35_tr");
extern const uint8_t u8g2_font_osb35_tn[] U8G2_FONT_SECTION("u8g2_font_osb35_tn");
extern const uint8_t u8g2_font_osb41_tf[] U8G2_FONT_SECTION("u8g2_font_osb41_tf");
extern const uint8_t u8g2_font_osb41_tr[] U8G2_FONT_SECTION("u8g2_font_osb41_tr");
extern const uint8_t u8g2_font_osb41_tn[] U8G2_FONT_SECTION("u8g2_font_osb41_tn");
extern const uint8_t u8g2_font_osr18_tf[] U8G2_FONT_SECTION("u8g2_font_osr18_tf");
extern const uint8_t u8g2_font_osr18_tr[] U8G2_FONT_SECTION("u8g2_font_osr18_tr");
extern const uint8_t u8g2_font_osr18_tn[] U8G2_FONT_SECTION("u8g2_font_osr18_tn");
extern const uint8_t u8g2_font_osr21_tf[] U8G2_FONT_SECTION("u8g2_font_osr21_tf");
extern const uint8_t u8g2_font_osr21_tr[] U8G2_FONT_SECTION("u8g2_font_osr21_tr");
extern const uint8_t u8g2_font_osr21_tn[] U8G2_FONT_SECTION("u8g2_font_osr21_tn");
extern const uint8_t u8g2_font_osr26_tf[] U8G2_FONT_SECTION("u8g2_font_osr26_tf");
extern const uint8_t u8g2_font_osr26_tr[] U8G2_FONT_SECTION("u8g2_font_osr26_tr");
extern const uint8_t u8g2_font_osr26_tn[] U8G2_FONT_SECTION("u8g2_font_osr26_tn");
extern const uint8_t u8g2_font_osr29_tf[] U8G2_FONT_SECTION("u8g2_font_osr29_tf");
extern const uint8_t u8g2_font_osr29_tr[] U8G2_FONT_SECTION("u8g2_font_osr29_tr");
extern const uint8_t u8g2_font_osr29_tn[] U8G2_FONT_SECTION("u8g2_font_osr29_tn");
extern const uint8_t u8g2_font_osr35_tf[] U8G2_FONT_SECTION("u8g2_font_osr35_tf");
extern const uint8_t u8g2_font_osr35_tr[] U8G2_FONT_SECTION("u8g2_font_osr35_tr");
extern const uint8_t u8g2_font_osr35_tn[] U8G2_FONT_SECTION("u8g2_font_osr35_tn");
extern const uint8_t u8g2_font_osr41_tf[] U8G2_FONT_SECTION("u8g2_font_osr41_tf");
extern const uint8_t u8g2_font_osr41_tr[] U8G2_FONT_SECTION("u8g2_font_osr41_tr");
extern const uint8_t u8g2_font_osr41_tn[] U8G2_FONT_SECTION("u8g2_font_osr41_tn");
extern const uint8_t u8g2_font_inr16_mf[] U8G2_FONT_SECTION("u8g2_font_inr16_mf");
extern const uint8_t u8g2_font_inr16_mr[] U8G2_FONT_SECTION("u8g2_font_inr16_mr");
extern const uint8_t u8g2_font_inr16_mn[] U8G2_FONT_SECTION("u8g2_font_inr16_mn");
extern const uint8_t u8g2_font_inr19_mf[] U8G2_FONT_SECTION("u8g2_font_inr19_mf");
extern const uint8_t u8g2_font_inr19_mr[] U8G2_FONT_SECTION("u8g2_font_inr19_mr");
extern const uint8_t u8g2_font_inr19_mn[] U8G2_FONT_SECTION("u8g2_font_inr19_mn");
extern const uint8_t u8g2_font_inr21_mf[] U8G2_FONT_SECTION("u8g2_font_inr21_mf");
extern const uint8_t u8g2_font_inr21_mr[] U8G2_FONT_SECTION("u8g2_font_inr21_mr");
extern const uint8_t u8g2_font_inr21_mn[] U8G2_FONT_SECTION("u8g2_font_inr21_mn");
extern const uint8_t u8g2_font_inr24_mf[] U8G2_FONT_SECTION("u8g2_font_inr24_mf");
extern const uint8_t u8g2_font_inr24_mr[] U8G2_FONT_SECTION("u8g2_font_inr24_mr");
extern const uint8_t u8g2_font_inr24_mn[] U8G2_FONT_SECTION("u8g2_font_inr24_mn");
extern const uint8_t u8g2_font_inr24_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr24_t_cyrillic");
extern const uint8_t u8g2_font_inr27_mf[] U8G2_FONT_SECTION("u8g2_font_inr27_mf");
extern const uint8_t u8g2_font_inr27_mr[] U8G2_FONT_SECTION("u8g2_font_inr27_mr");
extern const uint8_t u8g2_font_inr27_mn[] U8G2_FONT_SECTION("u8g2_font_inr27_mn");
extern const uint8_t u8g2_font_inr27_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr27_t_cyrillic");
extern const uint8_t u8g2_font_inr30_mf[] U8G2_FONT_SECTION("u8g2_font_inr30_mf");
extern const uint8_t u8g2_font_inr30_mr[] U8G2_FONT_SECTION("u8g2_font_inr30_mr");
extern const uint8_t u8g2_font_inr30_mn[] U8G2_FONT_SECTION("u8g2_font_inr30_mn");
extern const uint8_t u8g2_font_inr30_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr30_t_cyrillic");
extern const uint8_t u8g2_font_inr33_mf[] U8G2_FONT_SECTION("u8g2_font_inr33_mf");
extern const uint8_t u8g2_font_inr33_mr[] U8G2_FONT_SECTION("u8g2_font_inr33_mr");
extern const uint8_t u8g2_font_inr33_mn[] U8G2_FONT_SECTION("u8g2_font_inr33_mn");
extern const uint8_t u8g2_font_inr33_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr33_t_cyrillic");
extern const uint8_t u8g2_font_inr38_mf[] U8G2_FONT_SECTION("u8g2_font_inr38_mf");
extern const uint8_t u8g2_font_inr38_mr[] U8G2_FONT_SECTION("u8g2_font_inr38_mr");
extern const uint8_t u8g2_font_inr38_mn[] U8G2_FONT_SECTION("u8g2_font_inr38_mn");
extern const uint8_t u8g2_font_inr38_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr38_t_cyrillic");
extern const uint8_t u8g2_font_inr42_mf[] U8G2_FONT_SECTION("u8g2_font_inr42_mf");
extern const uint8_t u8g2_font_inr42_mr[] U8G2_FONT_SECTION("u8g2_font_inr42_mr");
extern const uint8_t u8g2_font_inr42_mn[] U8G2_FONT_SECTION("u8g2_font_inr42_mn");
extern const uint8_t u8g2_font_inr42_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr42_t_cyrillic");
extern const uint8_t u8g2_font_inr46_mf[] U8G2_FONT_SECTION("u8g2_font_inr46_mf");
extern const uint8_t u8g2_font_inr46_mr[] U8G2_FONT_SECTION("u8g2_font_inr46_mr");
extern const uint8_t u8g2_font_inr46_mn[] U8G2_FONT_SECTION("u8g2_font_inr46_mn");
extern const uint8_t u8g2_font_inr46_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr46_t_cyrillic");
extern const uint8_t u8g2_font_inr49_mf[] U8G2_FONT_SECTION("u8g2_font_inr49_mf");
extern const uint8_t u8g2_font_inr49_mr[] U8G2_FONT_SECTION("u8g2_font_inr49_mr");
extern const uint8_t u8g2_font_inr49_mn[] U8G2_FONT_SECTION("u8g2_font_inr49_mn");
extern const uint8_t u8g2_font_inr49_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr49_t_cyrillic");
extern const uint8_t u8g2_font_inr53_mf[] U8G2_FONT_SECTION("u8g2_font_inr53_mf");
extern const uint8_t u8g2_font_inr53_mr[] U8G2_FONT_SECTION("u8g2_font_inr53_mr");
extern const uint8_t u8g2_font_inr53_mn[] U8G2_FONT_SECTION("u8g2_font_inr53_mn");
extern const uint8_t u8g2_font_inr53_t_cyrillic[] U8G2_FONT_SECTION("u8g2_font_inr53_t_cyrillic");
extern const uint8_t u8g2_font_inr57_mn[] U8G2_FONT_SECTION("u8g2_font_inr57_mn");
extern const uint8_t u8g2_font_inr62_mn[] U8G2_FONT_SECTION("u8g2_font_inr62_mn");
extern const uint8_t u8g2_font_inb16_mf[] U8G2_FONT_SECTION("u8g2_font_inb16_mf");
extern const uint8_t u8g2_font_inb16_mr[] U8G2_FONT_SECTION("u8g2_font_inb16_mr");
extern const uint8_t u8g2_font_inb16_mn[] U8G2_FONT_SECTION("u8g2_font_inb16_mn");
extern const uint8_t u8g2_font_inb19_mf[] U8G2_FONT_SECTION("u8g2_font_inb19_mf");
extern const uint8_t u8g2_font_inb19_mr[] U8G2_FONT_SECTION("u8g2_font_inb19_mr");
extern const uint8_t u8g2_font_inb19_mn[] U8G2_FONT_SECTION("u8g2_font_inb19_mn");
extern const uint8_t u8g2_font_inb21_mf[] U8G2_FONT_SECTION("u8g2_font_inb21_mf");
extern const uint8_t u8g2_font_inb21_mr[] U8G2_FONT_SECTION("u8g2_font_inb21_mr");
extern const uint8_t u8g2_font_inb21_mn[] U8G2_FONT_SECTION("u8g2_font_inb21_mn");
extern const uint8_t u8g2_font_inb24_mf[] U8G2_FONT_SECTION("u8g2_font_inb24_mf");
extern const uint8_t u8g2_font_inb24_mr[] U8G2_FONT_SECTION("u8g2_font_inb24_mr");
extern const uint8_t u8g2_font_inb24_mn[] U8G2_FONT_SECTION("u8g2_font_inb24_mn");
extern const uint8_t u8g2_font_inb27_mf[] U8G2_FONT_SECTION("u8g2_font_inb27_mf");
extern const uint8_t u8g2_font_inb27_mr[] U8G2_FONT_SECTION("u8g2_font_inb27_mr");
extern const uint8_t u8g2_font_inb27_mn[] U8G2_FONT_SECTION("u8g2_font_inb27_mn");
extern const uint8_t u8g2_font_inb30_mf[] U8G2_FONT_SECTION("u8g2_font_inb30_mf");
extern const uint8_t u8g2_font_inb30_mr[] U8G2_FONT_SECTION("u8g2_font_inb30_mr");
extern const uint8_t u8g2_font_inb30_mn[] U8G2_FONT_SECTION("u8g2_font_inb30_mn");
extern const uint8_t u8g2_font_inb33_mf[] U8G2_FONT_SECTION("u8g2_font_inb33_mf");
extern const uint8_t u8g2_font_inb33_mr[] U8G2_FONT_SECTION("u8g2_font_inb33_mr");
extern const uint8_t u8g2_font_inb33_mn[] U8G2_FONT_SECTION("u8g2_font_inb33_mn");
extern const uint8_t u8g2_font_inb38_mf[] U8G2_FONT_SECTION("u8g2_font_inb38_mf");
extern const uint8_t u8g2_font_inb38_mr[] U8G2_FONT_SECTION("u8g2_font_inb38_mr");
extern const uint8_t u8g2_font_inb38_mn[] U8G2_FONT_SECTION("u8g2_font_inb38_mn");
extern const uint8_t u8g2_font_inb42_mf[] U8G2_FONT_SECTION("u8g2_font_inb42_mf");
extern const uint8_t u8g2_font_inb42_mr[] U8G2_FONT_SECTION("u8g2_font_inb42_mr");
extern const uint8_t u8g2_font_inb42_mn[] U8G2_FONT_SECTION("u8g2_font_inb42_mn");
extern const uint8_t u8g2_font_inb46_mf[] U8G2_FONT_SECTION("u8g2_font_inb46_mf");
extern const uint8_t u8g2_font_inb46_mr[] U8G2_FONT_SECTION("u8g2_font_inb46_mr");
extern const uint8_t u8g2_font_inb46_mn[] U8G2_FONT_SECTION("u8g2_font_inb46_mn");
extern const uint8_t u8g2_font_inb49_mf[] U8G2_FONT_SECTION("u8g2_font_inb49_mf");
extern const uint8_t u8g2_font_inb49_mr[] U8G2_FONT_SECTION("u8g2_font_inb49_mr");
extern const uint8_t u8g2_font_inb49_mn[] U8G2_FONT_SECTION("u8g2_font_inb49_mn");
extern const uint8_t u8g2_font_inb53_mf[] U8G2_FONT_SECTION("u8g2_font_inb53_mf");
extern const uint8_t u8g2_font_inb53_mr[] U8G2_FONT_SECTION("u8g2_font_inb53_mr");
extern const uint8_t u8g2_font_inb53_mn[] U8G2_FONT_SECTION("u8g2_font_inb53_mn");
extern const uint8_t u8g2_font_inb57_mn[] U8G2_FONT_SECTION("u8g2_font_inb57_mn");
extern const uint8_t u8g2_font_inb63_mn[] U8G2_FONT_SECTION("u8g2_font_inb63_mn");
extern const uint8_t u8g2_font_logisoso16_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso16_tf");
extern const uint8_t u8g2_font_logisoso16_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso16_tr");
extern const uint8_t u8g2_font_logisoso16_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso16_tn");
extern const uint8_t u8g2_font_logisoso18_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso18_tf");
extern const uint8_t u8g2_font_logisoso18_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso18_tr");
extern const uint8_t u8g2_font_logisoso18_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso18_tn");
extern const uint8_t u8g2_font_logisoso20_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso20_tf");
extern const uint8_t u8g2_font_logisoso20_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso20_tr");
extern const uint8_t u8g2_font_logisoso20_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso20_tn");
extern const uint8_t u8g2_font_logisoso22_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso22_tf");
extern const uint8_t u8g2_font_logisoso22_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso22_tr");
extern const uint8_t u8g2_font_logisoso22_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso22_tn");
extern const uint8_t u8g2_font_logisoso24_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso24_tf");
extern const uint8_t u8g2_font_logisoso24_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso24_tr");
extern const uint8_t u8g2_font_logisoso24_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso24_tn");
extern const uint8_t u8g2_font_logisoso26_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso26_tf");
extern const uint8_t u8g2_font_logisoso26_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso26_tr");
extern const uint8_t u8g2_font_logisoso26_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso26_tn");
extern const uint8_t u8g2_font_logisoso28_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso28_tf");
extern const uint8_t u8g2_font_logisoso28_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso28_tr");
extern const uint8_t u8g2_font_logisoso28_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso28_tn");
extern const uint8_t u8g2_font_logisoso30_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso30_tf");
extern const uint8_t u8g2_font_logisoso30_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso30_tr");
extern const uint8_t u8g2_font_logisoso30_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso30_tn");
extern const uint8_t u8g2_font_logisoso32_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso32_tf");
extern const uint8_t u8g2_font_logisoso32_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso32_tr");
extern const uint8_t u8g2_font_logisoso32_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso32_tn");
extern const uint8_t u8g2_font_logisoso34_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso34_tf");
extern const uint8_t u8g2_font_logisoso34_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso34_tr");
extern const uint8_t u8g2_font_logisoso34_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso34_tn");
extern const uint8_t u8g2_font_logisoso38_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso38_tf");
extern const uint8_t u8g2_font_logisoso38_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso38_tr");
extern const uint8_t u8g2_font_logisoso38_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso38_tn");
extern const uint8_t u8g2_font_logisoso42_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso42_tf");
extern const uint8_t u8g2_font_logisoso42_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso42_tr");
extern const uint8_t u8g2_font_logisoso42_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso42_tn");
extern const uint8_t u8g2_font_logisoso46_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso46_tf");
extern const uint8_t u8g2_font_logisoso46_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso46_tr");
extern const uint8_t u8g2_font_logisoso46_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso46_tn");
extern const uint8_t u8g2_font_logisoso50_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso50_tf");
extern const uint8_t u8g2_font_logisoso50_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso50_tr");
extern const uint8_t u8g2_font_logisoso50_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso50_tn");
extern const uint8_t u8g2_font_logisoso54_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso54_tf");
extern const uint8_t u8g2_font_logisoso54_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso54_tr");
extern const uint8_t u8g2_font_logisoso54_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso54_tn");
extern const uint8_t u8g2_font_logisoso58_tf[] U8G2_FONT_SECTION("u8g2_font_logisoso58_tf");
extern const uint8_t u8g2_font_logisoso58_tr[] U8G2_FONT_SECTION("u8g2_font_logisoso58_tr");
extern const uint8_t u8g2_font_logisoso58_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso58_tn");
extern const uint8_t u8g2_font_logisoso62_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso62_tn");
extern const uint8_t u8g2_font_logisoso78_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso78_tn");
extern const uint8_t u8g2_font_logisoso92_tn[] U8G2_FONT_SECTION("u8g2_font_logisoso92_tn");
extern const uint8_t u8g2_font_pressstart2p_8f[] U8G2_FONT_SECTION("u8g2_font_pressstart2p_8f");
extern const uint8_t u8g2_font_pressstart2p_8r[] U8G2_FONT_SECTION("u8g2_font_pressstart2p_8r");
extern const uint8_t u8g2_font_pressstart2p_8n[] U8G2_FONT_SECTION("u8g2_font_pressstart2p_8n");
extern const uint8_t u8g2_font_pressstart2p_8u[] U8G2_FONT_SECTION("u8g2_font_pressstart2p_8u");
extern const uint8_t u8g2_font_pcsenior_8f[] U8G2_FONT_SECTION("u8g2_font_pcsenior_8f");
extern const uint8_t u8g2_font_pcsenior_8r[] U8G2_FONT_SECTION("u8g2_font_pcsenior_8r");
extern const uint8_t u8g2_font_pcsenior_8n[] U8G2_FONT_SECTION("u8g2_font_pcsenior_8n");
extern const uint8_t u8g2_font_pcsenior_8u[] U8G2_FONT_SECTION("u8g2_font_pcsenior_8u");
extern const uint8_t u8g2_font_pxplusibmcgathin_8f[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcgathin_8f");
extern const uint8_t u8g2_font_pxplusibmcgathin_8r[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcgathin_8r");
extern const uint8_t u8g2_font_pxplusibmcgathin_8n[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcgathin_8n");
extern const uint8_t u8g2_font_pxplusibmcgathin_8u[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcgathin_8u");
extern const uint8_t u8g2_font_pxplusibmcga_8f[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcga_8f");
extern const uint8_t u8g2_font_pxplusibmcga_8r[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcga_8r");
extern const uint8_t u8g2_font_pxplusibmcga_8n[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcga_8n");
extern const uint8_t u8g2_font_pxplusibmcga_8u[] U8G2_FONT_SECTION("u8g2_font_pxplusibmcga_8u");
extern const uint8_t u8g2_font_pxplustandynewtv_8f[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8f");
extern const uint8_t u8g2_font_pxplustandynewtv_8r[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8r");
extern const uint8_t u8g2_font_pxplustandynewtv_8n[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8n");
extern const uint8_t u8g2_font_pxplustandynewtv_8u[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8u");
extern const uint8_t u8g2_font_pxplustandynewtv_t_all[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_t_all");
extern const uint8_t u8g2_font_pxplustandynewtv_8_all[] U8G2_FONT_SECTION("u8g2_font_pxplustandynewtv_8_all");
extern const uint8_t u8g2_font_pxplusibmvga9_tf[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_tf");
extern const uint8_t u8g2_font_pxplusibmvga9_tr[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_tr");
extern const uint8_t u8g2_font_pxplusibmvga9_tn[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_tn");
extern const uint8_t u8g2_font_pxplusibmvga9_mf[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_mf");
extern const uint8_t u8g2_font_pxplusibmvga9_mr[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_mr");
extern const uint8_t u8g2_font_pxplusibmvga9_mn[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_mn");
extern const uint8_t u8g2_font_pxplusibmvga9_t_all[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_t_all");
extern const uint8_t u8g2_font_pxplusibmvga9_m_all[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga9_m_all");
extern const uint8_t u8g2_font_pxplusibmvga8_tf[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_tf");
extern const uint8_t u8g2_font_pxplusibmvga8_tr[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_tr");
extern const uint8_t u8g2_font_pxplusibmvga8_tn[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_tn");
extern const uint8_t u8g2_font_pxplusibmvga8_mf[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_mf");
extern const uint8_t u8g2_font_pxplusibmvga8_mr[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_mr");
extern const uint8_t u8g2_font_pxplusibmvga8_mn[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_mn");
extern const uint8_t u8g2_font_pxplusibmvga8_t_all[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_t_all");
extern const uint8_t u8g2_font_pxplusibmvga8_m_all[] U8G2_FONT_SECTION("u8g2_font_pxplusibmvga8_m_all");
extern const uint8_t u8g2_font_px437wyse700a_tf[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_tf");
extern const uint8_t u8g2_font_px437wyse700a_tr[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_tr");
extern const uint8_t u8g2_font_px437wyse700a_tn[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_tn");
extern const uint8_t u8g2_font_px437wyse700a_mf[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_mf");
extern const uint8_t u8g2_font_px437wyse700a_mr[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_mr");
extern const uint8_t u8g2_font_px437wyse700a_mn[] U8G2_FONT_SECTION("u8g2_font_px437wyse700a_mn");
extern const uint8_t u8g2_font_px437wyse700b_tf[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_tf");
extern const uint8_t u8g2_font_px437wyse700b_tr[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_tr");
extern const uint8_t u8g2_font_px437wyse700b_tn[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_tn");
extern const uint8_t u8g2_font_px437wyse700b_mf[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_mf");
extern const uint8_t u8g2_font_px437wyse700b_mr[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_mr");
extern const uint8_t u8g2_font_px437wyse700b_mn[] U8G2_FONT_SECTION("u8g2_font_px437wyse700b_mn");

/* end font list */


#endif /* U8g2_for_Adafruit_GFX_h */


