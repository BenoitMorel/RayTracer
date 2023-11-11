#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

/*
 * Generate a random number 
*/
inline double getRand(double mi = 0.0, double ma = 1.0) {
  // todo: find a compromise between this and c++ slow generators  
  return mi + (rand() / ( RAND_MAX / (ma-mi) ) ) ;
      /*
    static std::mt19937 generator;
    std::uniform_real_distribution<double> distribution(mi, ma);
    auto res = distribution(generator);
    return res;
    */
}

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
    inline double operator[](int i) const {return _v[i];}
    inline double& operator[](int i) {return _v[i];}

    /*
     * common vector operations 
     */
    inline Vec3 operator+(const Vec3 &v) const {return Vec3(_v[0] + v[0], _v[1] + v[1], _v[2] + v[2]);} 
    inline Vec3 operator-(const Vec3 &v) const {return Vec3(_v[0] - v[0], _v[1] - v[1], _v[2] - v[2]);} 
    inline Vec3 &operator+=(const Vec3 &v) {_v[0] += v[0]; _v[1] += v[1]; _v[2] += v[2]; return *this;} 
    inline Vec3 &operator-=(const Vec3 &v) {_v[0] -= v[0]; _v[1] -= v[1]; _v[2] -= v[2]; return *this;} 
    inline Vec3 &operator=(const Vec3 &v) {_v[0] = v[0]; _v[1] = v[1]; _v[2] = v[2]; return *this;}
    inline Vec3 operator-() const {return Vec3(-_v[0], -_v[1],-_v[2]);}
    
    /**
     *  command float operations
     */
    inline Vec3 operator*(double val) const {return Vec3(_v[0] * val,  _v[1] * val, _v[2] * val); return *this;} 
    inline Vec3 operator/(double val) const {return Vec3(_v[0] / val,  _v[1] / val, _v[2] / val); return *this;} 
    inline Vec3 &operator*=(double val) {_v[0] *= val; _v[1] *= val; _v[2] *= val; return *this;} 
    inline Vec3 &operator/=(double val) {_v[0] /= val; _v[1] /= val; _v[2] /= val; return *this;} 
    

    /**
     *  Common tests
     */
    inline bool operator==(const Vec3 &v) const {return v[0] == _v[0] && v[1] == _v[1] && v[2] == _v[2];}

    /**
     *  Dot and cross products
     */
    inline double operator*(const Vec3 &v) const {return _v[0] * v[0] + _v[1] * v[1] + _v[2] * v[2];} 
    inline Vec3 operator^(const Vec3 &v) const {return Vec3(_v[1] * v[2] - _v[2] * v[1], 
        _v[2] * v[0] - _v[0] * v[2],
        _v[0] * v[1] - _v[1] * v[0]);} 

    /**
     *  normalization
     */
    inline double normSquare() const {return (*this) * (*this);}
    inline double norm() const {return sqrt(normSquare());}
    inline void normalize() {auto n = norm(); _v[0] /= n; _v[1] /= n; _v[2] /= n;}
    inline Vec3 getNormalized() const {Vec3 res = *this; res.normalize(); return res;}

    void ensureBounds(double min, double max) {
      for (unsigned int i = 0; i < 3; ++i) {
        if (_v[i] < min) {
          _v[i] = min;
        }
        if (_v[i] > max) {
          _v[i] = max;
        }
      }
    }

    static Vec3 getRandomVector(double min,double max) {
      return Vec3(getRand(min, max), getRand(min, max), getRand(min, max));
    }

    /*
    * Draw a unit vector from a uniform distribution (with rejection)
    */
    static Vec3 getRandomUnitVector() {
      auto theta = getRand(0.0, 2.0 * M_PI);
      auto z = getRand(-1.0, 1.0);
      auto temp = sqrt(1 - z * z);
      auto x = temp * cos(theta);
      auto y = temp * sin(theta);
      return Vec3(x, y, z);
      /*
      Old rejection implementation:
      while (true) {
        Vec3 res = getRandomVector(-1.0, 1.0);
        if (res.normSquare() < 1.0) {
          return res.getNormalized();
        }
      }
      */
    }

    friend std::ostream& operator<<(std::ostream &os, const Vec3 &v) { os << "(" << v[0] << "," << v[1] << "," << v[2] << ")"; return os;}

  private:
    double _v[3];
};
