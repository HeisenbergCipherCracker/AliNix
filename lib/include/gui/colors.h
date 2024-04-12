#ifndef __LIBCACTUSOS__GUI__COLORS_H
#define __LIBCACTUSOS__GUI__COLORS_H

#include <types.h>

namespace LIBHeisenKernel
{

    /**
     * @brief Colors implemented for the chess game 
     * 
    */

    namespace Chess{

        namespace engine{
        // Constants
        const int ColorNone = -1;

        const int White = 0;
        const int Black = 1;

        const int ColorNb = 2;

        const int WhiteFlag = 1 << White;
        const int BlackFlag = 1 << Black;
    
        };
    
    // Some useful macros

    #define COLOR_IS_OK(colour) (((colour)&~1)==0)

    #define COLOUR_IS_WHITE(colour) ((colour)==White)
    #define COLOUR_IS_BLACK(colour) ((colour)!=White)

    #define COLOUR_IS(piece,colour) (FLAG_IS((piece),COLOUR_FLAG(colour)))
    #define FLAG_IS(piece,flag)     (((piece)&(flag))!=0)

    #define COLOUR_OPP(colour)      ((colour)^(White^Black))
    #define COLOUR_FLAG(colour)     ((colour)+1)
    };
    // Union describing a ARGB color in the following format:
    // 0xAARRGGBB
    typedef union Color4Tag
    {
        uint32_t c;
        struct ColorComponents
        {
            uint8_t b;
            uint8_t g;
            uint8_t r;
            uint8_t a;
        } argb;
    } Color4;
  
    class Colors
    {
    public:
        static const uint32_t Black = 0xFF000000;
        static const uint32_t White = 0xFFFFFFFF;
        static const uint32_t Red   = 0xFFFF0000;
        static const uint32_t Green = 0xFF00FF00;
        static const uint32_t Blue  = 0xFF0000FF;
        static const uint32_t Transparent = 0x00000000;
    public:
        /**
         * Blend to colors using alpha blending
         * Color1 is background
         * Color2 is foreground
        */
        static const uint32_t AlphaBlend(uint32_t color1, uint32_t color2);
        
        /**
         * Convert a ARGB color to 0xAARRGGBB format
        */
        static const uint32_t FromARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
    };
}

#endif