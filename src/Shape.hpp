#pragma once 
#include <vector>
#include "Common.hpp"


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
      auto P1PCnorm = sqrt(P1PCSQuare);
      auto dist1 = normOPc - P1PCnorm;
      auto P1 = ray.origin() + ray.direction() * dist1;
      if (dist1 < hit.dist) {
        hit.point = P1;
        hit.dist = dist1;
        hit.normal = (P1 - _center).getNormalized();
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

class Shapes : public Shape {
  public:
    Shapes() {}
    virtual ~Shapes() {}
    void addShape(Shape &shape) {_shapes.push_back(&shape);}
    virtual bool hit(const Ray &ray, Hit &hit) const {
      bool ok = false;
      for (auto shape: _shapes) {
        ok |= shape->hit(ray, hit);
      }
      return ok;
    }
  private:
    std::vector<Shape *> _shapes;

};

