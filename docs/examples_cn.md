```
examples/
├── BasicUnit                       #基础硬件单元目录
│   ├── AXP20x_ADC                  #电源管理ADC示例
│   ├── AXP20x_ChargeCurrent        #电源管理充电管理示例
│   ├── AXP20x_IRQ                  #电源管理中断示例
│   ├── BMA423_Accel                #三轴加速度计基本示例
│   ├── BMA423_Direction            #三轴加速度计获取屏幕方向
│   ├── BMA423_Feature              #三轴加速度计特性
│   ├── BMA423_StepCount            #三轴加速度计内置步数获取示例
│   ├── DrawSD_BMP                  #画BMP格式图片到屏幕(只适用带SD卡槽主板)
│   ├── Motor                       #基本震动驱动
│   ├── PlayMP3FromPROGMEM          #从Flash中获取音频数据播放MP3
│   ├── PlayMP3FromSDToDAC          #从SD卡中获取音频数据播放MP3
│   ├── PlayMP3FromSPIFFS           #从SPIFFS中获取音频数据播放MP3
│   ├── RTC                         #基本RTC设置和闹钟示例
│   ├── SDCard                      #基本SD卡操作示例
│   ├── SetTimeFromBLE              #从BLE中设置RTC时间
│   ├── TouchPad                    #基本触摸示例
│   ├── UserButton                  #基本用户按键示例
│   ├── WakeupFormPEKKey            #使用电源管理PEK按键唤醒ESP32
│   ├── WakeupFormSensor            #使用三轴加速度计唤醒ESP32
│   ├── WakeupFormTimer             #使用内置定时器唤醒ESP32
│   ├── BluetoothAudio              #通过蓝牙播放音频
│   └── BluetoothAudioWeb           #通过蓝牙播放音频,附带网页控制
├── ClientProject                   #客户的示例目录
│   └── SimpleFramework             #使用TFT_eSPI构成的框架
├── ExternTFTLibrary                #使用PlatfromIO,外部TFT_eSPI与TTGO_TWatch_Library搭配使用示例
│   └── src                     
├── LVGL                            #LVGL图形框架演示目录
│   ├── AnalogRead                  #获取模拟数据在显示屏中绘制曲线
│   ├── BatmanDial                  #简单的蝙蝠侠样式时钟
│   ├── IRremote                    #红外遥控示例
│   ├── LilyGoGui                   #简单的手表界面
│   ├── Lvgl_Base                   #LVGL基础入门
│   ├── Lvgl_Button                 #LVGL按键的示例
│   ├── SimplePlayer                #LVGL简易播放器示例
│   ├── SimpleWatch                 #LVGL简易手表示例
│   └── TQuick                      #LVGL简易电机驱动
├── Sensor                          #传感器目录
│   ├── Fingerprint                 #指纹传感器示例
│   ├── Rotary                      #旋转编码器示例
│   └── VEMl6075                    #紫外线传感器示例
├── Shield                          #此目录只适用TWATCH-2019，不适用TWATCH2020
│   ├── AlarmClock                  #RTC闹钟示例
│   ├── BBQKeyboard                 #BBQ键盘示例
│   ├── DRV8833                     #电机示例
│   ├── GameControl                 #游戏手柄示例
│   ├── HeartRate                   #心率传感器示例
│   ├── INMP441_Mic                 #麦克风示例
│   ├── LoRa_SX127x                 #LORA示例
│   ├── M6_GPS                      #ubolx GPS示例
│   ├── NFC_Lock                    #NFC 示例
│   ├── S7xG_GPS                    #S76/78G GPS 示例
│   ├── S7xG_LoRa                   #S76/78G LORA 示例
│   ├── SIM800L                     #SIM800L 2G通信模块示例
│   ├── SIM868                      #SIM868  2G通信定位模块示例
│   └── SPM1423HM4H_Mic             #麦克风示例
├── T-Block                         #此目录只适用TBLOCK，不适用其他
│   ├── CapTouch                    #MPR121 电容触摸传感器示例
│   ├── Epaper_BL                   #1.54`带背光墨水屏驱动示例
│   ├── Epaper_TP                   #1.54`带触摸墨水屏驱动示例
│   ├── Epaper_GDEW0371W7           #3.71`带触摸墨水屏驱动示例
│   ├── LEDMatrix                   #LED点阵示例
│   ├── MPU6050_accel_pitch_roll    #MPU6050 加速度示例
│   ├── MPU6050_accel_simple        #MPU6050 加速度示例
│   ├── MPU6050_gyro_pitch_roll_yaw #MPU6050 陀螺仪示例
│   ├── MPU6050_gyro_simple         #MPU6050 陀螺仪示例
│   ├── MPU6050_temperature         #MPU6050 内置温度传感器示例
│   └── TouchScreen                 #ST7796S / ILI9488 显示屏模块示例
├── LilyPi                          #此目录只适用LilyPi，不适用其他
│   ├── Epaper_GDEW0371W7           #3.71`带触摸墨水屏驱动示例
│   ├── IRRemote                    #使用外部模块红外遥控示例
│   └── lv_demo_widgets             #lvgl 演示示例
└── TFT_eSPI                        #TFT_eSPI 显示示例
    ├── All_Free_Fonts_Demo
    ├── Arduino_Life
    ├── FlappyBird
    ├── IOTA_Price
    ├── Pong_v3
    ├── RLE_Font_test
    ├── TFT_Char_times
    ├── TFT_Clock
    ├── TFT_Clock_Digital
    ├── TFT_Ellipse
    ├── TFT_Meter_5
    ├── TFT_graphicstest_PDQ3
    ├── TFT_graphicstest_small
    ├── UTFT_demo_fast
    └── fillScreen
```