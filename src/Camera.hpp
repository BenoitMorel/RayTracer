#pragma once

#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <random>
#include "Common.hpp"
#include "Shape.hpp"
#include "Image.hpp"

/**
 * Caracteristics of  the camera (position, orientation etc.) and
 * raycasting
*/
class Camera {
  public:
    /**
     * Constructor
    */
    Camera(double aspectRatio = 1.5, unsigned int imageWidth = 10): _aspectRatio(aspectRatio),
      _focal(1.0),
      _raysPerPixel(10),
      _imageWidth(imageWidth),
      _vpWidth(2.0),
      _center(0.0, 0.0, -1.0),
      _lookAt(0.0, 0.0, 0.0),
      _lookUp(0.0, 1.0, 0.0)
    {


    }

    void render(const Shape &world) {
      _updateParameters();
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::uniform_real_distribution<double> dis(0.0, 1.0);
      unsigned int hits = 0; 
      Image image(_imageWidth, _imageHeight);
      for (unsigned int x = 0; x < _imageWidth; ++x) {
        for (unsigned int y = 0; y < _imageHeight; ++y) {
          Vec3 averageColor;
          for (unsigned int it = 0; it < _raysPerPixel; ++it) {
            double rightFactor = static_cast<double>(x) + dis(gen) - 0.5;
            double downFactor = static_cast<double>(y) + dis(gen) - 0.5;
            auto cell = _vpCorner + _cellOffsetRight * rightFactor + _cellOffsetDown * downFactor;
            Ray ray(_center, cell - _center);
            Vec3 color(static_cast<double>(x) / static_cast<double>(_imageWidth), 
              static_cast<double>(y) / static_cast<double>(_imageHeight),
              0.1); 
            Hit hit;
            if (world.hit(ray, hit)) {
              color = Vec3(1.0, 1.0, 1.0);
              color = hit.normal;
              color[0] += 1.0;
              color[1] += 1.0;
              color[2] += 1.0;
              color *= 0.5;
              hits += 1;
            }
            color *= 256.0;
            averageColor += color;
          }
          averageColor /= double(_raysPerPixel);
          image(x, y) = averageColor;
        }
      }
      std::cout << double(hits) / double(image.width() * image.height() * _raysPerPixel) << " hit ratio " << std::endl;
      // save
      std::string output = "C:\\Users\\benom\\github\\RayTracer\\src\\output.ppm";
      std::cout << "Output in " << output << std::endl;
      image.writePPM(output);
    }

  private:
    /**
     * Update the different parameters of the camera before rendering
    */
    void _updateParameters() {
      _imageHeight = static_cast<unsigned int>(static_cast<double>(_imageWidth) / _aspectRatio);
      _vpHeight = _vpWidth / _aspectRatio;
      Vec3 camDirection = (_lookAt - _center).getNormalized();
      _cellOffsetRight = (camDirection ^ _lookUp) * (_vpWidth / static_cast<double>(_imageWidth));
      _cellOffsetDown = _lookUp * (-_vpHeight / static_cast<double>(_imageHeight));
      _vpCorner = _lookAt
        + _cellOffsetRight * (-static_cast<double>(_imageWidth) / 2.0)
        + _cellOffsetDown * (-static_cast<double>(_imageHeight) / 2.0);
    }



  private:
    double _aspectRatio; // width / height
    double _focal; // focal length
    unsigned int _raysPerPixel;// number of rays per pixels for anti-aliasing
    unsigned int _imageWidth; // image width in pixels
    unsigned int _imageHeight; // image height in pixels
    Vec3 _center; // center of the camera
    Vec3 _lookAt; // point to which the camera points
    Vec3 _lookUp; // up orientation of the camera
    double _vpWidth; // viewport width
    double _vpHeight; // viewport height
    Vec3 _cellOffsetRight; // offset of one window cell (pixel) to the right
    Vec3 _cellOffsetDown; // offset of one window cell (pixel) below
    Vec3 _vpCorner; // position of the top left corner of the viewport 
};
