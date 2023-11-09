#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "Shape.hpp"
#include "BVH.hpp"

class Shapes : public Shape {
  public:
    Shapes() {}
    virtual ~Shapes() {}
    void addShape(Shape *shape) {_shapes.push_back(shape);}
    void updateBVH() {
      std::cout << "Creating BVH..." << std::endl;
      _bvh = std::make_shared<BVH>(_shapes);
    }

    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const {
      assert(_bvh.get());
      static int hits = 0;
      static int trials = 0;
      static int iterations = 0;
      ShapeSet candidates;
      _bvh->getHitCandidates(ray, candidates);
      trials += _shapes.size();
      hits += candidates.size();
      if (iterations++ % 10000 == 0) 
      std::cout << "Savings" << float(trials) / float(hits) << std::endl;
      bool ok = false;
      for (auto shape: candidates) {
        ok |= shape->hit(ray, minDist, hit);
      }
      return ok;
    }
  private:
    std::vector<Shape *> _shapes;
    std::shared_ptr<BVH> _bvh;
};

