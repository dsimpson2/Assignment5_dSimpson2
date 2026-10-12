#ifndef PIXEL_H // Include guard to prevent multiple inclusions of this header file
#define PIXEL_H // Define the PIXEL_H macro to indicate that this header file has been included

struct _pixel // Define a struct named _pixel with members for red, green, blue color values and x, y coordinates
{
  float r; // Red color value
  float g; // Green color value
  float b; // Blue color value
  int x;  // X coordinate
  int y;  // Y coordinate
};
typedef struct _pixel Pixel; // Define a struct named _pixel with members for red, green, blue color values and x, y coordinates. Then create a typedef for the struct named Pixel.

#endif