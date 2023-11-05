#pragma once 
#include <vector>
#include "Common.hpp"


class Material {
    public:
        Material(): _absorbtion(0.2), _reflection(0.0), _diffusion(0.3), _ambiant(0.5),
            _color(1.0, 1.0, 1.0)  {
        }

        Material(double absorbtion, 
            double reflection, 
            double diffusion, 
            double ambiant, 
            const Vec3 &color): _color(color) {
            double sum = absorbtion + reflection + diffusion + ambiant;
          _absorbtion = absorbtion / sum;
          _reflection = reflection / sum;
          _diffusion = diffusion / sum;  
          _ambiant = ambiant / sum;
        }

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
};

class Shape {
  public:
    virtual ~Shape() {}
    virtual void setMaterial(const Material &material) {_material = material;}
    virtual const Material &getMaterial() const {return _material;}
    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const = 0;

  private:
    Material _material;
};

class Sphere : public Shape {
  public:
    Sphere(const Vec3 &center, double radius): _center(center), _radius(radius), _radiusSquare(_radius * _radius) {}
    virtual ~Sphere() {}
    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const {
      // O is the origin, C the center
      // Pc the project of C on the ray
      // P1 is one of the potential 2 intersections with the sphere
      auto OC = _center - ray.origin();
      auto normOPc = OC * ray.direction();
      if (normOPc < 0.0) {
        //std::cout << "behind" << std::endl;
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
      if (dist1 < minDist) {
        return false;
      }
      auto P1 = ray.origin() + ray.direction() * dist1;
      if (dist1 < hit.dist) {
        hit.point = P1;
        hit.dist = dist1;
        hit.normal = (P1 - _center).getNormalized();
        hit.shape = this;
      }
      return true;
    }
    const Vec3& center() const {return _center;}
    double radius() const {return _radius;}
  private:
    Vec3 _center;
    double _radius;
    double _radiusSquare;
};

class Shapes : public Shape {
  public:
    Shapes() {}
    virtual ~Shapes() {}
    void addShape(Shape &shape) {_shapes.push_back(&shape);}
    virtual bool hit(const Ray &ray, double minDist, Hit &hit) const {
      bool ok = false;
      for (auto shape: _shapes) {
        ok |= shape->hit(ray, minDist, hit);
      }
      return ok;
    }
  private:
    std::vector<Shape *> _shapes;

};

