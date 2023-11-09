#pragma once

 #include <limits>
 #include <random>
 #include "Vec3.hpp"

 class Shape;

class Ray {
  public: 
    Ray(const Vec3 &origin, const Vec3 &direction): _o(origin), _d(direction) {
      _d.normalize();
    }
    const Vec3 &origin() const {return _o;}
    const Vec3 &direction() const {return _d;}

    friend std::ostream& operator<<(std::ostream &os, const Ray &ray) {
      os << "(origin:" << ray.origin() << ", direction:" << ray.direction() << ")"; return os;
    }
  private:
    Vec3 _o;
    Vec3 _d;
};

struct Hit {
  Hit(): dist(std::numeric_limits<double>::max()),
    shape(nullptr) {}
  Vec3 point;
  Vec3 normal;
  double dist;
  const Shape * shape;
};

