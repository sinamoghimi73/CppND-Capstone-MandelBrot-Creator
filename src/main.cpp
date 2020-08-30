#include <iostream>

#include "Mandelbrot.h"

int main() {
  double center_x{0.0}, center_y{0.0};
  int width{400}, height{360};

  unsigned int nCores{std::thread::hardware_concurrency()};
  if (width % nCores != 0)
    width -= (width % nCores);

  if (height % nCores != 0)
    height -= (height % nCores);

  Mandelbrot sina(center_x, center_y, width, height);

  sina.showImage();

  return 0;
}
