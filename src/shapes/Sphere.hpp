#pragma once


class Sphere : public Shape {
  public:
    Sphere(const Vec3 &center, double radius, const Material &material): 
      Shape(material),
      _center(center), 
      _radius(radius), 
      _radiusSquare(_radius * _radius) {
        Interval aabbx(center[0] - radius, center[0] + radius); 
        Interval aabby(center[1] - radius, center[1] + radius); 
        Interval aabbz(center[2] - radius, center[2] + radius); 
        setAABB(AABB(aabbx, aabby, aabbz));
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
