#pragma once

#include <vector>
#include <fstream>
#include "Vec3.hpp"

/**
 *  Image class to store pixels and write them into file
 */ 
class Image {
  public:
    /**
     *  Standard constructors and destructors
     */
    Image(unsigned int width, unsigned int height): 
      _w(width),
      _h(height),
      _pixels(width * height) {}
    virtual ~Image() {}

    /**
     *  Accesors
     */
    Vec3 &operator()(unsigned int x, unsigned int y) {return _pixels[y * _w + x];}
    Vec3 operator()(unsigned int x, unsigned int y) const {return _pixels[y * _w + x];}
    unsigned int width() const {return _w;}
    unsigned int height() const {return _h;}

    /**
     *  Save the image into a PPM file 
     */
    void writePPM(const std::string &output) {
      std::ofstream os(output);
      os << "P3" << std::endl; // ASCII and RGB convention
      os << width() << " " << height() << std::endl;
      os << 255 << std::endl; // max color value
      for (auto &p: _pixels) {
        os << int(p[0]) << " " << int(p[1]) << " " << int(p[2]) << std::endl;
      }
    }
  private:
    unsigned int _w;
    unsigned int _h;
    std::vector<Vec3> _pixels;
};