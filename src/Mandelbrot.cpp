#include "Mandelbrot.h"

using namespace cv;

Mandelbrot::Mandelbrot(double cx, double cy, int width, int height)
    : _cx{cx},
      _cy{cy},
      _width{width},
      _height{height},
      _image{std::make_shared<cv::Mat>(height, width, CV_8UC3,
                                       cv::Scalar(0, 0, 0))},
      _windowName{"Mandelbrot Fractal"},
      _offsetX{2.125},
      _offsetY{1.5},
      _scaleFactor{1},
      _maxIter{40},
      _divergenceLimit{2},
      _nCores{std::thread::hardware_concurrency()},
      _vec{std::make_shared<std::vector<std::vector<int>>>(
          height * width, std::vector<int>(3, 0))} {
  _x1 = _cx - _offsetX;
  _x2 = _cx + _offsetX - 0.75;
  _y1 = _cy - _offsetY;
  _y2 = _cy + _offsetY;
  _linspaceX = (_x2 - _x1) / (_width - 1);
  _linspaceY = (_y2 - _y1) / (_height - 1);
  cv::namedWindow(_windowName);
  std::cout << "The accepted resolution is : " << _width << "*" << _height
            << std::endl;
  vecSeg();
  draw();
}

Mandelbrot::~Mandelbrot() { std::cout << "Mandelbrot destructor called!\n"; }

void Mandelbrot::fillVecColor(
    std::shared_ptr<std::vector<std::vector<int> *>> vec) {
  std::lock(_mtex1, _mtex2);
  std::lock_guard<std::mutex> lck1(_mtex1, std::adopt_lock);
  std::lock_guard<std::mutex> lck2(_mtex2, std::adopt_lock);

  int i{}, j{};
  double x{}, y{};
  for (std::vector<int> *index : *vec) {
    i = index->at(0);
    j = index->at(1);
    x = _x1 + i * _linspaceX;
    y = _y1 + j * _linspaceY;
    *index = {i, j, iterPixel(std::complex<double>(x, y)), 0, 0};
  }
}

void Mandelbrot::vecSeg() {
  int n{getCores()};

  /// Devide _vec into subvectors and pass them to other threads.
  for (int j{0}; j < n; ++j) {
    std::shared_ptr<std::vector<std::vector<int> *>> I{
        std::make_shared<std::vector<std::vector<int> *>>()};
    for (int k{j * _height / n}; k < (j + 1) * _height / n; ++k) {
      for (int i{0}; i < _width; ++i) {
        _vec->at(i + (k * _width)).at(0) = i;
        _vec->at(i + (k * _width)).at(1) = k;
        I->push_back(&_vec->at(i + (k * _width)));
      }
    }
    // fillVecColor(I);
    _ftrs.emplace_back(
        std::async(std::launch::async, &Mandelbrot::fillVecColor, this, I));
  }
  futuresWait();
}

short Mandelbrot::iterPixel(std::complex<double> c) {
  std::complex<double> z(0, 0);
  _iterations = 0;
  while (abs(z) <= _divergenceLimit && _iterations < _maxIter) {
    ++_iterations;
    z = z * z + c;
  }
  return _iterations;
}

int Mandelbrot::getCores() { return _nCores; }

void Mandelbrot::draw() {
  for (int addr{0}; addr < _width * _height; addr++) {
    cv::Vec3b *_ptr = _image->ptr<cv::Vec3b>(_vec->at(addr).at(1));
    short m{};
    m = _vec->at(addr).at(2);
    cv::Vec3f HSV{0, 0, 0};
    HSV[0] = static_cast<float>(255 * m) / _maxIter;
    HSV[1] = 255;
    HSV[2] = (m < _maxIter) ? 255 : 0;

    cv::Vec3b BGR{HSVtoBGR(HSV)};

    _ptr[_vec->at(addr).at(0)] = BGR;
    // _image->at<cv::Vec3b>(_vec->at(addr).at(1), _vec->at(addr).at(0)) = BGR;
  }
}

