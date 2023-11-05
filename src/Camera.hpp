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

#define PI 3.14159265

/**
 * Caracteristics of  the camera (position, orientation etc.) and
 * raycasting
*/
class Camera {
  public:
    /**
     * Constructor
    */
    Camera(double aspectRatio = 1.5, unsigned int imageWidth = 10, unsigned int raysPerPixel = 10): 
      _aspectRatio(aspectRatio),
      _vfov(130),
      _raysPerPixel(raysPerPixel),
      _imageWidth(imageWidth),
      _lookFrom(-2, 2, 1),
      _lookAt(0.0, 0.0, -1.0),
      _lookUp(0.0, 1.0, 0.0)
    {


    }

    Vec3 getRayColor(const Ray &ray, const Shape &world, unsigned int depth) {
        auto color = Vec3(0.0, 0.0, 0.0);
        if (depth > 5) {
            return color;
        }
        
        Hit hit;
        if (world.hit(ray, hit)) {
            const bool diffuse = true;
            if (diffuse) {
                auto newDirection = Vec3::getRandomUnitVector();
                if (newDirection * hit.normal < 0) {
                    newDirection = newDirection * -1.0;
                }
                Ray newRay(hit.point, newDirection);
                return getRayColor(newRay, world, depth + 1) * 0.5;
            } else {
                color = hit.normal;
                color[0] += 1.0;
                color[1] += 1.0;
                color[2] += 1.0;
                color *= 0.5;
            } 
        } else {
            auto t = 0.5 * (ray.direction()[1] + 1.0);
            color = Vec3(1.0, 1.0, 1.0) * (1.0-t) + Vec3(0.5, 0.5, 1.) * t;
            //color = Vec3(0.5, 0.5, 1.);
        }
        return color;
    }


    Ray getRay(unsigned int x, unsigned int y) {
      std::random_device rd; 
      std::mt19937 gen(rd()); 
      std::uniform_real_distribution<double> dis(0.0, 1.0);
      double rightFactor = static_cast<double>(x) + dis(gen) - 0.5;
      double downFactor = static_cast<double>(y) + dis(gen) - 0.5;
      if (_raysPerPixel == 1) {
        rightFactor = static_cast<double>(x);
        downFactor = static_cast<double>(y);
      }
      auto cell = _vpCorner + _cellOffsetRight * rightFactor + _cellOffsetDown * downFactor;
      return Ray(_lookFrom, cell - _lookFrom);
    }

    void render(const Shape &world) {
      _updateParameters();
      unsigned int hits = 0; 
      Image image(_imageWidth, _imageHeight);
      for (unsigned int x = 0; x < _imageWidth; ++x) {
        for (unsigned int y = 0; y < _imageHeight; ++y) {
          Vec3 averageColor;
          for (unsigned int it = 0; it < _raysPerPixel; ++it) {
            auto ray = getRay(x, y);
            averageColor += getRayColor(ray, world, 0);
          }
          averageColor *= 255.0 / double(_raysPerPixel);
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
      auto theta = _vfov * PI / 180.0;
      auto focal = (_lookAt - _lookFrom).norm();
      _vpHeight = 2 * tan(theta / 2.0) * focal;
      _vpWidth = _vpHeight * _aspectRatio;
      std::cout << _vpWidth << " " << _vpHeight << std::endl;

      auto w = (_lookFrom - _lookAt).getNormalized();
      auto u = (_lookUp ^ w).getNormalized();
      auto v = w ^ u;
      std::cout << w << " " << u << " " << v << " " << std::endl;
      std::cout << w * u << " " << u * v << " " << v * w << " " << std::endl;
      _cellOffsetRight = u * (_vpWidth / static_cast<double>(_imageWidth));
      _cellOffsetDown = v * (-_vpHeight / static_cast<double>(_imageHeight));
      _vpCorner = _lookAt
        + _cellOffsetRight * (-static_cast<double>(_imageWidth) / 2.0)
        + _cellOffsetDown * (-static_cast<double>(_imageHeight) / 2.0);
    }



  private:
    double _aspectRatio; // width / height
    double _vfov;
    unsigned int _raysPerPixel;// number of rays per pixels for anti-aliasing
    unsigned int _imageWidth; // image width in pixels
    unsigned int _imageHeight; // image height in pixels
    Vec3 _lookFrom; // center of the camera
    Vec3 _lookAt; // point to which the camera points
    Vec3 _lookUp; // up orientation of the camera
    double _vpWidth; // viewport width
    double _vpHeight; // viewport height
    Vec3 _cellOffsetRight; // offset of one window cell (pixel) to the right
    Vec3 _cellOffsetDown; // offset of one window cell (pixel) below
    Vec3 _vpCorner; // position of the top left corner of the viewport 
};
