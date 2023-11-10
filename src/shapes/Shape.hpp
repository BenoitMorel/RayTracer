#pragma once 
#include <vector>
#include <memory>
#include "../AABB.hpp"
#include "../Ray.hpp"
#include "../Material.hpp"
#include "../Hit.hpp"

class Shape {
  public:
    Shape() {}
    Shape(const Material &material): _material(material) {}
    virtual ~Shape() {}
    virtual void setMaterial(const Material &material) {_material = material;}
    virtual void setAABB(const AABB &aabb) {_aabb = aabb;}
    virtual const Material &getMaterial() const {return _material;}
    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const = 0;
    virtual const AABB &getAABB() const {return _aabb;}
    virtual AABB &getAABB() {return _aabb;}
  private:
    Material _material;
    AABB _aabb;
};


