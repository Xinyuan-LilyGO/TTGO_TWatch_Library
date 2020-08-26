#include "ePaperCanvas.h"
#include "ePaperDeviceConfigurations.h"

#define DEBUG 1

#if DEBUG
#define DEBUG_PRINTLN(s) Serial.println(s)
#define DEBUG_PRINT(s) Serial.print(s)
#define DEBUG_PRINTFORMAT(s,f) Serial.print(s,f)
#else
#define DEBUG_PRINTLN(s)
#define DEBUG_PRINT(s)
#define DEBUG_PRINTFORMAT(s,f)
#endif

#define swap_coordinates(a, b) \
  (((a) ^= (b)), ((b) ^= (a)), ((a) ^= (b))) ///< No-temp-var swap operation

const uint8_t ePaperCanvas::bitmasks[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

ePaperCanvas::ePaperCanvas(
	int16_t w,
	int16_t h,
	ePaperColorMode mode
)	:	Adafruit_GFX(w, h),
		_bufferSize(0),
		_blackBuffer(NULL),
		_colorBuffer(NULL),
		_mode(mode)
{
	DEBUG_PRINT(F("Creating ePaperCanvas object with w = "));
	DEBUG_PRINT(w);
	DEBUG_PRINT(F(", h = "));
	DEBUG_PRINT(h);
	DEBUG_PRINT(F(", mode = "));
	switch (_mode) {
		case CMODE_BW:
			DEBUG_PRINT(F("Black & White"));
			break;
		case CMODE_3COLOR:
			DEBUG_PRINT(F("3-Color"));
			break;
		case CMODE_4GRAY:
			DEBUG_PRINT(F("4 Gray Scale"));
			break;
		default:
			DEBUG_PRINT(F("UNKNOWN"));
			break;

	}
	DEBUG_PRINT(F("\n"));
	
	_bufferSize = (size_t)width()*(((size_t)height()+7)/8);
	DEBUG_PRINT(F("    Allocating buffers with size = "));
	DEBUG_PRINT(_bufferSize);
	DEBUG_PRINT(F("\n"));
	_blackBuffer = (uint8_t *)malloc(_bufferSize);
	if (_blackBuffer) {
		DEBUG_PRINTLN(F("    Black buffer successfully allocated."));
	} else {
		DEBUG_PRINTLN(F("    ERROR - Black buffer NOT successfully allocated."));
	}
	if (_mode != CMODE_BW) {
		_colorBuffer = (uint8_t *)malloc(_bufferSize);
		if (_colorBuffer) {
			DEBUG_PRINTLN(F("    Color buffer successfully allocated."));
		} else {
			DEBUG_PRINTLN(F("    ERROR - Color buffer NOT successfully allocated."));
		}
	}
}
		
ePaperCanvas::~ePaperCanvas()
{
	if (_blackBuffer) {
		free(_blackBuffer);
	}
	if (_colorBuffer) {
		free(_colorBuffer);
	}
}

void ePaperCanvas::drawPixel(int16_t x, int16_t y, ePaperColorType color)
{
	if((x >= 0) && (x < width()) && (y >= 0) && (y < height())) {
		// Pixel is in-bounds. Rotate coordinates if needed.
		switch(getRotation()) {
			case 1:
				swap_coordinates(x, y);
				x = WIDTH - x - 1;
				break;
			case 2:
				x = WIDTH  - x - 1;
				y = HEIGHT - y - 1;
				break;
			case 3:
				swap_coordinates(x, y);
				y = HEIGHT - y - 1;
				break;
		}
		size_t buffer_index = (y*WIDTH + x)/8;
		int8_t buffer_bit_mask = (1 << ((7-(y*WIDTH + x))&7));

		switch(color) {
			case ePaper_WHITE:
				// setting white is turning the black pixel off
				_blackBuffer[buffer_index] &= ~(buffer_bit_mask); 
				if ((getColorMode() == CMODE_3COLOR)||(getColorMode() == CMODE_4GRAY)) {
					// turn the color off where we set white
					_colorBuffer[buffer_index] &= ~(buffer_bit_mask); 
				} 
				break;
			case ePaper_BLACK:
				// turn appropriate black pixel on
				_blackBuffer[buffer_index] |=  (buffer_bit_mask);
				if (getColorMode() == CMODE_3COLOR) {
					// turn the color off where we set black
					_colorBuffer[buffer_index] &= ~(buffer_bit_mask); 
				} else if (getColorMode() == CMODE_4GRAY) {
					// turn the color on where we set black
					_colorBuffer[buffer_index] |=  (buffer_bit_mask);
				}
				break;
			case ePaper_GRAY1:
				if (getColorMode() == CMODE_4GRAY) {
					// white is off, color is on
					_blackBuffer[buffer_index] &= ~(buffer_bit_mask); 
					_colorBuffer[buffer_index] |=  (buffer_bit_mask);
				}
				break;
			case ePaper_GRAY2:
				if (getColorMode() == CMODE_4GRAY) {
					// white is on, color is off
					_blackBuffer[buffer_index] |=  (buffer_bit_mask);
					_colorBuffer[buffer_index] &= ~(buffer_bit_mask); 
				}
				break;
			case ePaper_COLOR:
				if (getColorMode() == CMODE_3COLOR) {
					// make sure black pixel is off
					_blackBuffer[buffer_index] &= ~(buffer_bit_mask); 
					if (_colorBuffer) {
						// turn the color on 
						_colorBuffer[buffer_index] |=  (buffer_bit_mask);
					}
				}
				break;
			case ePaper_INVERSE1:
				// b -> w, w -> b, c -> w
				// to set the inverse, first see if the pixel has the color. If it does,
				// invert that (set to white). Otherwise, invert the B&W image.
				if (_colorBuffer) {
					if (_colorBuffer[buffer_index]&buffer_bit_mask) {
						_colorBuffer[buffer_index] &= ~(buffer_bit_mask); 
						break;
					}
				}
				_blackBuffer[buffer_index] ^= buffer_bit_mask;
				break;
			case ePaper_INVERSE2:
				// b -> c or w, w -> b, c -> b
				if (_colorBuffer) {
					if (_colorBuffer[buffer_index]&buffer_bit_mask) {
						_colorBuffer[buffer_index] &= ~(buffer_bit_mask); 
						_blackBuffer[buffer_index] |= buffer_bit_mask;
						break;
					}
				}
				if (_blackBuffer[buffer_index]&buffer_bit_mask) {
					_blackBuffer[buffer_index] &= ~(buffer_bit_mask); 
					if (_colorBuffer) {
						_colorBuffer[buffer_index] |= buffer_bit_mask;
					}
				}
				else {
					_blackBuffer[buffer_index] |= buffer_bit_mask;
				}
				break;
			case ePaper_INVERSE3:
				// b -> w, w -> c or b, c -> b
				if (_colorBuffer) {
					if (_colorBuffer[buffer_index]&buffer_bit_mask) {
						_colorBuffer[buffer_index] &= ~(buffer_bit_mask); 
						_blackBuffer[buffer_index] |= buffer_bit_mask;
						break;
					}
				}
				if (_blackBuffer[buffer_index]&buffer_bit_mask) {
					_blackBuffer[buffer_index] &= ~(buffer_bit_mask); 
				}
				else {
					if (_colorBuffer) {
						_colorBuffer[buffer_index] |= buffer_bit_mask;
					}
					else {
						_blackBuffer[buffer_index] |= buffer_bit_mask;
					}
				}
				break;
			default:
				// what color is this?
				break;			
		}
	}		
	yield();
}

void ePaperCanvas::getBitSettingsForColor(uint16_t color, bool& blackBit, bool& colorBit )
{
	blackBit = false;
	colorBit = false;
	
	switch (color) {
		case ePaper_WHITE:
			break;
		case ePaper_BLACK:
			blackBit = true;
			if (getColorMode() == CMODE_4GRAY) {
				colorBit = true;
			}
			break;
		case ePaper_COLOR:
			if (getColorMode() == CMODE_3COLOR) {
				colorBit = true;
			}
			break;
		case ePaper_GRAY1:
			if (getColorMode() == CMODE_4GRAY) {
				blackBit = false;
				colorBit = true;
			}
			break;
		case ePaper_GRAY2:
			if (getColorMode() == CMODE_4GRAY) {
				blackBit = true;
				colorBit = false;
			}
			break;
		default:
			// this method really should be called for other colors
			break;
	}
}
void ePaperCanvas::fillScreen(uint16_t color)
{
	uint8_t blackByte = 0;
	uint8_t colorByte = 0;

	switch (color) {
		case ePaper_WHITE:
		case ePaper_BLACK:
		case ePaper_COLOR:
		case ePaper_GRAY1:
		case ePaper_GRAY2:
			bool blackBitOn, colorBitOn;
			getBitSettingsForColor(color, blackBitOn, colorBitOn);
			blackByte = blackBitOn ? 0xFF : 0x00;
			colorByte = colorBitOn ? 0xFF : 0x00;
			break;
		default:
		case ePaper_INVERSE1:
		case ePaper_INVERSE2:
		case ePaper_INVERSE3:
			Adafruit_GFX::fillScreen(color);
			return;
			break;
	}

	startWrite();
	if (_blackBuffer) {
		memset(_blackBuffer, blackByte, _bufferSize);
	}
	if (_colorBuffer) {
		memset(_colorBuffer, colorByte, _bufferSize);
	}
	endWrite();
}

void ePaperCanvas::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	if (getRotation() == 0) {
		drawFastRawVLine(x, y, h, color);
	} else if (getRotation() == 1) {
		int16_t t = x;
		x = WIDTH - 1 - y;
		y = t;
		x -= h;
		drawFastRawHLine(x, y, h, color);
	} else if (getRotation() == 2) {
		x = WIDTH - 1 - x;
      	y = HEIGHT - 1 - y;
      	
      	y -= h;
      	drawFastRawVLine(x, y, h, color);
	} else if (getRotation() == 3) {
		int16_t t = x;
		x = y;
		y = HEIGHT - 1 - t;
		drawFastRawHLine(x, y, h, color);
	} else {
		this->Adafruit_GFX::drawFastVLine(x, y, h, color);
	}
}

