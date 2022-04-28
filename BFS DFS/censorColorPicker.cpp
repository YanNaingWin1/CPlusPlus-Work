/*
*  File: censorColorPicker.cpp
*  Implements the censor color picker for CPSC 221 PA2.
*
*/

#include "censorColorPicker.h"

#include <math.h> // gives access to sqrt function

/*
*  Useful function for computing the Euclidean distance between two PixelPoints
*/
double PointDistance(PixelPoint a, PixelPoint b) {
  unsigned int diff_x, diff_y;
  if (a.x > b.x)
    diff_x = a.x - b.x;
  else
    diff_x = b.x - a.x;
  if (a.y > b.y)
    diff_y = a.y - b.y;
  else
    diff_y = b.y - a.y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

CensorColorPicker::CensorColorPicker(unsigned int b_width, PixelPoint ctr, unsigned int rad, PNG& inputimage)
{
  // complete your implementation below
  blockwidth = b_width;
  center = ctr;
  radius = rad;
  img = inputimage;
  blockyimg = img;
  for (unsigned int y = 0; y < img.height(); y += blockwidth) {
    for (unsigned int x = 0; x < img.width(); x += blockwidth) {
      blockify(&blockyimg, x, y);
    }
  }
}

HSLAPixel CensorColorPicker::operator()(PixelPoint p)
{
  // complete your implementation below
  double dist = PointDistance(p, center);
  HSLAPixel* pixel;
  double rad = (double) radius;
  if (dist <= rad) {
    pixel = blockyimg.getPixel(p.x, p.y);
  } else {
    pixel = img.getPixel(p.x, p.y);
  }
  return *pixel;
}

void CensorColorPicker::blockify(PNG* b_img, unsigned int x_c, unsigned int y_c) {
  double t_h, t_s, t_l, t_a;
  t_h = 0; t_s = 0; t_l = 0; t_a = 0;
  int x_counter, y_counter;
  x_counter = 0; y_counter = 0;
  int temp = 0;

   for (unsigned int y = y_c; y < fmin(y_c + blockwidth, img.height()); y++) {
    for (unsigned int x = x_c; x < fmin(x_c + blockwidth, img.width()); x++) {
      HSLAPixel* pixel = b_img->getPixel(x, y);
      t_h += pixel->h;
      t_s += pixel->s;
      t_l += pixel->l;
      t_a += pixel->a;
      temp++;
    }
    x_counter = temp;
    temp = 0; 
    y_counter++;
  }

  double avg_h, avg_s, avg_l, avg_a;
  double b_width = (double) blockwidth;
  double dim;
  if ((x_counter % blockwidth == 0) && (y_counter % blockwidth == 0)) {
    dim = b_width * b_width;
  } else {
    dim = x_counter * y_counter;
  }
  
  avg_h = t_h / dim;
  avg_s = t_s / dim;
  avg_l = t_l / dim;
  avg_a = t_a / dim;

  for (unsigned int y = y_c; y < fmin(y_c + blockwidth, img.height()); y++) {
    for (unsigned int x = x_c; x < fmin(x_c + blockwidth, img.width()); x++) {
      b_img->getPixel(x,y)->h = avg_h;
      b_img->getPixel(x,y)->s = avg_s;
      b_img->getPixel(x,y)->l = avg_l;
      b_img->getPixel(x,y)->a = avg_a;
    }
  }
}

// void CensorColorPicker::assignAvgColor(PNG* given_img, double h, double s, double l, double a, unsigned int start_val_x, unsigned int start_val_y) {
//   double avg_h, avg_s, avg_l, avg_a;
//   double b_width = (double) blockwidth;
//   double dim = b_width * b_width;
//   avg_h = h / dim;
//   avg_s = s / dim;
//   avg_l = l / dim;
//   avg_a = a / dim;
//   for (unsigned int y = start_val_y; y < fmin(start_val_y + blockwidth, img.height()); y++) {
//     for (unsigned int x = start_val_x; x < fmin(start_val_x + blockwidth, img.width()); x++) {
//       given_img->getPixel(x,y)->h = avg_h;
//       given_img->getPixel(x,y)->s = avg_s;
//       given_img->getPixel(x,y)->l = avg_l;
//       given_img->getPixel(x,y)->a = avg_a;
//     }
//   }
// }