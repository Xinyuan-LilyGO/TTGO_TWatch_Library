/**
 * @file lv_log.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_log.h"
#if LV_USE_LOG

#include <stdarg.h>
#include <string.h>
#include "lv_printf.h"
#include "../hal/lv_hal_tick.h"

#if LV_LOG_PRINTF
    #include <stdio.h>
#endif

/*********************
 *      DEFINES
 *********************/

#if LV_LOG_USE_TIMESTAMP
    #define LOG_TIMESTAMP_FMT  "\t(%" LV_PRId32 ".%03" LV_PRId32 ", +%" LV_PRId32 ")\t"
    #define LOG_TIMESTAMP_EXPR t / 1000, t % 1000, t - last_log_time,
#else
    #define LOG_TIMESTAMP_FMT
    #define LOG_TIMESTAMP_EXPR
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_log_print_g_cb_t custom_print_cb;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Register custom print/write function to call when a log is added.
 * It can format its "File path", "Line number" and "Description" as required
 * and send the formatted log message to a console or serial port.
 * @param print_cb a function pointer to print a log
 */
void lv_log_register_print_cb(lv_log_print_g_cb_t print_cb)
{
    custom_print_cb = print_cb;
}

/**
 * Add a log
 * @param level the level of log. (From `lv_log_level_t` enum)
 * @param file name of the file when the log added
 * @param line line number in the source code where the log added
 * @param func name of the function when the log added
 * @param format printf-like format string
 * @param ... parameters for `format`
 */
void _lv_log_add(lv_log_level_t level, const char * file, int line, const char * func, const char * format, ...)
{
    if(level >= _LV_LOG_LEVEL_NUM) return; /*Invalid level*/

#if LV_LOG_USE_TIMESTAMP
    static uint32_t last_log_time = 0;
#endif

    if(level >= LV_LOG_LEVEL) {
        va_list args;
        va_start(args, format);

        /*Use only the file name not the path*/
        size_t p;
        for(p = strlen(file); p > 0; p--) {
            if(file[p] == '/' || file[p] == '\\') {
                p++;    /*Skip the slash*/
                break;
            }
        }
#if LV_LOG_USE_TIMESTAMP
        uint32_t t = lv_tick_get();
#endif
        static const char * lvl_prefix[] = {"Trace", "Info", "Warn", "Error", "User"};

#if LV_LOG_PRINTF
        printf("[%s]" LOG_TIMESTAMP_FMT " %s: ",
               lvl_prefix[level], LOG_TIMESTAMP_EXPR func);
        vprintf(format, args);
        printf(" \t(in %s line #%d)\n", &file[p], line);
#else
        if(custom_print_cb) {
            char buf[512];
            char msg[256];
            lv_vsnprintf(msg, sizeof(msg), format, args);
            lv_snprintf(buf, sizeof(buf), "[%s]" LOG_TIMESTAMP_FMT " %s: %s \t(in %s line #%d)\n",
                        lvl_prefix[level], LOG_TIMESTAMP_EXPR func, msg, &file[p], line);
            custom_print_cb(buf);
        }
#endif

#if LV_LOG_USE_TIMESTAMP
        last_log_time = t;
#endif
        va_end(args);
    }
}

void lv_log(const char * format, ...)
{
    if(LV_LOG_LEVEL >= LV_LOG_LEVEL_NONE) return; /* disable log */

    va_list args;
    va_start(args, format);

#if LV_LOG_PRINTF
    vprintf(format, args);
#else
    if(custom_print_cb) {
        char buf[512];
        lv_vsnprintf(buf, sizeof(buf), format, args);
        custom_print_cb(buf);
    }
#endif

    va_end(args);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*LV_USE_LOG*/