void Mandelbrot::showImage() {
  cv::Point point;
  int k{cv::waitKey(1)}, zoom{2};
  while (char(k != 'q')) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    if (_image->empty()) {
      std::cout << "Could not open the image" << std::endl;
      return;
    }
    cv::setMouseCallback(_windowName, onMouse, &_pix);

    k = cv::waitKey(1);
    if (char(k == 'i')) {
      updateImageIn(zoom);

    } else if (char(k == 'o')) {
      updateImageOut(zoom);
    }
    cv::imshow(_windowName, *_image);
  }
  closeImage();
}

void Mandelbrot::onMouse(int event, int x, int y, int flags, void *userdata) {
  if (event == cv::EVENT_MOUSEMOVE) {
    cv::Point *p = static_cast<cv::Point *>(userdata);
    p->x = x;
    p->y = y;
  }
}

void Mandelbrot::closeImage() { cv::destroyWindow(_windowName); }

cv::Vec3b Mandelbrot::HSVtoBGR(const cv::Vec3f &hsv) {
  cv::Mat_<cv::Vec3f> hsvMat(hsv);
  cv::Mat_<cv::Vec3f> bgrMat;

  cv::cvtColor(hsvMat, bgrMat, cv::COLOR_HSV2BGR);

  bgrMat *= 255;  // Upscale after conversion

  // Conversion to Vec3b is handled by OpenCV, no need to static_cast
  return bgrMat(0, 0);
}

void Mandelbrot::updateImageIn(int &zoom) {
  _cx = _x1 + (_x2 - _x1) * static_cast<double>(_pix.x) / _width;
  _cy = _y1 + (_y2 - _y1) * static_cast<double>(_pix.y) / _height;
  _scaleFactor /= zoom;
  std::cout << "Zoom in\n";
  std::cout << "scale : " << _scaleFactor << std::endl;
  std::cout << "center : (" << _cx << ", " << _cy << ")" << std::endl;
  _x1 = _cx - (_offsetX * _scaleFactor);
  _x2 = _cx + ((_offsetX - 0.75) * _scaleFactor);
  _y1 = _cy - (_offsetY * _scaleFactor);
  _y2 = _cy + (_offsetY * _scaleFactor);
  _linspaceX = (_x2 - _x1) / (_width - 1);
  _linspaceY = (_y2 - _y1) / (_height - 1);
  if (_maxIter <= 350) _maxIter += 10;
  vecSeg();
  draw();
  std::cout << "##################################\n";
}

void Mandelbrot::updateImageOut(int &zoom) {
  if (_scaleFactor < 1) {
    _scaleFactor *= zoom;
    std::cout << "Zoom out\n";
    std::cout << "scale : " << _scaleFactor << std::endl;
    std::cout << "center : (" << _cx << ", " << _cy << ")" << std::endl;
    if (_scaleFactor == 1) {
      _cx = 0.0;
      _cy = 0.0;
    } else {
      _cx = _x1 + (_x2 - _x1) * static_cast<double>(_pix.x) / _width;
      _cy = _y1 + (_y2 - _y1) * static_cast<double>(_pix.y) / _height;
    }
    _x1 = _cx - (_offsetX * _scaleFactor);
    _x2 = _cx - (0.75 * (_scaleFactor == 1)) + (_offsetX * _scaleFactor);
    _y1 = _cy - (_offsetY * _scaleFactor);
    _y2 = _cy + (_offsetY * _scaleFactor);
    _linspaceX = (_x2 - _x1) / (_width - 1);
    _linspaceY = (_y2 - _y1) / (_height - 1);
    if (_maxIter > 10 && _maxIter <= 360) _maxIter -= 10;

    vecSeg();
    draw();
    std::cout << "##################################\n";
  }
}

void Mandelbrot::futuresWait() {
  std::for_each(_ftrs.begin(), _ftrs.end(),
                [](std::future<void> &ftr) { ftr.wait(); });
}