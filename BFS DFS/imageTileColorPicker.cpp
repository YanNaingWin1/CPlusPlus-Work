/*
*  File: imageTileColorPicker.cpp
*  Implements the image tile color picker for CPSC 221 PA2.
*
*/

#include "imageTileColorPicker.h"

ImageTileColorPicker::ImageTileColorPicker(PNG& otherimage) {
  // complete your implementation below
  img_other = otherimage;
}

HSLAPixel ImageTileColorPicker::operator()(PixelPoint p) {
  // complete your implementation below
  unsigned int point_x = p.x % img_other.width();
  unsigned int point_y = p.y % img_other.height();
  HSLAPixel* pixel = img_other.getPixel(point_x, point_y);
  return *pixel; // REPLACE THIS STUB
}