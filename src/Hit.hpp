#pragma once

 #include <limits>
 #include <random>
 #include "Vec3.hpp"

 class Shape;

struct Hit {
  Hit(): dist(std::numeric_limits<double>::max()),
    shape(nullptr) {}
  Vec3 point;
  Vec3 normal;
  double dist;
  const Shape * shape;
};