void ePaperCanvas::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
	if (getRotation() == 0) {
		drawFastRawHLine(x, y, w, color);
	} else if (getRotation() == 1) {
		int16_t t = x;
		x = WIDTH - 1 - y;
		y = t;
		drawFastRawVLine(x, y, w, color);
	} else if (getRotation() == 2) {
		x = WIDTH - 1 - x;
      	y = HEIGHT - 1 - y;
      	
      	x -= w;
      	drawFastRawHLine(x, y, w, color);
	} else if (getRotation() == 3) {
		int16_t t = x;
		x = y;
		y = HEIGHT - 1 - t;
		y -= w;
		drawFastRawVLine(x, y, w, color);
	} else {
		this->Adafruit_GFX::drawFastHLine(x, y, w, color);
	}
}

void ePaperCanvas::drawFastRawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	// should not be called for inverse colors
	if ((color == ePaper_INVERSE1)||(color == ePaper_INVERSE2)||(color == ePaper_INVERSE3)) {
		return;
	}
	
	// first determine bit settings for color:
	bool blackBitOn, colorBitOn;
	getBitSettingsForColor(color, blackBitOn, colorBitOn);
	
	// calculate start bye and subbit
	size_t start_bit_index = (y*WIDTH + x);
	size_t start_buffer_index = start_bit_index/8;
	int8_t start_sub_bit = ((7-start_bit_index)&7);

	// calculate bit mask
	uint8_t byte_bit_mask = ePaperCanvas::bitmasks[start_sub_bit];
	
	// repeatedly apply the bit mask for each row
	size_t row_bytes = WIDTH/8;
	for (int16_t i = 0; i < h; i++) {
		size_t buffer_index = start_buffer_index + i*row_bytes;
		
		if (buffer_index >= _bufferSize) {
			DEBUG_PRINT(F("WARNING - Buffer index exceeded buffer size in ePaperCanvas::drawFastRawVLine(). x = "));
			DEBUG_PRINT(x);
			DEBUG_PRINT(F(", y = "));
			DEBUG_PRINT(y);
			DEBUG_PRINT(F(", h = "));
			DEBUG_PRINT(h);
			DEBUG_PRINT(F(", color = "));
			DEBUG_PRINT(color);
			DEBUG_PRINT(F(", iteration idx = "));
			DEBUG_PRINT(i);
			DEBUG_PRINT(F(", buffer_index = "));
			DEBUG_PRINT(buffer_index);
			DEBUG_PRINT(F("\n"));
			yield();
			return;
		}
		
		if (blackBitOn) {
			_blackBuffer[buffer_index] |= byte_bit_mask;
		} else {
			_blackBuffer[buffer_index] &= (~byte_bit_mask);
		}
	
		if (_colorBuffer) {
			if (colorBitOn) {
				_colorBuffer[buffer_index] |= byte_bit_mask;
			} else {
				_colorBuffer[buffer_index] &= (~byte_bit_mask);
			}
		}
		yield();
	}
}

