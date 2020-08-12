/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Simple Accelerometer Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski

    T-Block is adapted by Lewis He July 27,2020
*/

#include "config.h"

TTGOClass *watch;
MPU6050 *mpu;

void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();

    mpu = watch->mpu;
}

void loop()
{
    float temp = mpu->readTemperature();
    Serial.print(" Temp = ");
    Serial.print(temp);
    Serial.println(" *C");
    delay(500);
}
