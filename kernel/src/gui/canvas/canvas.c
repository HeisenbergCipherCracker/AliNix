/**
 * @author Ali Mirmohammad
 * @file canvas.c
 * *************************************IMPORTANT ALINIX LICENSE TERM********************************************
 ** This file is part of AliNix.

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * @abstraction:
 *  - Canvas for the GUI kernel implemented.
*/

#include <alinix/gui/canvas.h>
#include <alinix/types.h>
#include <alinix/math.h>
#include <alinix/memory.h>
#include <alinix/gui/colors.h>
#include <alinix/gui/fonts/font.h>
#include <alinix/module.h>

MODULE_AUTHOR("Ali Mirmohammad")
MODULE_DESCRIPTION("Canvas for the GUI kernel implemented.")
MODULE_LICENSE("AGPL-3.0")
MODULE_VERSION("1.0")

/**
 * Sets the color of a pixel in the canvas at the specified coordinates.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color of the pixel in ARGB format (e.g., 0xFF0000 for red).
 *
 * @return void
 *
 * @throws None
 */
void Canvas_SetPixel(int x, int y, uint32_t color){
*(uint32_t*)((uint32_t)bufferPointer + (y * Width * 4 + x * 4)) = color;
}

/**
 * Retrieves the color of a pixel in the canvas at the specified coordinates.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 *
 * @return The color of the pixel in ARGB format (e.g., 0xFF0000 for red).
 *
 * @throws None
 */
uint32_t Canvas_GetPixel(int x, int y){
    return *(uint32_t*)((uint32_t)bufferPointer + (y * Width * 4 + x * 4));

}

/**
 * Draws a diagonal line on a canvas from (x1, y1) to (x1 + dx, y1 + dy) with the specified color.
 *
 * @param color The color of the line in ARGB format (e.g., 0xFF0000 for red).
 * @param dx The change in x-coordinate of the line.
 * @param dy The change in y-coordinate of the line.
 * @param x1 The initial x-coordinate of the line.
 * @param y1 The initial y-coordinate of the line.
 *
 * @return void
 *
 * @throws None
 */
void DrawDiagonalLine(uint32_t color, int dx, int dy, int x1, int y1){
    int i, sdx, sdy, dxabs, dyabs, x, y, px, py;

    dxabs = Abs(dx);
    dyabs = Abs(dy);
    sdx = Sign(dx);
    sdy = Sign(dy);
    x = dyabs >> 1;
    y = dxabs >> 1;
    px = x1;
    py = y1;

    if (dxabs >= dyabs) // the line is more horizontal than vertical
    {
        for (i = 0; i < dxabs; i++)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            SetPixel(px, py, color);
        }
    }
    else // the line is more vertical than horizontal
    {
        for (i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            SetPixel(px, py, color);
        }
    }
}

/**
 * Draws a vertical line on a canvas from (x1, y1) to (x1, y1 + dy) with the specified color.
 *
 * @param color The color of the line in ARGB format (e.g., 0xFF0000 for red).
 * @param dy The change in y-coordinate of the line.
 * @param x1 The x-coordinate of the line.
 * @param y1 The initial y-coordinate of the line.
 *
 * @return void
 *
 * @throws None
 */
void DrawVerticalLine(uint32_t color, int dy, int x1, int y1)
{
    for(int i = 0; i < dy; i++)
        SetPixel(x1, y1 + i, color);
}

/**
 * Draws a line on a canvas from (x1, y1) to (x2, y2) with the specified color.
 *
 * @param color The color of the line in ARGB format (e.g., 0xFF0000 for red).
 * @param x1 The x-coordinate of the starting point of the line.
 * @param y1 The y-coordinate of the starting point of the line.
 * @param x2 The x-coordinate of the ending point of the line.
 * @param y2 The y-coordinate of the ending point of the line.
 *
 * @return void
 *
 * @throws None
 */
void DrawLine(uint32_t color, int x1, int y1, int x2, int y2)
{
    int dx, dy;

    dx = x2 - x1;      /* the horizontal distance of the line */
    dy = y2 - y1;      /* the vertical distance of the line */

    if (dy == 0) /* The line is horizontal */
    {
        if(dx < 0)
            DrawHorizontalLine(color, Abs(dx), x2, y1);
        else
            DrawHorizontalLine(color, dx, x1, y1);

        return;
    }

    if (dx == 0) /* the line is vertical */
    {
        if(dy < 0)
            DrawVerticalLine(color, Abs(dy), x1, y2);
        else
            DrawVerticalLine(color, dy, x1, y1);
        
        return;
    }

    /* the line is neither horizontal neither vertical, is diagonal then! */
    DrawDiagonalLine(color, dx, dy, x1, y1);
}

