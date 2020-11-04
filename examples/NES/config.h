// => Hardware select
// #define LILYGO_LILYPI_V1                  //To use LilyPi with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_WITH_TOUCH     // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH       // To use T-Watch2019 Not touchscreen , please uncomment this line
// #define LILYGO_WATCH_2020_V1             //To use T-Watch2020, please uncomment this line
// #define LILYGO_WATCH_2020_V2             //To use T-Watch2020, please uncomment this line

// Controller select
#define HW_CONTROLLER_DABBLE_APP         //Need to install Dabble App on your phone,download link : https://play.google.com/store/apps/details?id=io.dabbleapp&hl=en_US&gl=US
// #define HW_CONTROLLER_GAMEPAD            //Need an external control handle, can only be used in 2019 Twatch




#ifdef LILYGO_LILYPI_V1
#define LILYGO_BLOCK_ST7796S_MODULE
#endif

#ifdef HW_CONTROLLER_GAMEPAD
#define HW_CONTROLLER_GPIO
#define HW_CONTROLLER_GPIO_ANALOG_JOYSTICK
#define HW_CONTROLLER_GPIO_UP_DOWN              34
#define HW_CONTROLLER_GPIO_LEFT_RIGHT           33
#define HW_CONTROLLER_GPIO_SELECT               15
#define HW_CONTROLLER_GPIO_START                36
#define HW_CONTROLLER_GPIO_A                    13
#define HW_CONTROLLER_GPIO_B                    25
#define HW_CONTROLLER_GPIO_X                    14
#define HW_CONTROLLER_GPIO_Y                    26
#endif

#include <LilyGoWatch.h>

