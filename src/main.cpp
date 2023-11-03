#include <iostream>
#include <assert.h>
#include "Vec3.hpp"
#include <vector>
#include <string>
#include <fstream>

/*
 *  A few unit tests
 *  A bit dirty but this is a toy project
 */
void unittest()
{
  Vec3 v1(1.0, 2.0, 3.0);
  Vec3 v2(2.0, 2.0, 0.0);
  assert(v1 + v2 == Vec3(3,4,3));
  assert(v1 - v2 == Vec3(-1,0,3));
  assert(v1 * v2 == 6.0);
  Vec3 cross = v1 ^ v2;
  assert(cross == Vec3(-6,6,-2));
}

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

class Ray {
  public: 
    Ray(const Vec3 &origin, const Vec3 &direction): _o(origin), _d(direction) {
      _d.normalize();
    }
    const Vec3 &origin() const {return _o;}
    const Vec3 &direction() const {return _d;}
  private:
    Vec3 _o;
    Vec3 _d;

};

struct Hit {
  Hit() {}
  Hit(const Vec3 &point, const Vec3 &normal):
    point(point),
    normal(normal) {}
  Vec3 point;
  Vec3 normal;
};

class Shape {
  public:
    virtual ~Shape() {}
    virtual bool hit(const Ray &ray, Hit &hit) const = 0;
};

class Sphere : public Shape {
  public:
    Sphere(const Vec3 &center, double radius): _center(center), _radius(radius), _radiusSquare(_radius * _radius) {}
    virtual ~Sphere() {}
    virtual bool hit(const Ray &ray, Hit &hit) const {
      // O is the origin, C the center
      // Pc the project of C on the ray
      // P1 is one of the potential 2 intersections with the sphere
      auto OC = _center - ray.origin();
      auto normOPc = OC * ray.direction();
      if (normOPc < 0.0) {
        //std::cout << "behind" << std::endl;
        // the object is behind the eye
        return false;
      }
      auto normOPcSquare = normOPc * normOPc;
      auto CPcSquare = OC.normSquare() - normOPcSquare;
      
      auto P1PCSQuare = _radiusSquare - CPcSquare;
      if (P1PCSQuare <= 0.0) {
        // no hit with the sphere
        return false;
      }
      return true;
    }
    const Vec3& center() const {return _center;}
    double radius() const {return _radius;}
  private:
    Vec3 _center;
    double _radius;
    double _radiusSquare;
};

class Shapes {
  public:
    Shapes() {}
    virtual ~Shapes() {}
    void addShape(Shape &shape) {_shapes.push_back(&shape);}
    bool hit(const Ray &ray, Hit &hit) {
      bool ok = false;
      for (auto shape: _shapes) {
        ok |= shape->hit(ray, hit);
      }
      return ok;
    }
  private:
    std::vector<Shape *> _shapes;

};

int main(int, char **)
{
  unittest();
  
  // eye and window positions
  double focalLength = 1.0;
  Vec3 eye(0.0, 0.0, -1.0);
  Vec3 focusPoint(0.0, 0.0, 0.0);
  Vec3 eyeDir(focusPoint - eye);

  Vec3 eyeUpDir(0.0, 1.0, 0.0);
  Vec3 eyeRightDir = 


  double vpWidth = 3.0;
  double vpHeight = 2.0;
  unsigned int imageWidth = 600;
  unsigned int imageHeight = 400;
  

  // init the shapes 
  Shapes shapes;
  Sphere sphere1(Vec3(0, 5, 50.0), 5.0); shapes.addShape(sphere1);
  Sphere sphere2(Vec3(-15, 5, 50.0), 5.0); shapes.addShape(sphere2);
  double bigRadius = 1000;
  Sphere sphere3(Vec3(0, -bigRadius, 50.0), bigRadius); shapes.addShape(sphere3);
 
  // cast rays
  Image image(600, 400);

  unsigned int hits = 0;
  Vec3 vpCenter = eye + eyeDir.getNormalized() * focalLength;
  for (unsigned int x = 0; x < image.width(); ++x) {
    for (unsigned int y = 0; y < image.height(); ++y) {
      double xratio = (double(x) + 0.5) / double(image.width());
      double yratio = (double(y) + 0.5) / double(image.height());
      
      Vec3 cell = vpCenter;
      cell += eyeRightDir * (vpWidth * (0.5 - xratio));
      cell += eyeUpDir * (vpHeight * (0.5 - yratio));
      
      Ray ray(eye, cell - eye);

      Vec3 color(xratio, yratio, .5);
      Hit hit;
      if (shapes.hit(ray, hit)) {
        color = Vec3(1.0, 1.0, 1.0);
        hits += 1;
      }
      color *= 256;
      image(x, y) = color;
    }
  }
  std::cout << double(hits) / double(image.width() * image.height()) << " hits " << std::endl;
  // save
  image.writePPM("output.ppm");
  return 0;
}
