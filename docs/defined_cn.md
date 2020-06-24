## 套件类型

- **LILYGO_WATCH_2019_WITH_TOUCH**
    - 带触摸屏幕,可以扩展T-Watch系列的底板
- **LILYGO_WATCH_2019_NO_TOUCH**
    - 不带触摸屏幕,可以扩展T-Watch系列的底板
- **LILYGO_WATCH_2020_V1**
    - 带触摸屏幕,它不可以扩展T-Watch系列的底板
- **LILYGO_WATCH_BLOCK**
    - 没有屏幕和触摸屏功能,它没有BMA423

## 功能定义
- **LILYGO_WATCH_LVGL**
    - 开启LVGL功能，它需要有显示器的支持
- **LILYGO_WATCH_HAS_BLACKLIGHT**
    - 开启背光功能
- **LILYGO_WATCH_HAS_NFC**
    - 开启NFC底板功能,它适配T-Watch NFC 的底板
- **LILYGO_WATCH_HAS_MOTOR**
    - 开启震动功能,它适配T-Watch MOTOR 的底板
- **LILYGO_WATCH_HAS_BUZZER**
    - 开启蜂鸣器功能,它适配T-Watch 带有BUZZER的底板
- **LILYGO_WATCH_HAS_SDCARD**
    - 开启SD卡功能,它适配T-Watch 带有SD 功能的底板
- **LILYGO_WATCH_HAS_S76_S78G**
    - 开启S76/S78G功能,它适配T-Watch 带有S76/S78G功能的底板
- **LILYGO_EINK_TOUCHSCREEN**
    - 开启电子墨水屏触摸功能,它仅仅适用于T-Block触摸墨水屏面板
- **LILYGO_WATCH_HAS_EINK**
    - 开启电子墨水屏,它仅仅适用于T-Block墨水屏面板
- **LILYGO_WATCH_HAS_GAMEPAD**
    - 开启游戏手柄功能,它仅仅适用于游戏手柄扩展
- **LILYGO_WATCH_HAS_MPR121**
    - 开启MPR121触摸芯片功能,它仅仅适用于MPR121底板
- **LILYGO_WATCH_HAS_LORA**
    - 开启Lora模块功能,它适配T-Watch 带有Lora功能的底板
- **LILYGO_WATCH_HAS_GPS**
    - 开启GPS模块功能,它适配T-Watch 带有GPS功能的底板
- **LILYGO_WATCH_HAS_SIM800L**
    - 开启SIM800功能,它适配T-Watch 带有SIM800模块底板


**以上定义,必须包含在 #include <LilyGoWatch.h> 上方,否则它不起作用**
