#pragma once

#include "Quad.hpp"

class Parallelepiped: public Shapes {
public:
    Parallelepiped(const Vec3 &corner,
        const Vec3 &side1, 
        const Vec3 &side2,
        const Vec3 &side3,
        const Material &material) {
            Material special = material;
            //special.setColor(Vec3(0.0, 0.0, 1.0));
            addFace(std::make_shared<Quad>(corner, side1, side2, special));
            addFace(std::make_shared<Quad>(corner, side2, side3, material));
            addFace(std::make_shared<Quad>(corner, side3, side1, material));
            auto otherCorner = corner + side1 + side2 + side3;
            addFace(std::make_shared<Quad>(otherCorner, -side2, -side1, material));
            addFace(std::make_shared<Quad>(otherCorner, -side3, -side2, material));
            addFace(std::make_shared<Quad>(otherCorner, -side1, -side3, material));
        }




private:
    void addFace(std::shared_ptr<Quad> face) {
        addShape(face.get());
        _faces.push_back(face);
    }
    std::vector<std::shared_ptr<Quad> > _faces;

};