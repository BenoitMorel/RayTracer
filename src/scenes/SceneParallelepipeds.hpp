
#include "../Scene.hpp"
#include "../shapes/Shapes.hpp"
#include "../shapes/Sphere.hpp"
#include "../shapes/Quad.hpp"
#include "../Camera.hpp"
#include "../shapes/BVH.hpp"
#include "../shapes/FramedQuad.hpp"
#include "../SphereCollisionManager.hpp"
#include "../shapes/Parallelepiped.hpp"
#include "../shapes/Axis.hpp"


class TrafficLights: public Shapes {
public:
    TrafficLights(Vec3 center, 
        double scaler,
        double lightIntensity,
        unsigned int state) :
            darkDiffuse(0.8, 0.0, 0.2, 0.0, Vec3(1.0, 1.0, 1.0)),
            redLight(0.0, 0.0, 0.0, 1.0, Vec3(1.0, 0.0, 0.0)),
            orangeLight(0.0, 0.0, 0.0, 1.0, Vec3(1.0, 0.5, 0.0)),
            greenLight(0.0, 0.0, 0.0, 1.0, Vec3(0, 1.0, 0.0))
    {
        double attenuation = 0.2;
        
        if (state == 0) {
            redLight.multiplyColor(lightIntensity);
            orangeLight.multiplyColor(attenuation);
            greenLight.multiplyColor(attenuation);
        } else if (state == 1) {
            redLight.multiplyColor(attenuation);
            orangeLight.multiplyColor(lightIntensity);
            greenLight.multiplyColor(attenuation);
        } else if (state == 2) {
            redLight.multiplyColor(attenuation);
            orangeLight.multiplyColor(attenuation);
            greenLight.multiplyColor(lightIntensity);
        } else {
            assert(false);
        }
        auto poleLength = 7.0 * scaler;
        center[1] += poleLength;
        auto boxWidth = scaler;
        auto boxHeight = scaler * 3.0;
        auto box = std::make_shared<Parallelepiped>(center - Vec3(boxWidth / 2.0, 0.0, boxWidth / 2.0), 
            Vec3(boxWidth, 0.0, 0.0),
            Vec3(0.0, boxHeight, 0.0),
            Vec3(0.0, 0.0, boxWidth),
            darkDiffuse);
        _buffer.push_back(box);
        addShape(box.get());
        
        auto poleWidth = scaler / 4.0;
        auto pole = std::make_shared<Parallelepiped>(center - Vec3(poleWidth / 2.0, 0.0, poleWidth /2.0), 
            Vec3(poleWidth, 0.0, 0.0),
            Vec3(0.0, -poleLength, 0.0),
            Vec3(0.0, 0.0, poleWidth),
            darkDiffuse);
        _buffer.push_back(pole);
        addShape(pole.get());
        double baseLength = scaler * 4.0;
        double baseHight = scaler * 0.3;
        auto base = std::make_shared<Parallelepiped>(center + Vec3(-baseLength / 2.0, -poleLength, -baseLength / 2.0), 
            Vec3(baseLength, 0.0, 0.0),
            Vec3(0.0, baseHight, 0.0),
            Vec3(0.0, 0.0, baseLength),
            darkDiffuse);
        _buffer.push_back(base);
        addShape(base.get());
        auto  ballRadius = scaler * 0.3   ;
        auto ballX = 0.0; // scaler * 0.5;
        auto ballY = scaler * 1.5;
        auto ballZ = scaler * 0.4;
        auto ball1 = std::make_shared<Sphere>(center + Vec3(ballX, ballY + scaler       , ballZ), ballRadius, redLight);
        auto ball2 = std::make_shared<Sphere>(center + Vec3(ballX, ballY + 0.0          , ballZ), ballRadius, orangeLight);
        auto ball3 = std::make_shared<Sphere>(center + Vec3(ballX, ballY -scaler        , ballZ), ballRadius, greenLight);
        _buffer.push_back(ball1);
        addShape(ball1.get());
        _buffer.push_back(ball2);
        addShape(ball2.get());
        _buffer.push_back(ball3);
        addShape(ball3.get());
    }
private:
    std::vector<std::shared_ptr<Shape> > _buffer;
    Material darkDiffuse;
    Material redLight;
    Material orangeLight;
    Material greenLight;
};


std::shared_ptr<Scene> createSceneParallelepiped(unsigned int imageWidth,
  unsigned int raysPerPixel,
  unsigned int cores) 
{
    auto scene = std::make_shared<Scene>();

    auto attenuatedMirror = std::make_shared<Material>(0.4, 0.6, 0.1, 0.0, Vec3(1, 1, 1));
    Vec3 pink = Vec3(255,192,203) / 255.0;
    auto light = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, Vec3(10, 10, 10));
    auto pinkLight = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, pink * 10.0);
    attenuatedMirror->setFuzz(0.015);
 
    scene->materials.push_back(attenuatedMirror);
    scene->materials.push_back(light);

    double groundSize = 20.0;
    auto ground = std::make_shared<Quad>(Vec3(-groundSize / 2.0, 0.0, -groundSize/ 2.0), 
            Vec3(groundSize, 0.0, 0.0),
            Vec3(0.0, 0.0, groundSize), 
            *attenuatedMirror);
    scene->addBigShape(ground);

    //scene->addBigShape(std::make_shared<Axis>(Vec3(), 0.5, 5.0));
    for (int i = 0; i < 3; ++i) {
        double intensity = 10.0;
        auto trafficLights = std::make_shared<TrafficLights>(Vec3(double(i - 1) * 4.0, 0.0, 0.0), 0.5, intensity, i);
        scene->addBigShape(trafficLights);
    }
    //auto moon = std::make_shared<Quad>(Vec3(20, 20, 20), Vec3(20.0, 0.0, 0.0), Vec(0.0, 0.0, 0.0), light);
    auto moon = std::make_shared<Sphere>(Vec3(20, 10, 10), 10.0, *pinkLight);
    scene->addBigShape(moon);


  double fov = 20;
  double aspectRatio = 1.0;
  Vec3 lookFrom(5, 20, 30);
  Vec3 lookAt(0.0, 0.0, 0.0);
  scene->camera = std::make_shared<Camera>(aspectRatio, imageWidth, fov, raysPerPixel, lookFrom, lookAt, cores);
  scene->camera->setBackgrounds(Vec3(0.2, 0.2, 0.3),Vec3(0.2, 0.2, 0.5));
  return scene;
}