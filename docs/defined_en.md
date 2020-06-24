## Kit Type

- **LILYGO_WATCH_2019_WITH_TOUCH**
    -With touch screen, you can expand the bottom plate of the T-Watch series
- **LILYGO_WATCH_2019_NO_TOUCH**
    -Without the touch screen, the bottom plate of the T-Watch series can be expanded
- **LILYGO_WATCH_2020_V1**
    -With touch screen, it can not expand the bottom plate of T-Watch series
- **LILYGO_WATCH_BLOCK**
    -No screen and touch screen function, it does not have BMA423

## Function definition
- **LILYGO_WATCH_LVGL**
    -Turn on the LVGL function, it needs to be supported by the display
- **LILYGO_WATCH_HAS_BLACKLIGHT**
    -Turn on the backlight function
- **LILYGO_WATCH_HAS_NFC**
    -Turn on the NFC backplane function, it adapts to the T-Watch NFC backplane
- **LILYGO_WATCH_HAS_MOTOR**
    -Turn on the vibration function, it adapts to the bottom plate of T-Watch MOTOR
- **LILYGO_WATCH_HAS_BUZZER**
    -Turn on the buzzer function, it adapts to the T-Watch baseboard with BUZZER
- **LILYGO_WATCH_HAS_SDCARD**
    -Turn on the SD card function, it adapts to the T-Watch baseboard with SD function
- **LILYGO_WATCH_HAS_S76_S78G**
    -Turn on the S76/S78G function, it adapts to the T-Watch baseboard with S76/S78G function
- **LILYGO_EINK_TOUCHSCREEN**
    -Turn on the electronic ink screen touch function, it is only applicable to the T-Block touch ink screen panel
- **LILYGO_WATCH_HAS_EINK**
    -Turn on the electronic ink screen, it only applies to the T-Block ink screen panel
- **LILYGO_WATCH_HAS_GAMEPAD**
    -Turn on the gamepad function, it only applies to gamepad expansion
- **LILYGO_WATCH_HAS_MPR121**
    -Turn on the MPR121 touch chip function, it is only applicable to the MPR121 backplane
- **LILYGO_WATCH_HAS_LORA**
    -Turn on the Lora module function, it adapts to the T-Watch baseboard with Lora function
- **LILYGO_WATCH_HAS_GPS**
    -Turn on the GPS module function, it adapts to the T-Watch baseboard with GPS function
- **LILYGO_WATCH_HAS_SIM800L**
    -Turn on SIM800 function, it adapts to T-Watch with SIM800 module backplane


**The above definition must be included above #include <LilyGoWatch.h>, otherwise it will not work**