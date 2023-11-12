#pragma once

#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <random>
#include <thread>
#include "Hit.hpp"
#include "shapes/Shape.hpp"
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
    Camera(double aspectRatio, unsigned int imageWidth, double vfov, unsigned int raysPerPixel,
    const Vec3 &lookFrom, const Vec3 &lookAt, unsigned int cores): 
      _aspectRatio(aspectRatio),
      _vfov(vfov),
      _raysPerPixel(raysPerPixel),
      _imageWidth(imageWidth),
      _imageHeight(0),
      _lookFrom(lookFrom),
      _lookAt(lookAt),
      _lookUp(0.0, 1.0, 0.0),
      _vpWidth(0.0),
      _vpHeight(0.0),
      _cores(cores),
      _background1(1.0, 1.0, 1.0),
      _background2(0.5, 0.5, 1.0)
    {

    }

    void setBackgrounds(const Vec3 &b1, const Vec3 &b2) {
      _background1 = b1;
      _background2 = b2;
    }

    Vec3 getRayColor(const Ray &ray, const Shape &world, unsigned int depth) const {
        auto color = Vec3(0.0, 0.0, 0.0);
        if (depth > 10) {
            return color;
        }
        
        Hit hit;
        const double minDist = 0.00001;
        if (world.hit(ray, minDist, hit)) {
            const auto &material = hit.shape->getMaterial();
            
            if (material.getAmbiant() > 0.0) {
              color += material.getColor() * material.getAmbiant();
            }
            if (material.getDiffusion() > 0.0) {
                auto newDirection = hit.normal + Vec3::getRandomUnitVector();
                Ray newRay(hit.point, newDirection);
                color += getRayColor(newRay, world, depth + 1) * material.getDiffusion();
                // TODO near zero
            }
            
            if (material.getReflection() > 0.0) {
                auto newDirection = ray.direction() - hit.normal * (hit.normal * ray.direction()) * 2.0;
                newDirection += Vec3::getRandomUnitVector() * material.getFuzz();
                Ray newRay(hit.point, newDirection);
                auto temp = getRayColor(newRay, world, depth + 1) * material.getReflection();
                for (unsigned int i = 0; i < 3; ++i) {
                    temp[i] *= material.getColor()[i];
                }
                color += temp;
            }
        } else {
            auto t = 0.5 * (ray.direction()[1] + 1.0);
            color = _background1 * (1.0-t) + _background2 * t;
            //color = Vec3(0.1, 0.1, 0.1);
        }
        return color;
    }


    Ray getRay(unsigned int x, unsigned int y) const {
      double rightFactor = static_cast<double>(x) + getRand(-0.5, 0.5);
      double downFactor = static_cast<double>(y) + getRand(-0.5, 0.5);
      if (_raysPerPixel == 1) {
        rightFactor = static_cast<double>(x);
        downFactor = static_cast<double>(y);
      }
      auto cell = _vpCorner + _cellOffsetRight * rightFactor + _cellOffsetDown * downFactor;
      return Ray(_lookFrom, cell - _lookFrom);
    }


    void render(const Shape &world) {
      _updateParameters();
      Image image(_imageWidth, _imageHeight);
      unsigned int pixelsNumber = _imageWidth * _imageHeight;
      std::vector<std::thread> threads;
      unsigned int threadsNumber = _cores;
      unsigned int chunkSize = pixelsNumber / threadsNumber;
      if (_cores == 1) {
          renderAux(world, image, 0, pixelsNumber);
      } else {
          for (unsigned int i = 0; i < threadsNumber; ++i) {
              unsigned int start = i * chunkSize;
              unsigned int end = (i == threadsNumber - 1) ? pixelsNumber : (i + 1) * chunkSize;
              threads.push_back(std::thread(&Camera::renderAux, this, std::ref(world), std::ref(image), start, end));
          }
          for (auto& thread : threads) {
              thread.join();
          }
      }
      //image.blur();
      //image.cartoonize(8);
      // save
      std::string output = "C:\\Users\\benom\\github\\RayTracer\\src\\output.ppm";
      std::cout << "Output in " << output << std::endl;
      image.writePPM(output);
    }

    void renderAux(const Shape &world, Image &image, unsigned int start, unsigned int end)  const {
      for (unsigned int i = start; i < end; ++i) {
        unsigned int x = i % _imageWidth;
        unsigned int y = i / _imageWidth;
        Vec3 averageColor;
        for (unsigned int it = 0; it < _raysPerPixel; ++it) {
          auto ray = getRay(x, y);
          averageColor += getRayColor(ray, world, 0);
        }
        // average
        averageColor /= double(_raysPerPixel);
        // linear to scalar scale
        /*
        averageColor[0] = sqrt(averageColor[0]);
        averageColor[1] = sqrt(averageColor[1]);
        averageColor[2] = sqrt(averageColor[2]);
        */
        // from [0,1] to [0, 255]
        
        for (unsigned int i = 0; i < 3; ++i) {
          averageColor[i] = std::min(1.0, averageColor[i]);
        }
        averageColor *= 255.0;
        image(x, y) = averageColor;
      }
    }

  private:
    /**
     * Update the different parameters of the camera before rendering
    */
    void _updateParameters() {
      _imageHeight = static_cast<unsigned int>(static_cast<double>(_imageWidth) / _aspectRatio);
      auto theta = _vfov * M_PI / 180.0;
      auto focal = (_lookAt - _lookFrom).norm();
      _vpHeight = 2 * tan(theta / 2.0) * focal;
      _vpWidth = _vpHeight * _aspectRatio;

      auto w = (_lookFrom - _lookAt).getNormalized();
      auto u = (_lookUp ^ w).getNormalized();
      auto v = w ^ u;
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
    unsigned int _cores;
    Vec3 _background1;
    Vec3 _background2;
};
