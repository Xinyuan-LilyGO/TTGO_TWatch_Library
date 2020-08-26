# ePaper Driver Lib
An unofficial Arduino library for ePaper displays from various manufacturers and distributers. This library implements the Adafruit GFX interface, providing the Arduino community a familiar API for manipulating the pixels on the screen.  This library is currently designed to only support SPI communications with the ePaper device. The goal of this library is to keep simple all aspects of using the ePaper device. ePaper devices are conceptually simple, your code to use them should not look complicated.

This library supports the following colors in the GFX:

* `ePaper_WHITE` - Sets the pixel white
* `ePaper_BLACK` - Sets the pixel black
* `ePaper_COLOR` - If the ePaper device supports a third color (e.g., red), sets the pixel to that color.
* `ePaper_INVERSE1` - If the pixel is currently white, set it black, and vice versa. If the pixel is currently the device's third color, set it white.
* `ePaper_INVERSE2` - If the pixel is currently white, set it black. If it is black, set it to the device's third color (or white if B&W). If the pixel is currently the device's third color, set it black.
* `ePaper_INVERSE3` - If the pixel is currently white, set it to the device's third color (or black if B&W). If it is black, set it white. If the pixel is currently the device's third color, set it black.

These color constants are defined in the `ePaperDriver.h` header.

To initialize an ePaper device object, you simply declare it and call `initializeDevice()` as follows:
```
ePaperDisplay	device(
		CFAP176264A0_0270,	// device constant indicating what model it is
		D1,		// microtroller pin connected to display's BUSY pin
		D2,		// microtroller pin connected to display's RST pin
		D3,		// microtroller pin connected to display's D/C pin
		D8		// microtroller pin connected to display's CS pin
	);
device.initializeDevice();
```
Here, ESP8266 pin naming is used. Change as you need. The rest of the ePaper device pins are SPI and power. Connect to your microcontroller as appropriate. 

## Supported Models

* Crystalfontz
    * **[CFAP104212E0-0213](https://www.crystalfontz.com/product/cfap104212e00213-yellow-black-white-epaper-display)** - 2.13 inch 3-color ePaper display - yellow
    * **[CFAP104212C0-0213](https://www.crystalfontz.com/product/cfap104212c00213-epaper-eink-color-display)** - 2.13 inch 3-color ePaper display - red
    * **[CFAP176264A0-0270](https://www.crystalfontz.com/product/cfap176264a00270-3-color-epaper-display)** - 2.7 inch 3-color ePaper display
    * **[CFAP400300C0-0420](https://www.crystalfontz.com/product/cfap400300c00420-400x300-3-color-epaper-display)** - 4.2" 3-Color ePaper Display - yellow
    * **[CFAP400300A0-0420](https://www.crystalfontz.com/product/cfap400300a00420-3-color-4-inch-epaper-display)** - 4.2" 3-Color ePaper Display - red

## Requirements
The following Arduino libraries are required to make this library work: 

* [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) - Learn more how to use GFX [here](https://learn.adafruit.com/adafruit-gfx-graphics-library/).

This library uses a RAM buffer to manage the screen image. As such, this library can be a RAM hog for larger ePaper displays. The amount of ram needed can be calculated by multiple the device's dimensions and then dividing by 8. If the device has a third color, double the RAM needs. For example, the Crystalfontz `CFAP176264A0-0270` device has the pixel dimensions of 176 by 264, and it has three colors. This means it's RAM buffer will need 11,616 bytes. Ensure that you are using a microcontroller that can handle the RAM needs of your board. An excellent microcontroller board choice would be [the Teensy 3.2](https://www.pjrc.com/store/teensy32.html), as it has 64K of RAM and operates at 3.3V (similar to ePaper devices). This is adequate to drive the 7.5 inch 640x384 B&W ePaper device. 

# Disclaimer 

This project and its creators are not associated with any ePaper manufacturer or Adafruit, nor is this project officially endorsed or reviewed for correctness by any ePaper manufacturer. This project is an open source effort by the community to make a usable library for ePaper displays.
