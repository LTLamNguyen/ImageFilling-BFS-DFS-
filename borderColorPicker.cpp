/**
 * @file borderColorPicker.cpp
 * @description Implements the BorderColorPicker constructor and () function
 *              for CPSC 221 PA2
 *
 *              THIS FILE WILL BE SUBMITTED
 */

#include "borderColorPicker.h"
#include <algorithm>

/**
 * Constructs a new BorderColorPicker.
 *
 * @param width        Number of pixels to color around border.
 * @param inputimage   Image for referencing original image colors.
 * @param bcolor       Color to be used for painting the fill border.
 * @param tol          Tolerance used to determine the border of the fill.
 */
BorderColorPicker::BorderColorPicker(unsigned int width, PNG& inputimage, RGBAPixel scolor, RGBAPixel bcolor, double tol)
    // : borderwidth(width), img(inputimage), seedcolor(scolor), bordercolor(bcolor), tolerance(tol)
{
    // Complete your implementation below
	borderwidth = width;
    img = inputimage;
    seedcolor = scolor;
    bordercolor = bcolor;
    tolerance = tol;
}

/**
 * Picks the color for pixel at point. If the x or y coordinate is
 * at or within an Euclidean radius of borderwidth pixels from the border of
 * the fill region, it will be colored with the required border color.
 * Otherwise, its color will not be changed.
 *
 * @param p   The point for which you're picking a color.
 * @return    The color chosen for (p).
 */
RGBAPixel BorderColorPicker::operator()(PixelPoint p)
{
    // Replace the line below with your implementation

    /* 
    //Code 1:
    RGBAPixel originalPixel = *img.getPixel(p.x, p.y);
    // Assuming distanceTo method exists and returns a double representing the squared color distance.
    double pixelDist = originalPixel.distanceTo(seedcolor); 

    // If the original pixel color is not within the tolerance of the seed color, return its original color.
    if (pixelDist > tolerance) {
        return originalPixel;
    }

    // Check if the pixel is at or near the border of the image.
    if (p.x < borderwidth || p.y < borderwidth ||
        p.x >= img.width() - borderwidth || p.y >= img.height() - borderwidth) {
        return bordercolor;
    }

    // Check every point within the borderwidth radius.
    for (int dy = -static_cast<int>(borderwidth); dy <= static_cast<int>(borderwidth); ++dy) {
        for (int dx = -static_cast<int>(borderwidth); dx <= static_cast<int>(borderwidth); ++dx) {
            unsigned int nx = static_cast<unsigned int>(max(0, static_cast<int>(p.x) + dx));
            unsigned int ny = static_cast<unsigned int>(max(0, static_cast<int>(p.y) + dy));

            // If the point is outside the image boundaries, it's part of the border.
            if (nx >= img.width() || ny >= img.height()) {
                return bordercolor;
            }

            RGBAPixel neighborPixel = *img.getPixel(nx, ny);
            double neighborDist = neighborPixel.distanceTo(seedcolor);

            // If any neighbor's distance to seedcolor is greater than tolerance, it's a border.
            if (neighborDist > tolerance) {
                return bordercolor;
            }
        }
    }

    // If none of the above conditions are met, return the original color of the pixel.
    return originalPixel;

    End of Code 1*/

    /* 
    //Code 2:
    // Check if the pixel itself is within the tolerance of the seed color
    if (p.color.distanceTo(seedcolor) > tolerance) {
        return p.color; // Return its original color
    }

    // Calculate the search area boundaries safely to avoid underflow and overflow
    int startX = std::max(0, static_cast<int>(p.x) - static_cast<int>(borderwidth));
    int startY = std::max(0, static_cast<int>(p.y) - static_cast<int>(borderwidth));
    int endX = std::min(static_cast<int>(img.width()) - 1, static_cast<int>(p.x) + static_cast<int>(borderwidth));
    int endY = std::min(static_cast<int>(img.height()) - 1, static_cast<int>(p.y) + static_cast<int>(borderwidth));

    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            unsigned int squaredDistance = static_cast<unsigned int>((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
            if (squaredDistance <= borderwidth * borderwidth) {
                RGBAPixel* currentPixel = img.getPixel(x, y);
                if (currentPixel->distanceTo(seedcolor) > tolerance) {
                    return bordercolor; // Pixel is near the border
                }
            }
        }
    }

    // If none of the conditions are met, return the original color
    return p.color;
    End of Code 2*/


    RGBAPixel originalPixel = *img.getPixel(p.x, p.y);
    double pixelDist = originalPixel.distanceTo(seedcolor);

    // Check if the pixel's color is not within the tolerance of the seed color
    if (pixelDist > tolerance) {
        return originalPixel; // Return the original color
    }

    // Directly check if the pixel is on the perimeter of the image
    if (p.x < borderwidth || p.y < borderwidth ||
        p.x >= img.width() - borderwidth || p.y >= img.height() - borderwidth) {
        return bordercolor; // Pixel is near the image border
    }

    // Check if the pixel is near a border within the fill region
    for (int dy = -static_cast<int>(borderwidth); dy <= static_cast<int>(borderwidth); ++dy) {
        for (int dx = -static_cast<int>(borderwidth); dx <= static_cast<int>(borderwidth); ++dx) {
            unsigned int nx = p.x + dx;
            unsigned int ny = p.y + dy;

            // Skip checks if nx or ny go out of bounds (negative or beyond width/height)
            if (nx >= img.width() || ny >= img.height() || nx < 0 || ny < 0) {
                continue; 
            }

            // Compute the squared Euclidean distance from (p.x, p.y) to (nx, ny)
            unsigned int squaredDistance = (nx - p.x) * (nx - p.x) + (ny - p.y) * (ny - p.y);
            if (squaredDistance <= borderwidth * borderwidth) {
                RGBAPixel neighborPixel = *img.getPixel(nx, ny);

                // If a neighbor pixel's color is outside the tolerance, it's a border pixel
                if (neighborPixel.distanceTo(seedcolor) > tolerance) {
                    return bordercolor; // Pixel is near the border within the fill region
                }
            }
        }
    }

    // If the pixel is neither on the perimeter nor near a border within the fill region, return the original color
    return originalPixel;

}

/**
 * Add your private BorderColorPicker function implementations below
 */
