#pragma once


class Axis: public Shapes {
public:
    Axis(const Vec3 &center, 
        double axisWidth,
        double axisLength):
            redMaterial(0.0, 0.0, 0.0, 1.0, Vec3(1.0, 0.0, 0.0)),
            greenMaterial(0.0, 0.0, 0.0, 1.0, Vec3(0.0, 1.0, 0.0)),
            blueMaterial(0.0, 0.0, 0.0, 1.0, Vec3(0.0, 0.0, 1.0)) 
    {
        xaxis = std::make_shared<Parallelepiped>(Vec3(0, -axisWidth/2.0, -axisWidth/2.0),
            Vec3(0, 0, axisWidth),
            Vec3(axisLength, 0, 0),
            Vec3(0, axisWidth, 0.0),
            redMaterial);
        addShape(xaxis.get());
        yaxis = std::make_shared<Parallelepiped>(Vec3(-axisWidth/2.0, 0.0, -axisWidth/2.0),
            Vec3(axisWidth, 0, 0),
            Vec3(0, axisLength, 0),
            Vec3(0, 0.0, axisWidth),
            greenMaterial);
        addShape(yaxis.get());

        zaxis = std::make_shared<Parallelepiped>(Vec3(-axisWidth/2.0, -axisWidth/2.0, 0.0),
            Vec3(axisWidth, 0, 0),
            Vec3(0, axisWidth, 0),
            Vec3(0, 0.0, axisLength),
            blueMaterial);
        addShape(zaxis.get());
    }
    virtual ~Axis() {}
private:
    Material redMaterial;
    Material greenMaterial;
    Material blueMaterial;
    std::shared_ptr<Shape> xaxis;
    std::shared_ptr<Shape> yaxis;
    std::shared_ptr<Shape> zaxis;
};