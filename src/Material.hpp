#pragma once
#include "Vec3.hpp"

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
        void setColor(const Vec3 &color) {_color = color;}

    private:
        double _absorbtion;
        double _reflection;
        double _diffusion;
        double _ambiant;
        Vec3 _color;
        double _fuzz;
};
