// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH     // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH       // To use T-Watch2019 Not touchscreen , please uncomment this line
// #define LILYGO_WATCH_BLOCK               // To use T-Watch Block , please uncomment this line
// #define LILYGO_WATCH_2020_V1             // To use T-Watch2020 , please uncomment this line



// Except T-Watch2020, other versions need to be selected according to the actual situation
#ifndef LILYGO_WATCH_2020_V1

// T-Watch comes with the default backplane, it uses internal DAC
#define STANDARD_BACKPLANE

// Such as MAX98357A, PCM5102 external DAC backplane
// #define EXTERNAL_DAC_BACKPLANE

#else
// T-Watch2020 uses external DAC
#undef STANDARD_BACKPLANE
#define EXTERNAL_DAC_BACKPLANE

#endif


#include <LilyGoWatch.h>