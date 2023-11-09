#pragma once
#include <limits>

struct Interval {
  Interval(): min(std::numeric_limits<double>::infinity()), 
    max(-std::numeric_limits<double>::infinity()) {}
  Interval(double min, double max): min(min), max(max) {}
  double min;
  double max;
  double getCenter() const { return (min + max) * 0.5; }
  void unionWith(const Interval &interval) {
    min = std::min(min, interval.min);
    max = std::max(max, interval.max);
  }
  friend std::ostream& operator<<(std::ostream &os, const Interval &v) {os << v.min << " " << v.max; return os;}
};


class AABB {
public:
  AABB() {}
  AABB(const Interval &x, const Interval &y, const Interval &z) : _box{x, y, z} {}
  virtual ~AABB() {}
  bool hit(const Ray &ray) const {
    // this is almost a copy paste from: 
    // https://raytracing.github.io/books/RayTracingTheNextWeek.html#boundingvolumehierarchies/constructingboundingboxesforhittables
    Interval ray_t(-std::numeric_limits<double>::infinity(),
      std::numeric_limits<double>::infinity());

    //std::cout << "Checking intersection between " << *this << " and " << ray << std::endl;
    for (int a = 0; a < 3; a++) {
      auto invD = 1.0 / ray.direction()[a];
      auto orig = ray.origin()[a];
      auto t0 = (_box[a].min - orig) * invD;
      auto t1 = (_box[a].max - orig) * invD;
      if (invD < 0) {
        std::swap(t0, t1);
      }
      if (t0 > ray_t.min) {
        ray_t.min = t0;
      }
      if (t1 < ray_t.max) {
        ray_t.max = t1;
      }
      if (ray_t.max <= ray_t.min) {
        return false;
      }
    }
    return true;
  }
  const Interval &getInterval(unsigned int axis) const {return _box[axis];}

  void unionWith(const AABB &other) {
    for (unsigned int i = 0; i < 3; ++i) {
      _box[i].unionWith(other._box[i]);
    }
  }
  friend std::ostream& operator<<(std::ostream &os, const AABB &v) { os << "(" << v._box[0] << "," << v._box[1] << "," << v._box[2] << ")"; return os;}
private:
  Interval _box[3];
};