void ePaperCanvas::drawFastRawHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	// should not be called for inverse colors
	if ((color == ePaper_INVERSE1)||(color == ePaper_INVERSE2)||(color == ePaper_INVERSE3)) {
		return;
	}
		
	// first determine bit settings for color:
	bool blackBitOn, colorBitOn;
	getBitSettingsForColor(color, blackBitOn, colorBitOn);

	// calculate start bye and subbit
	size_t start_bit_index = (y*WIDTH + x);
	size_t remainingWidthBits = w;
	size_t start_buffer_index = start_bit_index/8;
	
	if (start_buffer_index >= _bufferSize) {
		DEBUG_PRINT(F("WARNING - Start buffer index exceeded buffer size in ePaperCanvas::drawFastRawHLine(). x = "));
		DEBUG_PRINT(x);
		DEBUG_PRINT(F(", y = "));
		DEBUG_PRINT(y);
		DEBUG_PRINT(F(", w = "));
		DEBUG_PRINT(w);
		DEBUG_PRINT(F(", color = "));
		DEBUG_PRINT(color);
		DEBUG_PRINT(F(", start_buffer_index = "));
		DEBUG_PRINT(start_buffer_index);
		DEBUG_PRINT(F("\n"));
		yield();
		return;
	}
	// handle the sub-bit of the first byte if needed
	if (start_bit_index%8 > 0) {
		int8_t start_sub_bit = ((7-start_bit_index)&7);
	
		// create bit mask for first byte
		uint8_t start_byte_bit_mask = 0x00;
	
		for (int8_t i = start_sub_bit; ((i >= 0)&&(remainingWidthBits > 0)); i--) {
			start_byte_bit_mask |= ePaperCanvas::bitmasks[i];
			remainingWidthBits--;
		}
	
		// set the start byte 
	
		if (blackBitOn) {
			_blackBuffer[start_buffer_index] |= start_byte_bit_mask;
		} else {
			_blackBuffer[start_buffer_index] &= (~start_byte_bit_mask);
		}
	
		if (_colorBuffer) {
			if (colorBitOn) {
				_colorBuffer[start_buffer_index] |= start_byte_bit_mask;
			} else {
				_colorBuffer[start_buffer_index] &= (~start_byte_bit_mask);
			}
		}
		start_buffer_index++;
	}
	
	// do the next remainingWidthBits bits
	if (remainingWidthBits > 0 ) {
		size_t remainingWholeBytes = remainingWidthBits/8;
		size_t lastByteBits = remainingWidthBits%8;

		uint8_t blackByte = blackBitOn ? 0xFF : 0x00;
		uint8_t colorByte = colorBitOn ? 0xFF : 0x00;
	
		// set the remaining whole bytes
		for (size_t i = start_buffer_index;
				i < start_buffer_index + remainingWholeBytes;
				i++
		) {
			if (i >= _bufferSize) {
				DEBUG_PRINT(F("WARNING - Iteration buffer index exceeded buffer size in ePaperCanvas::drawFastRawHLine(). x = "));
				DEBUG_PRINT(x);
				DEBUG_PRINT(F(", y = "));
				DEBUG_PRINT(y);
				DEBUG_PRINT(F(", w = "));
				DEBUG_PRINT(w);
				DEBUG_PRINT(F(", color = "));
				DEBUG_PRINT(color);
				DEBUG_PRINT(F(", iteration index = "));
				DEBUG_PRINT(i);
				DEBUG_PRINT(F("\n"));
				yield();
				return;
			}
		
			_blackBuffer[i] = blackByte;
			if (_colorBuffer) {
				_colorBuffer[i] = colorByte;
			}
			yield();
		}
		
		// set the last byte's left bits
		if (lastByteBits > 0) {
			if (start_buffer_index + remainingWholeBytes >= _bufferSize) {
				DEBUG_PRINT(F("WARNING - Last byte index exceeded buffer size in ePaperCanvas::drawFastRawHLine(). x = "));
				DEBUG_PRINT(x);
				DEBUG_PRINT(F(", y = "));
				DEBUG_PRINT(y);
				DEBUG_PRINT(F(", w = "));
				DEBUG_PRINT(w);
				DEBUG_PRINT(F(", color = "));
				DEBUG_PRINT(color);
				DEBUG_PRINT(F(", last byte index = "));
				DEBUG_PRINT(start_buffer_index + remainingWholeBytes);
				DEBUG_PRINT(F("\n"));
				yield();
				return;
			}


			uint8_t last_byte_bit_mask = 0x00;
			for (int8_t i = 7; i >= 7-(int8_t)lastByteBits; i--) {
				last_byte_bit_mask |= ePaperCanvas::bitmasks[i];
			}
			if (blackBitOn) {
				_blackBuffer[start_buffer_index + remainingWholeBytes] |= last_byte_bit_mask;
			} else {
				_blackBuffer[start_buffer_index + remainingWholeBytes] &= (~last_byte_bit_mask);
			}
			if (_colorBuffer) {
				if (colorBitOn) {
					_colorBuffer[start_buffer_index + remainingWholeBytes] |= last_byte_bit_mask;
				} else {
					_colorBuffer[start_buffer_index + remainingWholeBytes] &= (~last_byte_bit_mask);
				}
			}
		}
	}
}

