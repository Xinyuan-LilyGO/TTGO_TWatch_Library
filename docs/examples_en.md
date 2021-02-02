```
.
├── BasicUnit                       #Basic hardware unit catalog
│   ├── AXP20x_ADC                  #Power Management ADC Example
│   ├── AXP20x_ChargeCurrent        #Power managementCharging management example
│   ├── AXP20x_IRQ                  #Power management interrupt example
│   ├── BMA423_Accel                #Basic example of three-axis accelerometer
│   ├── BMA423_Direction            #Three-axis accelerometer to get screen orientation
│   ├── BMA423_Feature              #Three-axis accelerometer features
│   ├── BMA423_StepCount            #Three-axis accelerometer built-in steps acquisition example
│   ├── BluetoothAudio              #Bluetooth playback
│   ├── BluetoothAudioWeb           #Bluetooth playback, with web control
│   ├── DrawSD_BMP                  #Draw BMP format pictures to the screen (only for motherboards with SD card slot)
│   ├── Motor                       #Basic Vibration Drive
│   ├── PlayMP3FromPROGMEM          #Get audio data from Flash and play MP3      
│   ├── PlayMP3FromSDToDAC          #Get audio data from SD card to play MP3
│   ├── PlayMP3FromSPIFFS           #Get audio data from SPIFFS and play MP3
│   ├── RTC                         #Basic RC settings and alarm clock examples
│   ├── SDCard                      #Basic SD card operation example
│   ├── SetTimeFromBLE              #Set RTC time from BLE
│   ├── TimeSynchronization         #Time synchronization
│   ├── TouchMultitask              #Touch multitasking
│   ├── TouchPad                    #Basic touch example
│   ├── UserButton                  #Basic user button example
│   ├── TwatcV2Special              #T-Watch-2020-V2 unique features 
│   │   ├── DRV2605_Basic           #DRV2605 haptic feedback example 
│   │   ├── DRV2605_Complex         #DRV2605 haptic feedback example 
│   │   ├── DRV2605_Realtime        #DRV2605 haptic feedback example 
│   │   └── GPSDisplay              #GPS positioning example 
│   ├── TwatcV3Special              #T-Watch-2020-V3 unique features 
│   │   └── Microphone              #Microphone test 
│   ├── WakeupFormPEKKey            #Use the power management PEK button to wake up ESP32
│   ├── WakeupFormSensor            #Use the three-axis accelerometer to wake up the ESP32
│   ├── WakeupFormTimer             #Use built-in timer to wake up ESP32
│   └── WakeupFormTouchScreen       #Wake up from the touch screen
├── ClientProject                   #Customer's example directory
│   └── SimpleFramework             #Use TFT_eSPI frame
├── ExternTFTLibrary                #Use PlatfromIO, an example of using external TFT_eSPI with TTGO_TWatch_Library
│   └── src
├── LVGL                            #LVGLGraphic Framework Demo Directory
│   ├── AnalogRead                  #Get simulation data and draw a curve in the display
│   ├── BatmanDial                  #Simple Batman style clock
│   ├── ChargingAnimation           #Charging animation demonstration
│   ├── IRremote                    #Infrared remote control example
│   ├── LilyGoGui                   #Simple watch interface
│   ├── Lvgl_Base                   #LVGL Getting Started
│   ├── Lvgl_Button                 #LVGLKey Examples
│   ├── SimplePlayer                #LVGLSimple player example
│   ├── SimpleWatch                 #LVGLSimple watch example
│   └── lvgl_fs                     #Read the png from fs and display it in lvgl 
├── LilyPi                          #This directory is only applicable to LilyPi, not applicable to others 
│   ├── Epaper_GDEW0371W7           #3.71`Drive example with touch ink screen
│   ├── IRRemote                    #Example of using external module infrared remote control
│   └── lv_demo_widgets             #lvgl example
├── NES                             #NES emulator
├── Retention                       #示例保留目录
│   ├── Air530Allfunction
│   └── Air530Display
├── Sensor                          
│   ├── Fingerprint                 #Fingerprint sensor example
│   ├── MAX30208                    #MAX30208 temperature sensor
│   ├── Rotary                      #Rotary encoder example
│   └── VEMl6075                    #UV sensor example
├── Shield
│   ├── DW1000Ranging_ANCHOR
│   ├── DW1000Ranging_TAG
│   ├── DW1000_BasicReceiver
│   ├── DW1000_BasicSender
│   ├── DW1000_RangingAnchor
│   ├── DW1000_RangingTag
│   ├── AlarmClock                  #RTC Alarm clock example
│   ├── BBQKeyboard                 #BBQ keyboard example
│   ├── DRV8833                     #Motor example
│   ├── GameControl                 #Gamepad example
│   ├── HeartRate                   #Heart rate sensor example
│   ├── INMP441_Mic                 #Microphone example
│   ├── LoRa_SX127x                 #LoRa Example
│   ├── M6_GPS                      #Ubolx GPS example
│   ├── NFC_Lock                    #NFC example
│   ├── S7xG_GPS                    #S76/78G GPS example
│   ├── S7xG_LoRa                   #S76/78G LoRa example
│   ├── SIM800L                     #SIM800L 2G communication module example
│   ├── SIM868                      #SIM868  2G communication positioning module example
│   └── SPM1423HM4H_Mic             #Microphone example
├── T-Block                         #This directory is only applicable to TBLOCK, not applicable to others
│   ├── CapTouch                    #MPR121 Capacitive touch sensor example
│   ├── Epaper                      #1.54`Ink Single electronic paper, without backlight and touch
│   ├── Epaper_BL                   #1.54`Ink screen driver example with backlight    
│   ├── Epaper_Badge                #1.54`Ink Electronic badge
│   ├── Epaper_GDEW0371W7           #3.71`Drive example with touch ink screen    
│   ├── Epaper_GDEW0371W7_BTN       #3.71`Drive example with touch ink screen and button
│   ├── Epaper_TP                   #1.54`Drive example with touch ink screen
│   ├── LEDMatrix                   #LEDdot matrix example
│   ├── MPU6050_accel_pitch_roll    #MPU6050 Acceleration example
│   ├── MPU6050_accel_simple        #MPU6050 Acceleration example
│   ├── MPU6050_gyro_pitch_roll_yaw #MPU6050 Gyroscope example
│   ├── MPU6050_gyro_simple         #MPU6050 Gyroscope example
│   ├── MPU6050_temperature         #MPU6050 Example of built-in temperature sensor
│   └── TouchScreen                 #ST7796S / ILI9481 Display module example
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
├── T_Bao                           #Expansion trolley
├── T_Bao2                          #Expansion trolley
├── T_Quick                         #Expansion trolley
└── U8g2_for_TFT_eSPI               #U8g2 examples
│   ├── Shennong
│   └──  Unicode
├── UnitTest                        #Hardware test directory
│   └── ScreenRotation              
```
