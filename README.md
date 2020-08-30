# CPPND: Capstone Mandelbrot Creator
<img src="data/Mandelbrot_Creator.png"/>

This project consists of developing code to draw and explore a certain kind of fractal called a Mandelbrot set.

## Mandelbrot Set
Benoit Mandelbrot was a Polish born, French/American mathematician who spent most of his career at the IBM research center in New York. He is credited for coining the term “fractal” and developed a theory of `roughness`/`self- similarity`/`fractals`/`chaos`. His book [The Fractal Geometry of Nature](https://www.amazon.com/Fractal-Geometry-Nature-Benoit-Mandelbrot/dp/0716711869) was published in 1982 and was quite influential at the time (this was when computer graphics were just becoming widely available). One of his inventions/discoveries, the Mandelbrot set has stimulated significant mathematical research and is popularly used to generate interesting visual images (and also test the limits of hardware!). 

To understand the Mandelbrot set, let’s first consider what happens when we take a region in the complex plane, and then repeatedly square each point

<img src="https://render.githubusercontent.com/render/math?math=z_{k+1} = z_{k}^{2},\hspace{1cm}    k = 0,1,...">

For which initial values <img src="https://render.githubusercontent.com/render/math?math=z_{0}"> does the sequence <img src="https://render.githubusercontent.com/render/math?math={z_{k}}_z_{k} \ge 0">

<!-- In this project we are going to draw Mandelbrot fractal using the object oriented features of C++ and its ability to perform concurrently. OpenCV is used to implement this project and showing the result in form of an image which is able to zoom-in and zoom-out in all places the pointer points to in the frame.  -->

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* OpenCV >= 4.1
  * The OpenCV 4.1.0 source code can be found [here](https://github.com/opencv/opencv/tree/4.1.0)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Capstone`.

or simply:

0. You can only use below commands:
* `make debug` for Debug.
* `make build` to build the project.
* `make clean` to remove the build directory.
* `make style` to specify the format you want your code be like.
  * You will need to install `clang-format` first in case it is not installed. You only need to use it once.
  * In macOS you to have `Homebrew` installed. The instructions on how to install, can be found [here](https://brew.sh). Then you can `brew install clang-format`.
  * In Ubuntu or Debian you can `sudo apt install clang-format`.

* `make format` to use the settings of the `style` which in this project is set by default to `-style=google`.
* `make all` to perform almost all the steps above at once.

## Operations
In order to zoom-in or zoom-out the image, you will need to put the mouse pointer on the place you like in the frame.
1. Press button `i` to zoom-in.
2. Press button `o` to zoom-out.


