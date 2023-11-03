#pragma once

#include <math.h>

class Vec3 {
  public:
   
    /**
     *  Constructors and destructors
     */
    Vec3(): _v{0.0, 0.0, 0.0} {}
    Vec3(const Vec3 &v): _v{v[0], v[1], v[2]} {}
    Vec3(double x, double y, double z): _v{x, y, z} {}
    virtual ~Vec3() {}


    /**
     *  Accessors
     */
    double operator[](int i) const {return _v[i];}
    double& operator[](int i) {return _v[i];}

    /*
     * common vector operations 
     */
    Vec3 operator+(const Vec3 &v) const {return Vec3(_v[0] + v[0], _v[1] + v[1], _v[2] + v[2]);} 
    Vec3 operator-(const Vec3 &v) const {return Vec3(_v[0] - v[0], _v[1] - v[1], _v[2] - v[2]);} 
    Vec3 &operator+=(const Vec3 &v) {_v[0] += v[0]; _v[1] += v[1]; _v[2] += v[2]; return *this;} 
    Vec3 &operator-=(const Vec3 &v) {_v[0] -= v[0]; _v[1] -= v[1]; _v[2] -= v[2]; return *this;} 
    Vec3 &operator=(const Vec3 &v) {_v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; return *this;}
    
    /**
     *  command float operations
     */
    Vec3 operator*(double val) const {return Vec3(_v[0] * val,  _v[1] * val, _v[2] * val); return *this;} 
    Vec3 operator/(double val) const {return Vec3(_v[0] / val,  _v[1] / val, _v[2] / val); return *this;} 
    Vec3 &operator*=(double val) {_v[0] *= val; _v[1] *= val; _v[2] *= val; return *this;} 
    Vec3 &operator/=(double val) {_v[0] /= val; _v[1] /= val; _v[2] /= val; return *this;} 
    

    /**
     *  Common tests
     */
    bool operator==(const Vec3 &v) const {return v[0] == _v[0] && v[1] == _v[1] && v[2] == _v[2];}

    /**
     *  Dot and cross products
     */
    double operator*(const Vec3 &v) const {return _v[0] * v[0] + _v[1] * v[1] + _v[2] * v[2];} 
    Vec3 operator^(const Vec3 &v) const {return Vec3(_v[1] * v[2] - _v[2] * v[1], 
        _v[2] * v[0] - _v[0] * v[2],
        _v[0] * v[1] - _v[1] * v[1]);} 

    /**
     *  normalization
     */
    double normSquare() const {return (*this) * (*this);}
    double norm() const {return sqrt(normSquare());}
    void normalize() {auto n = norm(); _v[0] /= n; _v[1] /= n; _v[2] /= n;}
    Vec3 getNormalized() const {Vec3 res = *this; res.normalize(); return res;}

    friend std::ostream& operator<<(std::ostream &os, const Vec3 &v) { os << "(" << v[0] << "," << v[1] << "," << v[2] << ")"; return os;}

  private:
    double _v[3];
};
