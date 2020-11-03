/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _EYEDRAWER_h
#define _EYEDRAWER_h

#include "config.h"
#define TFT_COLOR TFT_YELLOW


#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "EyeConfig.h"


enum CornerType {
    T_R, T_L, B_L, B_R
};


class EyeDrawer
{

public:

    static void Draw(TFT_eSprite &buffer, int16_t centerX, int16_t centerY, EyeConfig *config)
    {
        int32_t delta_y_top = config->Height * config->Slope_Top / 2.0;
        int32_t delta_y_bottom = config->Height * config->Slope_Bottom / 2.0;

        auto totalHeight = config->Height + delta_y_top - delta_y_bottom;
        if (config->Radius_Bottom > 0 && config->Radius_Top > 0 && totalHeight - 1 < config->Radius_Bottom + config->Radius_Top) {
            int32_t corrected_radius_top = (float)config->Radius_Top * (totalHeight - 1) / (config->Radius_Bottom + config->Radius_Top);
            int32_t corrected_radius_bottom = (float)config->Radius_Bottom * (totalHeight - 1) / (config->Radius_Bottom + config->Radius_Top);
            config->Radius_Top = corrected_radius_top;
            config->Radius_Bottom = corrected_radius_bottom;
        }

        int32_t TLc_x = centerY + config->OffsetY - config->Height / 2 + config->Radius_Top + delta_y_top;
        int32_t TLc_y = centerX + config->OffsetX + config->Width / 2 - config->Radius_Top;
        int32_t TL0_x = TLc_x;
        int32_t TL0_y = TLc_y + config->Radius_Top;
        int32_t TL1_x = TLc_x - config->Radius_Top;
        int32_t TL1_y = TLc_y;

        int32_t TRc_x = centerY + config->OffsetY - config->Height / 2 + config->Radius_Top - delta_y_top;
        int32_t TRc_y = centerX + config->OffsetX - config->Width / 2 + config->Radius_Top;
        int32_t TR0_x = TRc_x;
        int32_t TR0_y = TRc_y - config->Radius_Top;
        int32_t TR1_x = TRc_x - config->Radius_Top;
        int32_t TR1_y = TRc_y;


        int32_t BLc_x = centerY + config->OffsetY + config->Height / 2 - config->Radius_Bottom + delta_y_bottom;
        int32_t BLc_y = centerX + config->OffsetX + config->Width / 2 - config->Radius_Bottom;
        int32_t BL0_x = BLc_x;
        int32_t BL0_y = BLc_y + config->Radius_Bottom;
        int32_t BL1_x = BLc_x + config->Radius_Bottom;
        int32_t BL1_y = BLc_y;

        int32_t BRc_x = centerY + config->OffsetY + config->Height / 2 - config->Radius_Bottom - delta_y_bottom;
        int32_t BRc_y = centerX + config->OffsetX - config->Width / 2 + config->Radius_Bottom;
        int32_t BR0_x = BRc_x;
        int32_t BR0_y = BRc_y - config->Radius_Bottom;
        int32_t BR1_x = BRc_x + config->Radius_Bottom;
        int32_t BR1_y = BRc_y;


        int32_t iTLc_x = TL1_x;
        int32_t iTLc_y = TL1_y - config->Inverse_Radius_Top;
        int32_t iTL0_x = iTLc_x + config->Inverse_Radius_Top;
        int32_t iTL0_y = iTLc_y;
        int32_t iTL1_x = iTL0_x;
        int32_t iTL1_y = iTLc_y + config->Inverse_Radius_Top;

        int32_t iTRc_x = TR1_x;
        int32_t iTRc_y = TR1_y + config->Inverse_Radius_Top;
        int32_t iTR0_x = iTRc_x + config->Inverse_Radius_Top;
        int32_t iTR0_y = iTRc_y;
        int32_t iTR1_x = iTRc_x;
        int32_t iTR1_y = iTRc_y - config->Inverse_Radius_Top;

        int32_t iBLc_x = BL1_x;
        int32_t iBLc_y = BL1_y - config->Inverse_Radius_Bottom;
        int32_t iBL0_x = iBLc_x - config->Inverse_Radius_Bottom;
        int32_t iBL0_y = iBLc_y;
        int32_t iBL1_x = iBLc_x - config->Inverse_Radius_Bottom;
        int32_t iBL1_y = iBLc_y;

        int32_t iBRc_x = BR1_x;
        int32_t iBRc_y = BR1_y + config->Inverse_Radius_Bottom;
        int32_t iBR0_x = iBRc_x - config->Inverse_Radius_Bottom;
        int32_t iBR0_y = iBRc_y;
        int32_t iBR1_x = iBRc_x - config->Inverse_Radius_Bottom;
        int32_t iBR1_y = iBRc_y;


        int32_t min_c_x = max(TLc_x, TRc_x);
        int32_t max_c_x = min(BLc_x, BRc_x);
        int32_t min_c_y = max(TRc_y, BRc_y);
        int32_t max_c_y = min(TLc_y, BLc_y);

        int32_t min_e_x = max(TL1_x, TR1_x);
        int32_t max_e_x = min(BL1_x, BR1_x);
        int32_t min_e_y = max(BR0_y, TR0_y);
        int32_t max_e_y = min(TL0_y, BL0_y);

        EyeDrawer::FillReactangle(buffer, min_c_x, min_c_y, max_c_x, max_c_y, TFT_COLOR);

        EyeDrawer::FillReactangle(buffer, TRc_x, min_e_y, BRc_x, min_c_y, TFT_COLOR);
        EyeDrawer::FillReactangle(buffer, TLc_x, max_c_y, BLc_x, max_e_y, TFT_COLOR);
        EyeDrawer::FillReactangle(buffer, min_e_x, TRc_y, min_c_x, TLc_y, TFT_COLOR);
        EyeDrawer::FillReactangle(buffer, max_c_x, BRc_y, max_e_x, BLc_y, TFT_COLOR);

        if (config->Slope_Top > 0) {
            EyeDrawer::FillRectangularTriangle(buffer, TL1_x, TL1_y, TR1_x, TR1_y, TFT_BLACK);
            EyeDrawer::FillRectangularTriangle(buffer, TR1_x, TR1_y, TL1_x, TL1_y, TFT_COLOR);
        } else if (config->Slope_Top < 0) {
            EyeDrawer::FillRectangularTriangle(buffer, TR1_x, TR1_y, TL1_x, TL1_y, TFT_BLACK);
            EyeDrawer::FillRectangularTriangle(buffer, TL1_x, TL1_y, TR1_x, TR1_y, TFT_COLOR);
        }

        if (config->Slope_Bottom > 0) {
            EyeDrawer::FillRectangularTriangle(buffer, BR1_x, BR1_y, BL1_x, BL1_y, TFT_BLACK);
            EyeDrawer::FillRectangularTriangle(buffer, BL1_x, BL1_y, BR1_x, BR1_y, TFT_COLOR);
        } else if (config->Slope_Bottom < 0) {
            EyeDrawer::FillRectangularTriangle(buffer, BL1_x, BL1_y, BR1_x, BR1_y, TFT_BLACK);
            EyeDrawer::FillRectangularTriangle(buffer, BR1_x, BR1_y, BL1_x, BL1_y, TFT_COLOR);
        }


        if (config->Radius_Top > 0) {
            EyeDrawer::FilleEllipseCorner(buffer, T_L, TLc_x, TLc_y, config->Radius_Top, config->Radius_Top, TFT_COLOR);
            EyeDrawer::FilleEllipseCorner(buffer, T_R, TRc_x, TRc_y, config->Radius_Top, config->Radius_Top, TFT_COLOR);
        }

        if (config->Radius_Bottom > 0) {
            EyeDrawer::FilleEllipseCorner(buffer, B_L, BLc_x, BLc_y, config->Radius_Bottom, config->Radius_Bottom, TFT_COLOR);
            EyeDrawer::FilleEllipseCorner(buffer, B_R, BRc_x, BRc_y, config->Radius_Bottom, config->Radius_Bottom, TFT_COLOR);
        }

        if (config->Inverse_Radius_Top > 0) {
            EyeDrawer::FillReactangle(buffer, min(iTLc_x, iTRc_x) - 1, iTRc_y, min(iTL0_x, iTR0_x) + 1, iTLc_y, TFT_BLACK);

            if (config->Slope_Top > 0) {

                EyeDrawer::FillRectangularTriangle(buffer, iTL0_x, iTL0_y, iTR0_x, iTR0_y, TFT_BLACK);
            } else if (config->Slope_Top < 0) {
                EyeDrawer::FillRectangularTriangle(buffer, iTR0_x, iTR0_y, iTL0_x, iTL0_y, TFT_BLACK);
            }

            EyeDrawer::FilleEllipseCorner(buffer, B_L, iTLc_x - 1, iTLc_y, config->Inverse_Radius_Top, config->Inverse_Radius_Top, TFT_BLACK);
            EyeDrawer::FilleEllipseCorner(buffer, B_R, iTRc_x - 1, iTRc_y, config->Inverse_Radius_Top, config->Inverse_Radius_Top, TFT_BLACK);
        }

        if (config->Inverse_Radius_Bottom > 0) {
            EyeDrawer::FilleEllipseCorner(buffer, T_L, iBLc_x + 1, iBLc_y, config->Inverse_Radius_Bottom, config->Inverse_Radius_Bottom, TFT_BLACK);
            EyeDrawer::FilleEllipseCorner(buffer, T_R, iBRc_x + 1, iBRc_y, config->Inverse_Radius_Bottom, config->Inverse_Radius_Bottom, TFT_BLACK);

            EyeDrawer::FillReactangle(buffer, max(iBL0_x, iBR0_x) + 1, iBRc_y - 1, max(iBLc_x, iBRc_x) + 1, iBLc_y + 1, TFT_BLACK);

            if (config->Slope_Bottom > 0) {
                EyeDrawer::FillRectangularTriangle(buffer, iBR0_x, iBR0_y, iBL0_x, iBL0_y, TFT_BLACK);
            } else if (config->Slope_Bottom < 0) {
                EyeDrawer::FillRectangularTriangle(buffer, iBL0_x, iBL0_y, iBR0_x, iBR0_y, TFT_BLACK);
            }
        }

    }

