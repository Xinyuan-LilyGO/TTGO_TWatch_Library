/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts:
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef FONT_JETBRAINSMONO
#define FONT_JETBRAINSMONO 1
#endif

#if FONT_JETBRAINSMONO

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0x55, 0x55, 0xf,

    /* U+0022 "\"" */
    0x8c, 0x63, 0x18, 0x80,

    /* U+0023 "#" */
    0x11, 0x8, 0x84, 0x82, 0x47, 0xf9, 0x10, 0x88,
    0x44, 0xff, 0x12, 0x19, 0x8, 0x80,

    /* U+0024 "$" */
    0x10, 0x20, 0xf3, 0xb5, 0x2a, 0x1c, 0x1c, 0x1c,
    0x2c, 0x4c, 0x9d, 0x6f, 0x84, 0x8,

    /* U+0025 "%" */
    0x60, 0x48, 0x24, 0x12, 0x19, 0x33, 0x30, 0x60,
    0x4e, 0x48, 0xc4, 0x42, 0x20, 0xe0,

    /* U+0026 "&" */
    0x38, 0x44, 0x44, 0x40, 0x60, 0x60, 0x92, 0x9e,
    0x8c, 0x84, 0x8e, 0x72,

    /* U+0027 "'" */
    0xf8,

    /* U+0028 "(" */
    0x0, 0xd8, 0x88, 0x42, 0x10, 0x84, 0x21, 0x4,
    0x30, 0xe1,

    /* U+0029 ")" */
    0x87, 0xc, 0x20, 0x84, 0x21, 0x8, 0x42, 0x11,
    0x99, 0x98,

    /* U+002A "*" */
    0x8, 0x4, 0x12, 0x4f, 0xe1, 0x80, 0xa0, 0xd8,
    0x44,

    /* U+002B "+" */
    0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10,

    /* U+002C "," */
    0x6b, 0x68,

    /* U+002D "-" */
    0xfc,

    /* U+002E "." */
    0xff, 0x80,

    /* U+002F "/" */
    0x2, 0xc, 0x10, 0x20, 0xc1, 0x6, 0xc, 0x10,
    0x60, 0x81, 0x6, 0x8, 0x30, 0x0,

    /* U+0030 "0" */
    0x3c, 0x8a, 0xc, 0x19, 0x32, 0x64, 0xc9, 0x83,
    0x5, 0x13, 0xc0,

    /* U+0031 "1" */
    0x30, 0xe2, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,
    0x20, 0x47, 0xf0,

    /* U+0032 "2" */
    0x7d, 0x8e, 0x8, 0x10, 0x20, 0xc1, 0x4, 0x30,
    0xc3, 0x7, 0xf0,

    /* U+0033 "3" */
    0xfc, 0x8, 0x20, 0xc3, 0x7, 0x1, 0x1, 0x3,
    0x7, 0x13, 0xc0,

    /* U+0034 "4" */
    0x8, 0x61, 0x8, 0x21, 0x4, 0x61, 0xfc, 0x10,
    0x41,

    /* U+0035 "5" */
    0xff, 0x2, 0x4, 0x8, 0x17, 0xb1, 0x81, 0x2,
    0x7, 0x1b, 0xc0,

    /* U+0036 "6" */
    0x18, 0x30, 0xc1, 0x6, 0xf, 0xb1, 0x41, 0x83,
    0x5, 0x11, 0xc0,

    /* U+0037 "7" */
    0xff, 0xe, 0x18, 0x60, 0xc1, 0x6, 0xc, 0x30,
    0x60, 0x83, 0x0,

    /* U+0038 "8" */
    0x7d, 0x8e, 0xc, 0x1c, 0x67, 0x11, 0x41, 0x83,
    0x7, 0x11, 0xc0,

    /* U+0039 "9" */
    0x38, 0x8a, 0xc, 0x18, 0x38, 0xdf, 0x6, 0x8,
    0x30, 0xc1, 0x80,

    /* U+003A ":" */
    0xfc, 0x0, 0x7, 0xe0,

    /* U+003B ";" */
    0x77, 0x0, 0x0, 0x76, 0x66, 0x4c,

    /* U+003C "<" */
    0x0, 0xc, 0x73, 0x8c, 0x1c, 0xe, 0x7, 0x2,

    /* U+003D "=" */
    0xfe, 0x0, 0x0, 0xf, 0xe0,

    /* U+003E ">" */
    0x1, 0x81, 0xc0, 0xe0, 0x61, 0xce, 0x70, 0x80,

    /* U+003F "?" */
    0xf0, 0x20, 0x41, 0x4, 0x33, 0x88, 0x20, 0x6,
    0x18,

    /* U+0040 "@" */
    0x3c, 0x42, 0x81, 0x81, 0x8d, 0x93, 0x91, 0x91,
    0x91, 0x91, 0x8f, 0x80, 0xc0, 0x40, 0x38,

    /* U+0041 "A" */
    0x18, 0x38, 0x38, 0x2c, 0x24, 0x64, 0x66, 0x46,
    0x7e, 0xc2, 0xc3, 0x83,

    /* U+0042 "B" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0xbe, 0x43, 0x83,
    0x6, 0x1f, 0xe0,

    /* U+0043 "C" */
    0x79, 0x9a, 0x1c, 0x8, 0x10, 0x20, 0x40, 0x81,
    0xf, 0x33, 0xc0,

    /* U+0044 "D" */
    0xf9, 0xa, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x6, 0x17, 0xc0,

    /* U+0045 "E" */
    0xfe, 0x8, 0x20, 0x82, 0xf, 0xe0, 0x82, 0x8,
    0x3f,

    /* U+0046 "F" */
    0xfe, 0x8, 0x20, 0x82, 0xf, 0xe0, 0x82, 0x8,
    0x20,

    /* U+0047 "G" */
    0x3c, 0x8e, 0xc, 0x8, 0x10, 0x27, 0xc1, 0x83,
    0x5, 0x1b, 0xe0,

    /* U+0048 "H" */
    0x83, 0x6, 0xc, 0x18, 0x3f, 0xe0, 0xc1, 0x83,
    0x6, 0xc, 0x10,

    /* U+0049 "I" */
    0xfe, 0x20, 0x40, 0x81, 0x2, 0x4, 0x8, 0x10,
    0x20, 0x47, 0xf0,

    /* U+004A "J" */
    0x3e, 0x4, 0x8, 0x10, 0x20, 0x40, 0x81, 0x3,
    0x85, 0x11, 0xe0,

    /* U+004B "K" */
    0x87, 0xa, 0x34, 0x49, 0x9e, 0x26, 0x44, 0x8d,
    0x1a, 0x1c, 0x30,

    /* U+004C "L" */
    0x82, 0x8, 0x20, 0x82, 0x8, 0x20, 0x82, 0x8,
    0x3f,

    /* U+004D "M" */
    0xc7, 0x8f, 0x1d, 0x5a, 0xb5, 0x64, 0xc1, 0x83,
    0x6, 0xc, 0x10,

    /* U+004E "N" */
    0xc3, 0x87, 0x8d, 0x1b, 0x32, 0x66, 0xcd, 0x8b,
    0x1e, 0x1c, 0x30,

    /* U+004F "O" */
    0x3c, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x5, 0x13, 0xc0,

    /* U+0050 "P" */
    0xfd, 0xa, 0xc, 0x18, 0x30, 0xbe, 0x40, 0x81,
    0x2, 0x4, 0x0,

    /* U+0051 "Q" */
    0x3c, 0x8a, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x5, 0x19, 0xe0, 0xc0, 0x80, 0x80,

    /* U+0052 "R" */
    0xfd, 0xa, 0xc, 0x18, 0x30, 0xff, 0x44, 0x8d,
    0xa, 0x1c, 0x10,

    /* U+0053 "S" */
    0x3c, 0xcd, 0xa, 0x6, 0x7, 0x7, 0x3, 0x3,
    0x7, 0x1b, 0xe0,

    /* U+0054 "T" */
    0xff, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8,
    0x8, 0x8, 0x8, 0x8,

    /* U+0055 "U" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83,
    0x7, 0x1b, 0xe0,

    /* U+0056 "V" */
    0x83, 0xc3, 0xc2, 0x42, 0x46, 0x64, 0x64, 0x24,
    0x2c, 0x28, 0x38, 0x18,

    /* U+0057 "W" */
    0x99, 0xcc, 0xa6, 0x5b, 0x2d, 0x56, 0xab, 0x54,
    0xca, 0x65, 0x33, 0x99, 0xcc, 0xe0,

    /* U+0058 "X" */
    0xc3, 0x46, 0x64, 0x2c, 0x38, 0x18, 0x18, 0x3c,
    0x2c, 0x66, 0xc2, 0xc3,

    /* U+0059 "Y" */
    0x41, 0x31, 0x88, 0x84, 0x43, 0x60, 0xa0, 0x50,
    0x10, 0x8, 0x4, 0x2, 0x1, 0x0,

    /* U+005A "Z" */
    0xfe, 0xc, 0x10, 0x60, 0x83, 0xc, 0x10, 0x60,
    0x83, 0x7, 0xf0,

    /* U+005B "[" */
    0xf2, 0x49, 0x24, 0x92, 0x49, 0x38,

    /* U+005C "\\" */
    0xc0, 0x81, 0x81, 0x2, 0x6, 0x4, 0xc, 0x18,
    0x10, 0x30, 0x20, 0x40, 0xc0, 0x80,

    /* U+005D "]" */
    0xe4, 0x92, 0x49, 0x24, 0x92, 0x78,

    /* U+005E "^" */
    0x10, 0x30, 0xe1, 0x26, 0x48, 0xf0, 0x80,

    /* U+005F "_" */
    0xff,

    /* U+0060 "`" */
    0x89, 0x80,

    /* U+0061 "a" */
    0x3d, 0x8c, 0x8, 0x17, 0xf0, 0x60, 0xc3, 0x7a,

    /* U+0062 "b" */
    0x81, 0x2, 0x5, 0xec, 0x70, 0x60, 0xc1, 0x83,
    0x7, 0x1d, 0xe0,

    /* U+0063 "c" */
    0x79, 0x9a, 0x1c, 0x8, 0x10, 0x21, 0xe6, 0x78,

    /* U+0064 "d" */
    0x2, 0x4, 0xb, 0xdc, 0x70, 0x60, 0xc1, 0x83,
    0x7, 0x1b, 0xd0,

    /* U+0065 "e" */
    0x7d, 0x8e, 0xc, 0x1f, 0xf0, 0x20, 0x61, 0x7c,

    /* U+0066 "f" */
    0xf, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10,

    /* U+0067 "g" */
    0x7b, 0x8f, 0xe, 0x1c, 0x38, 0x71, 0xbd, 0x2,
    0x4, 0x1b, 0xe0,

    /* U+0068 "h" */
    0x81, 0x2, 0x5, 0xec, 0x70, 0x60, 0xc1, 0x83,
    0x6, 0xc, 0x10,

    /* U+0069 "i" */
    0x0, 0x30, 0x0, 0xf, 0x81, 0x2, 0x4, 0x8,
    0x10, 0x20, 0x4f, 0xe0,

    /* U+006A "j" */
    0x0, 0x30, 0x0, 0x7c, 0x10, 0x41, 0x4, 0x10,
    0x41, 0x4, 0x10, 0xbc,

    /* U+006B "k" */
    0x81, 0x2, 0x4, 0x38, 0xd1, 0x26, 0x78, 0x99,
    0x12, 0x34, 0x30,

    /* U+006C "l" */
    0xf0, 0x8, 0x4, 0x2, 0x1, 0x0, 0x80, 0x40,
    0x20, 0x10, 0x8, 0x4, 0x1, 0xf0,

    /* U+006D "m" */
    0xfd, 0x26, 0x4c, 0x99, 0x32, 0x64, 0xc9, 0x92,

    /* U+006E "n" */
    0xbd, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x82,

    /* U+006F "o" */
    0x7d, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xe3, 0x7c,

    /* U+0070 "p" */
    0xbd, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xe3, 0xbd,
    0x2, 0x4, 0x0,

    /* U+0071 "q" */
    0x7b, 0x8e, 0xc, 0x18, 0x30, 0x60, 0xe3, 0x7a,
    0x4, 0x8, 0x10,

    /* U+0072 "r" */
    0xbd, 0x8e, 0xc, 0x18, 0x10, 0x20, 0x40, 0x80,

    /* U+0073 "s" */
    0x7d, 0xe, 0x7, 0x7, 0xc0, 0xc0, 0xc1, 0x7c,

    /* U+0074 "t" */
    0x10, 0x20, 0x47, 0xf1, 0x2, 0x4, 0x8, 0x10,
    0x20, 0x40, 0x70,

    /* U+0075 "u" */
    0x83, 0x6, 0xc, 0x18, 0x30, 0x60, 0xe3, 0x7c,

    /* U+0076 "v" */
    0x83, 0xc2, 0x46, 0x46, 0x64, 0x2c, 0x2c, 0x38,
    0x18,

    /* U+0077 "w" */
    0x99, 0x99, 0x99, 0xd9, 0xdb, 0x5a, 0x66, 0x66,
    0x66,

    /* U+0078 "x" */
    0xc2, 0x66, 0x2c, 0x38, 0x18, 0x38, 0x6c, 0x46,
    0xc2,

    /* U+0079 "y" */
    0xc3, 0xc2, 0x46, 0x66, 0x24, 0x2c, 0x38, 0x18,
    0x18, 0x10, 0x30, 0x30,

    /* U+007A "z" */
    0xfe, 0xc, 0x30, 0xc1, 0x6, 0x18, 0x60, 0xfe,

    /* U+007B "{" */
    0x7, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0xf0,
    0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x7,

    /* U+007C "|" */
    0xff, 0xfe,

    /* U+007D "}" */
    0xc0, 0x40, 0x81, 0x2, 0x4, 0x8, 0xf, 0x20,
    0x40, 0x81, 0x2, 0x4, 0x30, 0x0,

    /* U+007E "~" */
    0x63, 0x26, 0x70};

