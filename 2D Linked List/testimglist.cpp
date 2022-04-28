// File:        testimglist.cpp
// Date:        2022-01-24 11:35
// Description: Contains basic tests for ImgList class
//              for CPSC 221 2021W2 PA1
//              You may add your own additional tests

#include "imglist.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#else
#define _ASSERT(expr) ((void)0)

#define _ASSERTE(expr) ((void)0)
#endif
#include <iostream>

using namespace std;
using namespace cs221util;

// test function definitions
void TestConstructorRender();
void TestCarveMode0();
void TestRenderMode2();

int main(void) {

  // call the various test functions
  // you may comment and uncomment these as needed
  TestConstructorRender();
  TestCarveMode0();
  TestRenderMode2();

  return 0;
}

void TestConstructorRender() {
  cout << "Entered TestConstructorRender function" << endl;

  PNG inimg;
  inimg.readFromFile("input-images/3x4.png");

  // PNG myfancypng;
  // myfancypng.resize(2, 1);

  // HSLAPixel *somepixel;

  //change the HSL colour of the northwest pixel
  // somepixel = myfancypng.getPixel(0, 0);
  // *somepixel = HSLAPixel(30, 0.25, 0.75);

  // somepixel = myfancypng.getPixel(0, 1);
  // *somepixel = HSLAPixel(120, 0.7, 0.4);

  // // somepixel = myfancypng.getPixel(0, 2);
  // // *somepixel = HSLAPixel(40, 0.5, 0.2);

  // //now change the HSL colour of the northeast pixel
  // somepixel = myfancypng.getPixel(1, 0);
  // *somepixel = HSLAPixel(60, 0.5, 0.25);

  // somepixel = myfancypng.getPixel(1, 1);
  // *somepixel = HSLAPixel(240, 0.1, 0.8);

  // somepixel = myfancypng.getPixel(1, 2);
  // *somepixel = HSLAPixel(200, 0.7, 0.5);

  // and the other pixels
  // somepixel = myfancypng.getPixel(2, 0);
  // *somepixel = HSLAPixel(100, 0.4, 0.4);

  // somepixel = myfancypng.getPixel(2, 1);
  // *somepixel = HSLAPixel(180, 0.6, 0.2);

  // somepixel = myfancypng.getPixel(2, 2);
  // *somepixel = HSLAPixel(80, 0.5, 0.9);

  // if (myfancypng.height() == 1 && myfancypng.width() == 2) {
  //   cout << "Input PNG 1x2.png successfully read." << endl;
  //   cout << "Creating ImgList object using input PNG... ";
  //   ImgList list2x1(myfancypng);
  //   cout << "done." << endl;

  //   cout << "Attempting to render unmodified list data to output PNG... ";
  //   PNG outputimg = list2x1.Render(false, 0);
  //   cout << "done." << endl;

  //   cout << "Writing output PNG to 3x4-0-0.png... ";
  //   outputimg.writeToFile("output-images/2x2-0-0.png");
  //   cout << "done." << endl;

  //   cout << "Exiting TestConstructorRender function" << endl;
  // }
  // else {
  //   cout << "Input PNG image not read. Exiting..." << endl;
  // }

  if (inimg.height() == 4 && inimg.width() == 3) {
    cout << "Input PNG 3x4.png successfully read." << endl;
    cout << "Creating ImgList object using input PNG... ";
    ImgList list3x4(inimg);
    cout << "done." << endl;

    cout << "Attempting to render unmodified list data to output PNG... ";
    PNG outputimg = list3x4.Render(false, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 3x4-0-0.png... ";
    outputimg.writeToFile("output-images/3x4-0-0.png");
    cout << "done." << endl;

    //Test
    PNG solnimg;
    solnimg.readFromFile("output-images/soln-3x4-0-0.png");
    cout << (outputimg == solnimg) << endl;
    outputimg.readFromFile("output-images/3x4-0-0.png");
    cout << (outputimg == solnimg) << endl;

    cout << "Exiting TestConstructorRender function" << endl;
  }
  else {
    cout << "Input PNG image not read. Exiting..." << endl;
  }
}

void TestCarveMode0() {
  cout << "Entered TestCarveMode0 function" << endl;

  PNG inimg;
  inimg.readFromFile("input-images/5x6.png");

  if (inimg.height() == 6 && inimg.width() == 5) {
    cout << "Input PNG 3x4.png successfully read." << endl;
    cout << "Creating ImgList object using input PNG... ";
    ImgList list5x6(inimg);
    cout << "done." << endl;

    cout << "Attempting to carve list using selection mode 0... ";
    list5x6.Carve(1, 0);
    cout << "done." << endl;

    cout << "Determining physical node dimensions of carved image... ";
    unsigned int postcarvex = list5x6.GetDimensionX();
    unsigned int postcarvey = list5x6.GetDimensionY();
    cout << "done." << endl;

    cout << "Expected horizontal dimension: 4" << endl;
    cout << "Your horizontal dimension: " << postcarvex << endl;

    cout << "Rendering carved image using rendermode 0... ";
    PNG outputimg1 = list5x6.Render(false, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-0-0.png... ";
    outputimg1.writeToFile("output-images/5x6-0-0.png");
    cout << "done." << endl;

    //Test
    PNG solnimg1;
    solnimg1.readFromFile("output-images/soln-5x6-0-0.png");
    cout << (outputimg1 == solnimg1) << endl;
    outputimg1.readFromFile("output-images/5x6-0-0.png");
    cout << (outputimg1 == solnimg1) << endl;

    cout << "Rendering carved image using rendermode 1 and fillmode 0... ";
    PNG outputimg2 = list5x6.Render(true, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-1-0.png... ";
    outputimg2.writeToFile("output-images/5x6-1-0.png");
    cout << "done." << endl;

    //Test
    PNG solnimg2;
    solnimg2.readFromFile("output-images/soln-5x6-1-0.png");
    cout << (outputimg2 == solnimg2) << endl;
    outputimg2.readFromFile("output-images/5x6-1-0.png");
    cout << (outputimg2 == solnimg2) << endl;

    cout << "Rendering carved image using rendermode 1 and fillmode 1... ";
    PNG outputimg3 = list5x6.Render(true, 1);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-1-1.png... ";
    outputimg3.writeToFile("output-images/5x6-1-1.png");
    cout << "done." << endl;

    //Test
    PNG solnimg3;
    solnimg3.readFromFile("output-images/soln-5x6-1-1.png");
    cout << (outputimg3 == solnimg3) << endl;
    outputimg3.readFromFile("output-images/5x6-1-1.png");
    cout << (outputimg3 == solnimg3) << endl;

    cout << "Carving a second pixel from image... ";
    list5x6.Carve(1, 0);
    cout << "done." << endl;

    cout << "Determining physical node dimensions of carved image... ";
    postcarvex = list5x6.GetDimensionX();
    postcarvey = list5x6.GetDimensionY();
    cout << "done." << endl;

    cout << "Expected horizontal dimension: 3" << endl;
    cout << "Your horizontal dimension: " << postcarvex << endl;

    cout << "Rendering carved image using rendermode 0... ";
    PNG outputimg4 = list5x6.Render(false, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-0-0.png... ";
    outputimg4.writeToFile("output-images/5x6-0-0-c2.png");
    cout << "done." << endl;

    //Test
    PNG solnimg4;
    solnimg4.readFromFile("output-images/soln-5x6-0-0-c2.png");
    cout << (outputimg4 == solnimg4) << endl;
    outputimg4.readFromFile("output-images/5x6-0-0-c2.png");
    cout << (outputimg4 == solnimg4) << endl;

    cout << "Rendering carved image using rendermode 1 and fillmode 2... ";
    PNG outputimg5 = list5x6.Render(true, 2);
    cout << "done." << endl;

    cout << "Writing output PNG to 5x6-1-2.png... ";
    outputimg5.writeToFile("output-images/5x6-1-2.png");
    cout << "done." << endl;

    //Test
    PNG solnimg5;
    solnimg5.readFromFile("output-images/soln-5x6-1-2.png");
    cout << (outputimg5 == solnimg5) << endl;
    outputimg5.readFromFile("output-images/5x6-1-2.png");
    cout << (outputimg5 == solnimg5) << endl;

    cout << "Exiting TestCarveMode0 function" << endl;
  }
  else {
    cout << "Input PNG image not read. Exiting..." << endl;
  }
}

void TestRenderMode2() {
  cout << "Entered TestRenderMode2 function" << endl;

  PNG inimg;
  inimg.readFromFile("input-images/6x6grad.png");

  if (inimg.height() == 6 && inimg.width() == 6) {
    cout << "Input PNG 6x6.png successfully read." << endl;
    cout << "Creating ImgList object using input PNG... ";
    ImgList list6x6(inimg);
    cout << "done." << endl;

    cout << "Attempting to carve more pixels than allowable in mode 0... ";
    list6x6.Carve(12, 0);
    cout << "done." << endl;

    cout << "Determining physical node dimensions of carved image... ";
    unsigned int postcarvex = list6x6.GetDimensionX();
    unsigned int postcarvey = list6x6.GetDimensionY();
    cout << "done." << endl;

    cout << "Expected horizontal dimension: 2" << endl;
    cout << "Your horizontal dimension: " << postcarvex << endl;

    cout << "Rendering carved image using rendermode 0... ";
    PNG outputimg1 = list6x6.Render(false, 0);
    cout << "done." << endl;

    cout << "Writing output PNG to 6x6-0-0.png... ";
    outputimg1.writeToFile("output-images/6x6-0-0.png");
    cout << "done." << endl;

    //Test
    PNG solnimg1;
    solnimg1.readFromFile("output-images/soln-6x6-0-0.png");
    cout << (outputimg1 == solnimg1) << endl;
    outputimg1.readFromFile("output-images/6x6-0-0.png");
    cout << (outputimg1 == solnimg1) << endl;

    cout << "Rendering carved image using rendermode 1 and fillmode 2... ";
    PNG outputimg2 = list6x6.Render(true, 2);
    cout << "done." << endl;

    cout << "Writing output PNG to 6x6-1-2.png... ";
    outputimg2.writeToFile("output-images/6x6-1-2.png");
    cout << "done." << endl;

    //Test
    PNG solnimg2;
    solnimg2.readFromFile("output-images/soln-6x6-1-2.png");
    cout << (outputimg2 == solnimg2) << endl;
    outputimg2.readFromFile("output-images/6x6-1-2.png");
    cout << (outputimg2 == solnimg2) << endl;

    cout << "Exiting TestRenderMode2 function" << endl;
  }
  else {
    cout << "Input PNG image not read. Exiting..." << endl;
  }
}