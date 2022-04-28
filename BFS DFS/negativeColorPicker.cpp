/*
*  File: negativeColorPicker.cpp
*  Implements the negative color picker for CPSC 221 PA2.
*
*/

#include "negativeColorPicker.h"

NegativeColorPicker::NegativeColorPicker(PNG& inputimg)
{
  // complete your implementation below
  img = inputimg;
}

HSLAPixel NegativeColorPicker::operator()(PixelPoint p)
{
  int one = 1;
  double o = (double) one;

  int circle = 360;
  double c = (double) circle;

  int eighty = 180;
  double oe = (double) eighty;

  HSLAPixel* color = img.getPixel(p.x, p.y);
  if (color->h < oe) {
    color->h = color->h + eighty;
  } else {
    color->h = (color->h + eighty) - c;
  }
  color->l = (o - color->l);
  return *color;
}
