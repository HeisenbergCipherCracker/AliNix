/**
 * @author Ali Mirmohammad
 * @file textgui.c
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
 *  - TextGUI utility implemented here.
*/
#include <alinix/textgui.h>

volatile uint16_t* videoMemory = (uint16_t*)0xC00B8000;

void DisableCursor()
{
    outportb(0x3D4, 0x0A);
	outportb(0x3D5, 0x20);
}
void SetPixel(int x, int y, char color, uint16_t character, char background)
{
    uint16_t attrib = (background << 4) | (color & 0x0F);
    *(videoMemory + (y * VGA_WIDTH + x)) = character | (attrib << 8);
}
void ClearScreen(char color)
{
    for(int y = 0; y < VGA_HEIGHT; y++)
        for(int x = 0; x < VGA_WIDTH; x++)
        // TODO check here
            SetPixel(x, y, color, ' ',VGA_COLOR_BLACK);
}
void StatusBar(char* text, int percentage)
{
    /**
     * @param text: Text to display
     * @param percentage: Percentage of progress completed [0-100]
    */
    for(int x = 0; x < VGA_WIDTH; x++)
        SetPixel(x, VGA_HEIGHT - 1, TEXT_COLOR, ' ', VGA_COLOR_LIGHT_GREY);
    
    DrawString(text, 0, VGA_HEIGHT - 1, TEXT_COLOR, VGA_COLOR_LIGHT_GREY);

    const int barWidth = 20; //Amount of characters for status bar
    const int startX = VGA_WIDTH - barWidth - 1; //Start of progress bar
    int width = ((double)percentage / 100.0) * (double)barWidth;

    SetPixel(startX, VGA_HEIGHT - 1, VGA_COLOR_BLACK, '[', VGA_COLOR_LIGHT_GREY);
    for(int i = 0; i < width; i++)
        SetPixel(startX + i + 1, VGA_HEIGHT - 1, VGA_COLOR_BLACK, '#', VGA_COLOR_LIGHT_GREY);
    
    SetPixel(startX + barWidth, VGA_HEIGHT - 1, VGA_COLOR_BLACK, ']', VGA_COLOR_LIGHT_GREY);
}
void TextDrawString(char* text, int x, int y, char color, char background)
{
    int px = x;
    int py = y;
    for(int i = 0; text[i] != '\0'; ++i) {
        switch(text[i]) {
            case '\n':
                px = x;
                py += 1;
                break;
            default:
                SetPixel(px, py, color, text[i], background);
                px++;
                break;
        }
    }
}