void  ePaperCanvas::invertDisplay(boolean i)
{
	if (!i) return;
	
	if (_colorBuffer != nullptr) {
		startWrite();
		uint8_t *tempPtr = _colorBuffer;
		_colorBuffer = _blackBuffer;
		_blackBuffer = tempPtr;
		endWrite();
	} else {
		fillScreen(ePaper_INVERSE1);
	}
}


/*!
    @brief  Sets the device image buffer directly. 
    @param	blackBitMap		the bitmap to set the bit and white image to. High (1) values set
    						the pixel black, low (0) sets the pixel white. 
    @param	blackBitMapSize	the size of the black bitmap in total bytes.
    @param	blackBitMapIsProgMem	indicates whether the passed bitmap resides in PROGMEM or not.
    @return None (void).
    @note   Since this sets the image buffer directly, the bit map must be correctly sized
    		for the device's demensions. Also, any rotation that has been set is ignored. 
*/
void ePaperCanvas::setDeviceImage( 
	const uint8_t* blackBitMap,
	size_t blackBitMapSize,
	bool blackBitMapIsProgMem
)
{
	setDeviceImage(blackBitMap, blackBitMapSize, blackBitMapIsProgMem, nullptr, 0, false);
}

/*!
    @brief  Sets the device image buffers directly. 
    @param	blackBitMap		the bitmap to set the black and white image to. High (1) values set
    						the pixel black, low (0) sets the pixel white. may be null.
    @param	blackBitMapSize	the size of the black bitmap in total bytes.
    @param	blackBitMapIsProgMem	indicates whether the passed bitmap resides in PROGMEM or not.
    @param	colorBitMap		the bitmap to set the color image to. High (1) values set
    						the pixel colored, low (0) sets the uncolored. 
    @param	colorBitMapSize	the size of the color bitmap in total bytes.
    @param	colorBitMapIsProgMem	indicates whether the passed bitmap resides in PROGMEM or not.
    @return None (void).
    @note   Since this sets the image buffers directly, the bit map must be correctly sized
    		for the device's demensions. Also, any rotation that has been set is ignored. 
*/
void ePaperCanvas::setDeviceImage( 
	const uint8_t* blackBitMap,
	size_t blackBitMapSize,
	bool blackBitMapIsProgMem,
	const uint8_t* colorBitMap,
	size_t colorBitMapSize,
	bool colorBitMapIsProgMem
)
{
	if (blackBitMap && _blackBuffer && (blackBitMapSize <= _bufferSize)) {
		if (blackBitMapIsProgMem) {
			memcpy_P(_blackBuffer, blackBitMap, blackBitMapSize);
		} else {
			memcpy(_blackBuffer, blackBitMap, blackBitMapSize);
		}
	}
	if (colorBitMap && _colorBuffer && (colorBitMapSize <= _bufferSize)) {
		if (blackBitMapIsProgMem) {
			memcpy_P(_colorBuffer, colorBitMap, colorBitMapSize);
		} else {
			memcpy(_colorBuffer, colorBitMap, colorBitMapSize);
		}
	}
}


