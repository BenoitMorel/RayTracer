#pragma once

#include "Vec3.hpp"
#include "Shape.hpp"
#include "Shapes.hpp"


/**
 * Create a framed quad using one Quad in front of another
 * Each of them can have a different material
*/
class FramedQuad : public Shape {
  public:
    FramedQuad(const Vec3 &corner, const Vec3 &side1, const Vec3 &side2, double frameSize, const Material &material, const Material &frameMaterial) {
      Vec3 frameOffset = (side1.getNormalized() + side2.getNormalized()) * frameSize; 
      Vec3 offsetBehind = (side1 ^ side2).getNormalized() * 0.001 ;
      std::cout << offsetBehind << std::endl;
      Vec3 inCorner = corner + frameOffset - offsetBehind;
      
      Vec3 inSide1 = side1 - side1.getNormalized() * (2.0 * frameSize); 
      Vec3 inSide2 = side2 - side2.getNormalized() * (2.0 * frameSize);
      _in = std::make_shared<Quad>(inCorner, inSide1, inSide2, material);
      _out = std::make_shared<Quad>(corner, side1, side2, frameMaterial);
      _shapes.addShape(_in.get());
      _shapes.addShape(_out.get());
      _aabb = _shapes.getAABB();
    }
    
    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const {
      return _shapes.hit(ray, minDist, hit);
    }
  private:
    std::shared_ptr<Quad> _in;
    std::shared_ptr<Quad> _out;
    Shapes _shapes;
};

