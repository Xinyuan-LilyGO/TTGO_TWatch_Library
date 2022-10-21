/**
 * @file lv_templ.h
 *
 */

#ifndef LV_TINY_TTF_H
#define LV_TINY_TTF_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"

#if LV_USE_TINY_TTF

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
#if LV_TINY_TTF_FILE_SUPPORT !=0
// create a font from the specified file or path with the specified line height.
lv_font_t * lv_tiny_ttf_create_file(const char * path, lv_coord_t line_height);

// create a font from the specified file or path with the specified line height with the specified cache size.
lv_font_t * lv_tiny_ttf_create_file_ex(const char * path, lv_coord_t line_height, size_t cache_size);
#endif

// create a font from the specified data pointer with the specified line height.
lv_font_t * lv_tiny_ttf_create_data(const void * data, size_t data_size, lv_coord_t line_height);

// create a font from the specified data pointer with the specified line height and the specified cache size.
lv_font_t * lv_tiny_ttf_create_data_ex(const void * data, size_t data_size, lv_coord_t line_height, size_t cache_size);

// set the size of the font to a new line_height
void lv_tiny_ttf_set_size(lv_font_t * font, lv_coord_t line_height);

// destroy a font previously created with lv_tiny_ttf_create_xxxx()
void lv_tiny_ttf_destroy(lv_font_t * font);

/**********************
 *      MACROS
 **********************/

#endif /*LV_USE_TINY_TTF*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_TINY_TTF_H*/
