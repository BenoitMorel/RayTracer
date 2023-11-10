#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "Shape.hpp"

class Shapes : public Shape {
  public:
    Shapes() {}
    virtual ~Shapes() {}
    void addShape(Shape *shape) {_shapes.push_back(shape);
      _aabb.unionWith(shape->getAABB());
    }
    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const {
      bool ok = false;
      for (auto shape: _shapes) {
        ok |= shape->hit(ray, minDist, hit);
      }
      return ok;
    }
    const std::vector<Shape *> &getShapes() const {return _shapes;}
  private:
    std::vector<Shape *> _shapes;
};

