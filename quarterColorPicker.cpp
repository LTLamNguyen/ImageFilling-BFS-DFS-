/**
 * @file quarterColorPicker.cpp
 * @description Implements the QuarterColorPicker constructor and () function
 *              for CPSC 221 PA2
 *
 *              THIS FILE WILL BE SUBMITTED
 */

#include "quarterColorPicker.h"
#include <cmath>


QuarterColorPicker::QuarterColorPicker(PNG& inputimg, unsigned char b_amount)
    // : referenceimg(inputimg), brightamount(b_amount)
{
    // Complete your implementation below
	referenceimg = inputimg;
    brightamount = b_amount;
}

/**
 * Picks the color for pixel (x, y).
 *
 * Using the private reference image, scale each dimension by half
 * and tile the smaller image in a 2 x 2 grid over the original dimensions
 * of the image, and return the pixel at the appropriate coordinate from
 * the tiled image, brightened on each R/G/B channel by the required amount.
 * 
 * The value of each pixel in the scaled image will be a bilinear interpolation
 * of a 2x2 pixel region from the original image, with each of the R/G/B/A channels
 * processed individually. Interpolate over the x-axis before the y-axis.
 * Truncate any fractional values on the R/G/B channels. Brighten each color channel
 * as the final step.
 * 
 * @pre referenceimg width and height are both even
 */
RGBAPixel QuarterColorPicker::operator()(PixelPoint p)
{
    // Replace the line below with your implementation

    unsigned int quarterWidth = referenceimg.width() / 2;
    unsigned int quarterHeight = referenceimg.height() / 2;
    unsigned int tileX = p.x % quarterWidth;
    unsigned int tileY = p.y % quarterHeight;

    // Scale these coordinates up to the original image's scale for bilinear interpolation
    double originalX = tileX * 2.0;
    double originalY = tileY * 2.0;

    // Identify the four pixels from the original image that we'll use for interpolation
    unsigned int x1 = static_cast<unsigned int>(originalX);
    unsigned int y1 = static_cast<unsigned int>(originalY);
    unsigned int x2 = std::min(x1 + 1, referenceimg.width() - 1);
    unsigned int y2 = std::min(y1 + 1, referenceimg.height() - 1);


    RGBAPixel p11 = *referenceimg.getPixel(x1, y1);
    RGBAPixel p12 = *referenceimg.getPixel(x2, y1);
    RGBAPixel p21 = *referenceimg.getPixel(x1, y2);
    RGBAPixel p22 = *referenceimg.getPixel(x2, y2);

    auto average = [&](unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
        return static_cast<unsigned char>((a + b + c + d) / 4);
    };

    unsigned char r = average(p11.r, p12.r, p21.r, p22.r);
    unsigned char g = average(p11.g, p12.g, p21.g, p22.g);
    unsigned char b = average(p11.b, p12.b, p21.b, p22.b);
    unsigned char a = average(p11.a, p12.a, p21.a, p22.a);

    // Brighten each color channel by the specified amount, clamping to 255
    auto brighten = [&](unsigned char color) -> unsigned char {
        int newColor = color + brightamount;
        return static_cast<unsigned char>(std::min(255, newColor));
    };

    r = brighten(r);
    g = brighten(g);
    b = brighten(b);
    return RGBAPixel(r, g, b, a);

}
