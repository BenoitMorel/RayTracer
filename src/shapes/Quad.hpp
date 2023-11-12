#pragma once

#include "Shape.hpp"

/*
 *  a quad in the 3D space
 */
class Quad : public Shape {
  public:
    Quad(const Vec3 &corner, const Vec3 &side1, const Vec3 &side2, const Material &material):
      Shape(material),
      _corner(corner),
      _side1(side1),
      _side2(side2),
      _normal((side1 ^ side2)),
      _D(_normal * _corner)
  {  
    auto corner2 = _corner + _side1 + _side2;
    Interval box[3];
    double epsilon = 0.00001;
    for (unsigned int i = 0; i < 3; ++i) {
      double m = std::min(corner[i], corner2[i]);
      double M = std::max(corner[i], corner2[i]);
      if (M - m < epsilon) {
        m -= epsilon;
        M += epsilon;
      }
      box[i] = Interval(m, M);
    }
    setAABB(AABB(box[0], box[1], box[2]));
    _precomputedVec = _normal / (_normal * _normal);
  }

  virtual bool hit(const Ray &ray, double minDist, Hit &hit) const {
    double den = _normal * ray.direction();
    if (den == 0.0) {
      // the ray and the plane are parallel, no intersection
      return false;
    }
    // t is the point such that the intersection point P = ray.or + t * ray.dir
    double t = (_D - _normal * ray.origin()) / den;
    if (t < minDist || t > hit.dist) {
      return false;
    }
    Vec3 P = ray.origin() + ray.direction() * t;
    // compute alpha and beta such that P = corner + alpha side1 + beta side2
    // solution: cross product (P-corner) with side1 and with side2, dot product with
    // the normal, and then we get two equations to compute alpha and beta respectively
    auto QP = P - _corner;
    auto alpha = _precomputedVec * (QP ^ _side2);
    if (alpha < 0.0 || alpha > 1.0) {
      return false;
    }
    auto beta = _precomputedVec * (_side1 ^ QP);
    if (beta < 0.0 || beta > 1.0) {
      return false;
    }
    // there is a hit!
    hit.point = P;
    hit.dist = t;
    hit.normal = _normal.getNormalized();
    if (hit.normal * ray.direction() > 0.0) {
      hit.normal = -hit.normal;
    }
    hit.shape = this;
    return true;
  }

  private:
    Vec3 _corner; // one corner of the quad
    Vec3 _side1;  // one side of the quad (corner + side == another corner)
    Vec3 _side2;  // the other side
    Vec3 _normal; // normal of the plane containing the quad
    double _D; // The plane equation is Ax + By + Cz = D, where (A,B,C) is the normal
    Vec3 _precomputedVec;
};