/**
 * Draws a horizontal line on a canvas from (x1, y1) to (x1 + dx, y1) with the specified color.
 *
 * @param color The color of the line in ARGB format (e.g., 0xFF0000 for red).
 * @param dx The change in x-coordinate of the line.
 * @param x1 The initial x-coordinate of the line.
 * @param y1 The y-coordinate of the line.
 *
 * @return void
 *
 * @throws None
 */
void DrawHorizontalLine(uint32_t color, int dx, int x1, int y1)
{
    for (int i = 0; i < dx; i++)
        SetPixel(x1 + i, y1, color);
}


/**
 * @brief Draws a filled circle with specified parameters.
 *
 * This function draws a filled circle at the given coordinates (x, y) with a 
 * specified radius and color. The circle can be filled in specific corners 
 * based on the 'corner' parameter and adjusted by the 'delta' parameter.
 *
 * @param x The x-coordinate of the center of the circle.
 * @param y The y-coordinate of the center of the circle.
 * @param radius The radius of the circle.
 * @param corner A bitmask that specifies which corners to fill:
 *               - Bit 0 (0x1): Fill upper right and lower right corners.
 *               - Bit 1 (0x2): Fill upper left and lower left corners.
 * @param delta An additional offset to apply to the fill.
 * @param color The color to fill the circle with.
 *
 * This function uses an incremental algorithm to draw the circle. The circle
 * is divided into four corners, and the `corner` parameter determines which 
 * corners will be filled. The `delta` parameter adds an extra offset to the 
 * filling. The `DrawVerticalLine` function is assumed to draw a vertical line 
 * with a given color, length, and starting coordinates.
 *
 * Example usage:
 * @code
 * FillCircleHelper(50, 50, 20, 0x3, 1, 0xFFFFFF);
 * @endcode
 */
void FillCircleHelper(int x, int y, int radius, uint32_t corner, int delta, uint32_t color)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int i = 0;
    int j = radius;
 
    while (i < j) {
        if (f >= 0) {
            j--;
            ddF_y += 2;
            f += ddF_y;
        }
        i++;
        ddF_x += 2;
        f += ddF_x;
 
        if (corner & 0x1) {
            DrawVerticalLine(color, 2 * j + 1 + delta, x + i, y - j);
            DrawVerticalLine(color, 2 * i + 1 + delta, x + j, y - i);
        }
        if (corner & 0x2) {
            DrawVerticalLine(color, 2 * j + 1 + delta, x - i, y - j);
            DrawVerticalLine(color, 2 * i + 1 + delta, x - j, y - i);
        }
    }
}

/**
 * Fills a circle on a canvas with the specified color.
 *
 * @param x The x-coordinate of the center of the circle.
 * @param y The y-coordinate of the center of the circle.
 * @param radius The radius of the circle.
 * @param corner The bitwise OR of flags indicating the corners to fill (e.g., 0x1 for the top-left corner).
 * @param delta The offset to apply to the coordinates of the filled pixels.
 * @param color The color of the filled pixels in ARGB format (e.g., 0xFF0000 for red).
 *
 * @return void
 *
 * @throws None
 */
void DrawCircleHelper(int x, int y, int radius, uint32_t corner, uint32_t color)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int i = 0;
    int j = radius;
 
    while (i < j) {
        if (f >= 0) {
            j--;
            ddF_y += 2;
            f += ddF_y;
        }
        i++;
        ddF_x += 2;
        f += ddF_x;
        if (corner & 0x4) {
            SetPixel(x + i, y + j, color);
            SetPixel(x + j, y + i, color);
        }
        if (corner & 0x2) {
            SetPixel(x + i, y - j, color);
            SetPixel(x + j, y - i, color);
        }
        if (corner & 0x8) {
            SetPixel(x - j, y + i, color);
            SetPixel(x - i, y + j, color);
        }
        if (corner & 0x1) {
            SetPixel(x - j, y - i, color);
            SetPixel(x - i, y - j, color);
        }
    }
}

// void Clear()
// {
//     memset((void*)bufferPointer, 0, Width*Height*4);
// }

/**
 * Draws a filled rounded rectangle on a canvas with the specified color.
 *
 * @param color The color of the rectangle in ARGB format (e.g., 0xFF0000 for red).
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param radius The radius of the rounded corners.
 *
 * @return void
 *
 * @throws None
 */
void DrawFillRoundedRect(uint32_t color, int x, int y, int width, int height, int radius)
{
    // Draw the body
    DrawFillRect(color, x + radius, y, width - 2 * radius + 1, height);
 
    // Draw the four corners
    FillCircleHelper(x + width - radius - 1, y + radius, radius, 1, height - 2 * radius - 1, color);
    FillCircleHelper(x + radius, y + radius, radius, 2, height - 2 * radius - 1, color);
}

