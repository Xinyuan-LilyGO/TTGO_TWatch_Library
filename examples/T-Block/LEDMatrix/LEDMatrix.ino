
#pragma mark - Depend MAX7219LedMatrix library
/*
cd ~/Arduino/libraries
git clone https://github.com/Xinyuan-LilyGO/MAX7219LedMatrix
*/


#include <SPI.h>
#include <LedMatrix.h>

#define NUMBER_OF_DEVICES 8

//! T-Block led Matrix
#define MATRIX_MSIO 23  //No use
#define MATRIX_MOSI 18
#define MATRIX_CS   19
#define MATRIX_SCLK 14

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, MATRIX_CS);

void setup()
{
    SPI.begin(MATRIX_SCLK, MATRIX_MSIO, MATRIX_MOSI, MATRIX_CS);
    ledMatrix.init();
    ledMatrix.setText("Hello world");
}

void loop()
{
    ledMatrix.clear();
    ledMatrix.scrollTextLeft();
    ledMatrix.drawText();
    ledMatrix.commit();
    delay(50);
}