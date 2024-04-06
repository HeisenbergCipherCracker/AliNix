#ifndef _HEISEN_KERNEL_RECT_H
#define _HEISEN_KERNEL_RECT_H
//. LIBHeisenKernel

#include <types.hpp>
#include <list.h>

namespace LIBHeisenKernel
{
    /**
     * A class that describes a rectangular shape
    */
    class Rectangle
    {
    public:
        /**
         * The width of this rectangle
        */
        int width;
        /**
         * The height of this rectangle
        */
        int height;
        /**
         * The x coördinate of this rectangle
        */
        int x;
        /**
         * The y coördinate of this rectangle
        */
        int y;

        /**
         * Create a new instance of the Rectangle Class
        */
        Rectangle(int w, int h, int x = 0, int y = 0);
        
        /**
         * Create a new instance of the Rectangle Class
        */
        Rectangle();

        /**
         * Get the area of this rectangle
         * Basically just width * height
         */
        int Area();

        /**
         * Get the intersection rectangle between this one and the target 
         * Results holds the intersected rect
         * Returns true for a intersection
        */
        bool Intersect(Rectangle other, Rectangle* result);

        /**
         * Does this rect contain the given point?
        */
        bool Contains(int x, int y);

        /**
         * Explode this rect into a list of contiguous rects
        */
        List<Rectangle>* Split(Rectangle cuttingRect, List<Rectangle>* output = 0);

        /**
         * Insert this rectangle into the clip list, splitting all existing rectangles against it to prevent overlap 
        */
        void PushToClipList(List<Rectangle>* targetList);

        bool operator==(const Rectangle& right)
        {
            return (this->width == right.width && this->height == right.height && this->x == right.x && this->y == right.y);
        }

        // Return a rectangle of size 0, like Rectangle(0, 0, 0, 0)
        static Rectangle Zero();
    };
}

#endif // _HEISEN_KERNEL_RECT_H