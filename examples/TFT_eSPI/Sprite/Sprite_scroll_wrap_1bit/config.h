// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH         // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH           // To use T-Watch2019 Not touchscreen , please uncomment this line
// #define LILYGO_WATCH_2020_V1                 // To use T-Watch2020 V1, please uncomment this line
// #define LILYGO_WATCH_2020_V2                 // To use T-Watch2020 V2, please uncomment this line
// #define LILYGO_WATCH_2020_V3                 // To use T-Watch2020 V3, please uncomment this line

// #define LILYGO_LILYPI_V1                     //LILYPI / TBLOCK requires an external display module
// #define LILYGO_WATCH_BLOCK                   //LILYPI / TBLOCK requires an external display module



#if defined(LILYGO_LILYPI_V1) || defined(LILYGO_WATCH_BLOCK)
// #define LILYGO_BLOCK_ST7796S_MODULE          //Use ST7796S
// #define LILYGO_BLOCK_ILI9481_MODULE          //Use ILI9841
// #define LILYGO_GC9A01A_MODULE                   //Use GC9A01A
#endif

#include <LilyGoWatch.h>