/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 154, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 154, .box_w = 2, .box_h = 12, .ofs_x = 4, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 154, .box_w = 5, .box_h = 5, .ofs_x = 2, .ofs_y = 7},
    {.bitmap_index = 8, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 22, .adv_w = 154, .box_w = 7, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 36, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 50, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 62, .adv_w = 154, .box_w = 1, .box_h = 5, .ofs_x = 4, .ofs_y = 7},
    {.bitmap_index = 63, .adv_w = 154, .box_w = 5, .box_h = 16, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 73, .adv_w = 154, .box_w = 5, .box_h = 16, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 83, .adv_w = 154, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 92, .adv_w = 154, .box_w = 8, .box_h = 7, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 99, .adv_w = 154, .box_w = 3, .box_h = 5, .ofs_x = 3, .ofs_y = -3},
    {.bitmap_index = 101, .adv_w = 154, .box_w = 6, .box_h = 1, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 102, .adv_w = 154, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 104, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 118, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 140, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 151, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 162, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 171, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 182, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 204, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 215, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 226, .adv_w = 154, .box_w = 3, .box_h = 9, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 154, .box_w = 4, .box_h = 12, .ofs_x = 2, .ofs_y = -3},
    {.bitmap_index = 236, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 244, .adv_w = 154, .box_w = 7, .box_h = 5, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 249, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 1},
    {.bitmap_index = 257, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 266, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 281, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 293, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 304, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 315, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 326, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 335, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 366, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 377, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 388, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 399, .adv_w = 154, .box_w = 6, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 408, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 419, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 441, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 452, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 466, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 477, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 488, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 500, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 511, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 523, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 537, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 549, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 563, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 574, .adv_w = 154, .box_w = 3, .box_h = 15, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 580, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 594, .adv_w = 154, .box_w = 3, .box_h = 15, .ofs_x = 3, .ofs_y = -2},
    {.bitmap_index = 600, .adv_w = 154, .box_w = 7, .box_h = 7, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 607, .adv_w = 154, .box_w = 8, .box_h = 1, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 608, .adv_w = 154, .box_w = 3, .box_h = 3, .ofs_x = 3, .ofs_y = 10},
    {.bitmap_index = 610, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 618, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 629, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 637, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 648, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 656, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 668, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 679, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 690, .adv_w = 154, .box_w = 7, .box_h = 13, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 702, .adv_w = 154, .box_w = 6, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 714, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 725, .adv_w = 154, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 739, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 747, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 755, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 763, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 774, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 785, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 793, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 801, .adv_w = 154, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 812, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 820, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 829, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 838, .adv_w = 154, .box_w = 8, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 847, .adv_w = 154, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 859, .adv_w = 154, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 867, .adv_w = 154, .box_w = 8, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 882, .adv_w = 154, .box_w = 1, .box_h = 15, .ofs_x = 4, .ofs_y = -2},
    {.bitmap_index = 884, .adv_w = 154, .box_w = 7, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 898, .adv_w = 154, .box_w = 7, .box_h = 3, .ofs_x = 1, .ofs_y = 4}};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
    {
        {.range_start = 32, .range_length = 95, .glyph_id_start = 1, .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY}};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};

/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t font_jetBrainsMono = {
#else
lv_font_t font_jetBrainsMono = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt, /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt, /*Function pointer to get glyph's bitmap*/
    .line_height = 17,                              /*The maximum line height required by the font*/
    .base_line = 3,                                 /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};

#endif /*#if FONT_JETBRAINSMONO*/
