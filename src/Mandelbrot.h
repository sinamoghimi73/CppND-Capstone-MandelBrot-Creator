#ifndef MANDELBROT_H_
#define MANDELBROT_H_

#include <opencv2/core/hal/interface.h>

#include <algorithm>
#include <cmath>
#include <complex>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class Mandelbrot {
public:
  /// Constructor / Destructor.
  Mandelbrot(double cx = 0.0, double cy = 0.0, int width = 2, int height = 2);
  ~Mandelbrot();

  /// instances for the image.
  void fillVecColor(std::shared_ptr<std::vector<std::vector<int> *>> vec);
  cv::Vec3b HSVtoBGR(const cv::Vec3f &hsv);
  void draw();
  void updateImageIn(int &zoom);
  void updateImageOut(int &zoom);
  void showImage();
  void closeImage();

  /// members for the calculation.
  void vecSeg();
  static void onMouse(int event, int x, int y, int flags, void *userdata);
  short iterPixel(std::complex<double> c);
  int getCores();
  void futuresWait();

private:
  /// instances for the image.
  std::shared_ptr<cv::Mat> _image;
  std::shared_ptr<std::vector<std::vector<int>>> _vec;
  std::string _windowName{};
  double _cx{}, _cy{}, _offsetX{}, _offsetY{}, _scaleFactor{};
  double _x1{}, _y1{}, _x2{}, _y2{}, _linspaceX{}, _linspaceY{};
  const int _width{}, _height{};

/// instances for the color conversion.
  short _tempPix;
  cv::Vec3b _pixelColorBGR;
  cv::Vec3f _pixelColorHSV;
  cv::Vec3b *_pixelPtr;

/// instances for getting the mouse cursor coordinates.
  cv::Point _pix;

  /// instances for the calculation.
  unsigned int _nCores;
  std::mutex _mtex1, _mtex2;
  std::vector<std::future<void>> _ftrs;
  short _iterations;
  short _maxIter, _divergenceLimit;
};

#endif