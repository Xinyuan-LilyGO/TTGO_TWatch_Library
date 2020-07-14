# CUSTOM BACKGROUND IMAGE EXAMPLE
background images are built using https://lvgl.io/tools/imageconverter.

## Example
Once a valid image(240x240) is added, specify the following options in the form:
* **Name**: custom_bg
* **Color format**: True color
* **Output format**: C array.
 
By clicking convert button a **custom_bg.c** file is downloaded.  
place it under **src/imgs** path.

Use generated background image by declaring it with `LV_IMG_DECLARE(custom_bg);` for example inside **gui.cpp**


