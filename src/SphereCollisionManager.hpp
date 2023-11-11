#pragma once
#include <vector>
#include "shapes/Sphere.hpp"

class SphereCollisionManager {
public:
  SphereCollisionManager() {}
  virtual ~SphereCollisionManager() {}
  bool canAddSphere(std::shared_ptr<Sphere> sphere) {
    for (auto sp: _spheres) {
      if (sp->intersect(*sphere)) {
        return false;
      }
    }
    return true;
  }
  void addSphere(std::shared_ptr<Sphere> sphere) {
      _spheres.push_back(sphere);
  }
private:
  std::vector<std::shared_ptr<Sphere> >_spheres;
};