void ePaperCanvas::drawBitImage( 
	int16_t loc_x, int16_t loc_y,
	int16_t img_w, int16_t img_h,
	const uint8_t* blackBitMap,
	size_t blackBitMapSize,
	bool blackBitMapIsProgMem,
	const uint8_t* colorBitMap,
	size_t colorBitMapSize,
	bool colorBitMapIsProgMem
)
{

	for (int16_t i = 0; i < img_w; i++ ) {
		for (int16_t j = 0; j < img_h; j++ ) {
			size_t bit_index = (j*img_w + i);
			size_t buffer_index = bit_index/8;
			int8_t buffer_bit_mask = (1 << ((7-bit_index)&7));
			
			if (blackBitMap && colorBitMap && (getColorMode() == CMODE_4GRAY)) {
				// in 4 gray mode need to interpret the bits
				if( (buffer_index < blackBitMapSize)&&(buffer_index <= colorBitMapSize)) {
					uint8_t blackByteVal = blackBitMapIsProgMem ? pgm_read_byte(&blackBitMap[buffer_index]) : blackBitMap[buffer_index];
					uint8_t colorByteVal = colorBitMapIsProgMem ? pgm_read_byte(&colorBitMap[buffer_index]) : colorBitMap[buffer_index];
					bool isBlack = blackByteVal&buffer_bit_mask ? true : false;
					bool isColor = colorByteVal&buffer_bit_mask ? true : false;
					ePaperColorType c = ePaper_BLACK;
					if (isBlack&&isColor) {
						// both on is black
						c = ePaper_BLACK;
					}
					else if (isBlack && (!isColor)) {
						c = ePaper_GRAY2;
					}
					else if ((!isBlack) && isColor) {
						c = ePaper_GRAY1;
					}
					else if ((!isBlack) && (!isColor)) {
						c = ePaper_WHITE;
					}
					
					this->drawPixel(loc_x+i, loc_y+j, c);
				}
				
			}
			else if (getColorMode() != CMODE_4GRAY) {
				// use B&W or 3 Color mode
				if (blackBitMap && _blackBuffer && (buffer_index < blackBitMapSize)) {
					uint8_t byteVal = blackBitMapIsProgMem ? pgm_read_byte(&blackBitMap[buffer_index]) : blackBitMap[buffer_index];
					bool isBlack = byteVal&buffer_bit_mask ? true : false;
					this->drawPixel(loc_x+i, loc_y+j, isBlack ? ePaper_BLACK : ePaper_WHITE );
				}
				if (colorBitMap && _colorBuffer && (buffer_index <= colorBitMapSize)) {
					uint8_t byteVal = colorBitMapIsProgMem ? pgm_read_byte(&colorBitMap[buffer_index]) : colorBitMap[buffer_index];
					bool isColor = byteVal&buffer_bit_mask ? true : false;
					// only set red, let B&W image be the "background"
					if (isColor) {
						this->drawPixel(loc_x+i, loc_y+j, ePaper_COLOR);
					}
				}
			}
		}
		yield();
	}
}	
