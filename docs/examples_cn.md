```
.
├── BasicUnit                       #基础硬件单元目录
│   ├── AXP20x_ADC                  #电源管理ADC示例
│   ├── AXP20x_ChargeCurrent        #电源管理充电管理示例
│   ├── AXP20x_IRQ                  #电源管理中断示例
│   ├── BMA423_Accel                #三轴加速度计基本示例
│   ├── BMA423_Direction            #三轴加速度计获取屏幕方向
│   ├── BMA423_Feature              #三轴加速度计特性
│   ├── BMA423_StepCount            #三轴加速度计内置步数获取示例
│   ├── BluetoothAudio              #通过蓝牙播放音频
│   ├── BluetoothAudioWeb           #通过蓝牙播放音频,附带网页控制
│   ├── DRV2605_Basic               #DRV2605 Vibration Basic
│   ├── DRV2605_Complex             #DRV2605 Vibration Complex
│   ├── DRV2605_Realtime            #DRV2605 Vibration Realtime
│   ├── DrawSD_BMP                  #画BMP格式图片到屏幕(只适用带SD卡槽主板)
│   ├── Motor                       #基本震动驱动示例
│   ├── PlayMP3FromPROGMEM          #从Flash中获取音频数据播放MP3  
│   ├── PlayMP3FromSDToDAC          #从SD卡中获取音频数据播放MP3
│   ├── PlayMP3FromSPIFFS           #从SPIFFS中获取音频数据播放MP3
│   ├── RTC                         #基本RTC设置和闹钟示例
│   ├── SDCard                      #基本SD卡操作示例
│   ├── SetTimeFromBLE              #从BLE中设置RTC时间
│   ├── TimeSynchronization         #WiFi时间同步
│   ├── TouchMultitask              #多任务触摸读取
│   ├── TouchPad                    #基本触摸示例
│   ├── TwatcV2Special              #T-Watch-V2特有功能
│   │   ├── DRV2605_Basic           #DRV2605 触觉反馈示例
│   │   ├── DRV2605_Complex         #DRV2605 触觉反馈示例
│   │   ├── DRV2605_Realtime        #DRV2605 触觉反馈示例
│   │   └── GPSDisplay              #GPS定位示例
│   ├── TwatcV3Special              #T-Watch-V3特有功能
│   │   └── Microphone              #麦克风测试
│   ├── UserButton                  #基本用户按键示例
│   ├── WakeupFormPEKKey            #使用电源管理PEK按键唤醒ESP32
│   ├── WakeupFormSensor            #使用三轴加速度计唤醒ESP32
│   ├── WakeupFormTimer             #使用内置定时器唤醒ESP32
│   └── WakeupFormTouchScreen       #从触摸屏唤醒ESP32
├── ClientProject                   #客户的示例目录
│   └── SimpleFramework             #使用TFT_eSPI构成的框架
├── ExternTFTLibrary                #使用PlatformIO,外部TFT_eSPI与TTGO_TWatch_Library搭配使用示例
│   └── src
├── LVGL                            #LVGL图形框架演示目录
│   ├── AnalogRead                  #获取模拟数据在显示屏中绘制曲线
│   ├── BatmanDial                  #简单的蝙蝠侠样式时钟
│   ├── ChargingAnimation           #lvgl & fs 充电动画演示
│   ├── IRremote                    #红外遥控示例
│   ├── LilyGoGui                   #简单的手表界面
│   ├── Lvgl_Base                   #LVGL基础入门
│   ├── Lvgl_Button                 #LVGL按键的示例
│   ├── SimplePlayer                #LVGL简易播放器示例
│   ├── SimpleWatch                 #LVGL简易手表示例
│   └── lvgl_fs                     #从文件系统中读取png图片显示在lvgl中
├── LilyPi                          #此目录只适用LilyPi，不适用其他
│   ├── Epaper_GDEW0371W7           #3.71`带触摸墨水屏驱动示例
│   ├── IRRemote                    #使用外部模块红外遥控示例
│   └── lv_demo_widgets             #lvgl 演示示例
├── NES                             #NES游戏模拟器
├── Retention                       #示例保留目录
│   ├── Air530Allfunction
│   └── Air530Display
├── Sensor                          #传感器目录
│   ├── Fingerprint                 #指纹传感器示例
│   ├── MAX30208                    #MAX30208 温度传感器
│   ├── Rotary                      #旋转编码器示例
│   └── VEMl6075                    #紫外线传感器示例
├── Shield                          #此目录只适用TWATCH-2019，不适用TWATCH2020
│   ├── DW1000Ranging_ANCHOR
│   ├── DW1000Ranging_TAG
│   ├── DW1000_BasicReceiver
│   ├── DW1000_BasicSender
│   ├── DW1000_RangingAnchor
│   ├── DW1000_RangingTag
│   ├── AlarmClock                  #RTC闹钟示例
│   ├── BBQKeyboard                 #BBQ键盘示例
│   ├── DRV8833                     #电机示例
│   ├── GameControl                 #游戏手柄示例
│   ├── HeartRate                   #心率传感器示例
│   ├── INMP441_Mic                 #麦克风示例
│   ├── LoRa_SX127x                 #LORA示例
│   ├── M6_GPS                      #Ubolx GPS示例
│   ├── NFC_Lock                    #NFC 示例
│   ├── S7xG_GPS                    #S76/78G GPS 示例
│   ├── S7xG_LoRa                   #S76/78G LORA 示例
│   ├── SIM800L                     #SIM800L 2G通信模块示例
│   ├── SIM868                      #SIM868  2G通信定位模块示例
│   └── SPM1423HM4H_Mic             #麦克风示例
├── T-Block                         #此目录只适用TBLOCK，不适用其他
│   ├── CapTouch                    #MPR121 电容触摸传感器示例
│   ├── Epaper                      #1.54` 墨水屏，不带背光和触摸
│   ├── Epaper_BL                   #1.54`带背光墨水屏驱动示例
│   ├── Epaper_Badge                #1.54`电子纸 电子徽章
│   ├── Epaper_GDEW0371W7           #3.71`带触摸墨水屏驱动示例
│   ├── Epaper_GDEW0371W7_BTN       #3.71`带触摸墨水屏带按键驱动示例
│   ├── Epaper_TP                   #1.54`带触摸墨水屏驱动示例
│   ├── LEDMatrix                   #LED点阵示例
│   ├── MPU6050_accel_pitch_roll    #MPU6050 加速度示例
│   ├── MPU6050_accel_simple        #MPU6050 加速度示例
│   ├── MPU6050_gyro_pitch_roll_yaw #MPU6050 陀螺仪示例
│   ├── MPU6050_gyro_simple         #MPU6050 陀螺仪示例
│   ├── MPU6050_temperature         #MPU6050 内置温度传感器示例
│   └── TouchScreen                 #ST7796S / ILI9488 显示屏模块示例
├── TFT_eSPI                        
│   ├── All_Free_Fonts_Demo         
│   ├── Arduino_Life                
│   ├── ESPFace                     
│   ├── FlappyBird                  
│   ├── IOTA_Price                  
│   ├── Pong_v3
│   ├── RLE_Font_test
│   ├── TFT_Char_times
│   ├── TFT_Clock
│   ├── TFT_Clock_Digital
│   ├── TFT_Ellipse
│   ├── TFT_Meter_5
│   ├── TFT_graphicstest_PDQ3
│   ├── TFT_graphicstest_small
│   ├── UTFT_demo_fast
│   └── fillScreen
├── T_Bao                           # T_Bao 小车示例
├── T_Bao2                          # T_Bao 小车示例
├── T_Quick                         # T_Quick 小车示例
└── U8g2_for_TFT_eSPI               # U8g2示例
│   ├── Shennong
│   └──  Unicode
├── UnitTest                        #硬件单元测试目录
│   └── ScreenRotation              #屏幕选择与触摸
```
