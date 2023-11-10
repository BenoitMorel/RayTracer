#pragma once 
#include <vector>
#include "Common.hpp"
#include <memory>
#include "AABB.hpp"

class Material {
    public:
        Material(): _absorbtion(0.2), _reflection(0.0), _diffusion(0.3), _ambiant(0.5),
            _color(1.0, 1.0, 1.0), _fuzz(0.0)  {
        }

        Material(double absorbtion, 
            double reflection, 
            double diffusion, 
            double ambiant, 
            const Vec3 &color): _color(color), _fuzz(0.0) {
            double sum = absorbtion + reflection + diffusion + ambiant;
          _absorbtion = absorbtion / sum;
          _reflection = reflection / sum;
          _diffusion = diffusion / sum;  
          _ambiant = ambiant / sum;
        }

        void setFuzz(double fuzz) {_fuzz = std::min(fuzz, 1.0);}
        double getFuzz() const {return _fuzz;}

        double getAbsorbtion() const {return _absorbtion;}
        double getReflection() const {return _reflection;}
        double getDiffusion() const {return _diffusion;}
        double getAmbiant() const {return _ambiant;}
        const Vec3 &getColor() const {return _color;}

    private:
        double _absorbtion;
        double _reflection;
        double _diffusion;
        double _ambiant;
        Vec3 _color;
        double _fuzz;
};

class Shape {
  public:
    virtual ~Shape() {}
    virtual void setMaterial(const Material &material) {_material = material;}
    virtual const Material &getMaterial() const {return _material;}
    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const = 0;
    virtual const AABB &getAABB() const {return _aabb;}
  protected:
    Material _material;
    AABB _aabb;
};

/*
 *  a quad in the 3D space
 */
class Quad : public Shape {
  public:
    Quad(const Vec3 &corner, const Vec3 &side1, const Vec3 &side2, const Material &material):
      _corner(corner),
      _side1(side1),
      _side2(side2),
      _normal((side1 ^ side2)),
      _D(_normal * _corner)
  {
    setMaterial(material);
    
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
    _aabb = AABB(box[0], box[1], box[2]);
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


class Sphere : public Shape {
  public:
    Sphere(const Vec3 &center, double radius, const Material &material): 
      _center(center), 
      _radius(radius), 
      _radiusSquare(_radius * _radius) {
        setMaterial(material);
        Interval aabbx(center[0] - radius, center[0] + radius); 
        Interval aabby(center[1] - radius, center[1] + radius); 
        Interval aabbz(center[2] - radius, center[2] + radius); 
        _aabb = AABB(aabbx, aabby, aabbz);
      }
    virtual ~Sphere() {}
    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const {
      // O is the origin, C the center
      // Pc the project of C on the ray
      // P1 is one of the potential 2 intersections with the sphere
      auto OC = _center - ray.origin();
      auto normOPc = OC * ray.direction();
      if (normOPc < 0.0) {
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
      if (dist1 < minDist || dist1 > hit.dist) {
        return false;
      }
      auto P1 = ray.origin() + ray.direction() * dist1;
      hit.point = P1;
      hit.dist = dist1;
      hit.normal = (P1 - _center).getNormalized();
      hit.shape = this;
      return true;
    }
    const Vec3& center() const {return _center;}
    
    double radius() const {return _radius;}

    bool intersect(const Sphere &other) {
      auto d = (other.center() - center()).norm();
      return d < (radius() + other.radius());
    }

  private:
    Vec3 _center;
    double _radius;
    double _radiusSquare;
};