/**
 * Draws a filled rectangle on a canvas with the specified color.
 *
 * @param color The color of the rectangle in ARGB format (e.g., 0xFF0000 for red).
 * @param x_start The x-coordinate of the top-left corner of the rectangle.
 * @param y_start The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 *
 * @return void
 *
 * @throws None
 */
void DrawFillRect(uint32_t color, int x_start, int y_start, int width, int height)
{
    for (int y = y_start; y < y_start + height; y++)
    {
        DrawLine(color, x_start, y, x_start + width, y);
    }
}

/**
 * Draws a rounded rectangle on a canvas with the specified color.
 *
 * @param color The color of the rectangle in ARGB format (e.g., 0xFF0000 for red).
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param radius The radius of the rounded corners.
 *
 * @return void
 *
 * @throws None
 */
void DrawRoundedRect(uint32_t color, int x, int y, int width, int height, int radius)
{
    // Draw the four lines
    DrawHorizontalLine(color, width - 2 * radius, x + radius, y);               // Top
    DrawHorizontalLine(color, width - 2 * radius, x + radius, y + height - 1);  // Bottom
    DrawVerticalLine(color, height - 2 * radius, x, y + radius);                // Left
    DrawVerticalLine(color, height - 2 * radius, x + width - 1, y + radius);    // Right
 
    // Draw the four corners
    DrawCircleHelper(x + radius, y + radius, radius, 1, color);
    DrawCircleHelper(x + width - radius - 1, y + radius, radius, 2, color);
    DrawCircleHelper(x + width - radius - 1, y + height - radius - 1, radius, 4, color);
    DrawCircleHelper(x + radius, y + height - radius - 1, radius, 8, color);
}

void DrawRect(uint32_t color, int x, int y, int width, int height)
{
    int xa = x;
    int ya = y;

    /* The vertex B has the same y coordinate of A but x is moved of width pixels */
    int xb = x + width;
    int yb = y;

    /* The vertex C has the same x coordiate of A but this time is y that is moved of height pixels */
    int xc = x;
    int yc = y + height;

    /* The Vertex D has x moved of width pixels and y moved of height pixels */
    int xd = x + width;
    int yd = y + height;

    /* Draw a line betwen A and B */
    DrawLine(color, xa, ya, xb, yb);

    /* Draw a line between A and C */
    DrawLine(color, xa, ya, xc, yc);

    /* Draw a line between B and D */
    DrawLine(color, xb, yb, xd, yd + 1);

    /* Draw a line between C and D */
    DrawLine(color, xc, yc, xd, yd);
}

/**
 * Draws a rectangle on a canvas with the specified color.
 *
 * @param color The color of the rectangle in ARGB format (e.g., 0xFF0000 for red).
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 *
 * @return void
 *
 * @throws None
 */
void DrawString(struct Font* font, char* string, int x, int y, uint32_t color)
{
    if(font == 0 || string == 0 || color == Transparent)
        return;
    
    int xOffset = x;
    int yOffset = y;
    while(*string)
    {
        // Get the character we need to draw for this string
        char c = *string++;

        // Check for newline
        if(c == '\n') {
            xOffset = x;

            // Add the height of the space character. TODO: Update this!
            yOffset += ((uint8_t*)(font->data + font->offsetTable[0]))[1];
            continue;
        }

        // Load data for this char from the font
        const uint8_t* charData = (uint8_t*)(font->data + font->offsetTable[(int)c - 32]);
        const uint8_t width = charData[0];
        const uint8_t height = charData[1];

        // Loop through the complete bitmap and draw the character
        for(uint8_t px = 0; px < width; px++) {
            for(uint8_t py = 0; py < height; py++) {
                // Can be any value between 0 and 255
                uint8_t d = charData[py * width + px + 2];
                
                // This pixel does not need to be drawn
                if(d == 0)
                    continue;

                // This is a full color pixel
                if(d == 255)
                    SetPixel(px + xOffset, py + yOffset, color);
                // We need to blend this pixel with the background
                else {
                    Color4 realColor;
                    realColor.c = color;
                    realColor.argb.a = d; // Adjust the alpha component of the color. TODO: Also support full transparent text drawing in the future!

                    SetPixel(px + xOffset, py + yOffset, AlphaBlend(Canvas_GetPixel(px + xOffset, py + yOffset), realColor.c));
                }  
            }
        }

        xOffset += width;
    }
}

/**
 * Clears the canvas with the specified color.
 *
 * @param color The color to fill the canvas with in ARGB format (e.g., 0xFF0000 for red).
 *
 * @return void
 *
 * @throws None
 */
void Clear(uint32_t color)
{
    uint32_t* buf = (uint32_t*)bufferPointer;       
    for(uint32_t index = 0; index < (uint32_t)(Width*Height); index++)
        buf[index] = color;
}