    static void FilleEllipseCorner(TFT_eSprite &buffer, CornerType corner, int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color)
    {
        if (rx < 2) return;
        if (ry < 2) return;
        int32_t x, y;
        int32_t rx2 = rx * rx;
        int32_t ry2 = ry * ry;
        int32_t fx2 = 4 * rx2;
        int32_t fy2 = 4 * ry2;
        int32_t s;

        if (corner == T_R) {
            for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {

                buffer.drawFastHLine(x0 - x, y0 - y, x, color);

                if (s >= 0) {
                    s += fx2 * (1 - y);
                    y--;
                }
                s += ry2 * ((4 * x) + 6);
            }

            for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {

                buffer.drawFastHLine(x0 - x, y0 - y, x, color);

                if (s >= 0) {
                    s += fy2 * (1 - x);
                    x--;
                }
                s += rx2 * ((4 * y) + 6);
            }
        }

        else if (corner == B_R) {
            for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {

                buffer.drawFastHLine(x0, y0 - y, x, color);

                if (s >= 0) {
                    s += fx2 * (1 - y);
                    y--;
                }
                s += ry2 * ((4 * x) + 6);
            }

            for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {

                buffer.drawFastHLine(x0, y0 - y, x, color);

                if (s >= 0) {
                    s += fy2 * (1 - x);
                    x--;
                }
                s += rx2 * ((4 * y) + 6);
            }
        }

        else if (corner == T_L) {
            for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {

                buffer.drawFastHLine(x0 - x, y0 + y - 1, x, color);

                if (s >= 0) {
                    s += fx2 * (1 - y);
                    y--;
                }
                s += ry2 * ((4 * x) + 6);
            }

            for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {

                buffer.drawFastHLine(x0 - x, y0 + y - 1, x, color);

                if (s >= 0) {
                    s += fy2 * (1 - x);
                    x--;
                }
                s += rx2 * ((4 * y) + 6);
            }
        }

        else if (corner == B_L) {
            for (x = 0, y = ry, s = 2 * ry2 + rx2 * (1 - 2 * ry); ry2 * x <= rx2 * y; x++) {

                buffer.drawFastHLine(x0, y0 + y - 1, x, color);

                if (s >= 0) {
                    s += fx2 * (1 - y);
                    y--;
                }
                s += ry2 * ((4 * x) + 6);
            }

            for (x = rx, y = 0, s = 2 * rx2 + ry2 * (1 - 2 * rx); rx2 * y <= ry2 * x; y++) {

                buffer.drawFastHLine(x0, y0 + y - 1, x, color);

                if (s >= 0) {
                    s += fy2 * (1 - x);
                    x--;
                }
                s += rx2 * ((4 * y) + 6);
            }

        }
    }

    static void FillReactangle(TFT_eSprite &buffer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t color)
    {
        buffer.fillRect(x0, y0, x1 - x0, y1 - y0, color);
    }

    static void FillRectangularTriangle(TFT_eSprite &buffer, int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t color)
    {
        buffer.fillTriangle(x0, y0, x1, y1, x1, y0, color);
    }
};


#endif
