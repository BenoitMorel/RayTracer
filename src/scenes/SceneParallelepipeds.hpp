
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


void addAxis(Scene *scene) {
    double axisWidth = 1.0;
    double axisLength = 5.0;

    auto redMaterial = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, Vec3(1.0, 0.0, 0.0));
    auto greenMaterial = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, Vec3(0.0, 1.0, 0.0));
    auto blueMaterial = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, Vec3(0.0, 0.0, 1.0));
    scene->materials.push_back(redMaterial);
    scene->materials.push_back(greenMaterial);
    scene->materials.push_back(blueMaterial);

    auto xaxis = std::make_shared<Parallelepiped>(Vec3(0, -axisWidth/2.0, -axisWidth/2.0),
        Vec3(0, 0, axisWidth),
        Vec3(axisLength, 0, 0),
        Vec3(0, axisWidth, 0.0),
        *redMaterial);
    scene->addBigShape(xaxis);

    auto yaxis = std::make_shared<Parallelepiped>(Vec3(-axisWidth/2.0, 0.0, -axisWidth/2.0),
        Vec3(axisWidth, 0, 0),
        Vec3(0, axisLength, 0),
        Vec3(0, 0.0, axisWidth),
        *greenMaterial);
    scene->addBigShape(yaxis);

    auto zaxis = std::make_shared<Parallelepiped>(Vec3(-axisWidth/2.0, -axisWidth/2.0, 0.0),
        Vec3(axisWidth, 0, 0),
        Vec3(0, axisWidth, 0),
        Vec3(0, 0.0, axisLength),
        *blueMaterial);
    scene->addBigShape(zaxis);
}

std::shared_ptr<Scene> createSceneParallelepiped(unsigned int imageWidth,
  unsigned int raysPerPixel,
  unsigned int cores) 
{
  auto scene = std::make_shared<Scene>();
  
  auto diffuseGreen = std::make_shared<Material>(0.0, 0.0, 1.0, 0.0, Vec3(0., 0.5, 0.));
  auto greenMetal = std::make_shared<Material>(0.0, 0.5, 0.0, 0.0, Vec3(0., 0.5, 0.));
  auto blueStuff = std::make_shared<Material>(0.0, 0.0, 1.0, 0.0, Vec3(0., 0., 0.7));
  auto redStuff = std::make_shared<Material>(0.0, 0.0, 0.5, 0.5, Vec3(0.7, 0., 0.));
  auto mirror = std::make_shared<Material>(0.0, 1.0, 0.0, 0.0, Vec3(1, 1, 1));
  auto light = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, Vec3(10.0, 10.0, 10.0));
  auto redLight = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, Vec3(10.0, 0.0, 0.0));
  auto blueLight = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, Vec3(0.0, 0.0, 10.0));
  auto greenLight = std::make_shared<Material>(0.0, 0.0, 0.0, 1.0, Vec3(0.0, 10.0, 0.0));
  auto mirrorAttenuated = std::make_shared<Material>(0.3, 0.8, 0.0, 0.0, Vec3(1, 1, 1));

  scene->materials.push_back(diffuseGreen);
  scene->materials.push_back(greenMetal);
  scene->materials.push_back(blueStuff);
  scene->materials.push_back(redStuff);
  scene->materials.push_back(mirror);
  scene->materials.push_back(mirrorAttenuated);

  scene->addBigShape(std::make_shared<Axis>(Vec3(), 0.5, 5.0));
  double groundRadius = 300;
  Vec3 groundCenter(0.0, -groundRadius, 1.0);
  auto bigBallRadius = 2.5;
  auto bigBallDepth = 5.0;
  // ground
  auto ground = std::make_shared<Sphere>(groundCenter, groundRadius, *blueStuff);
  scene->addBigShape(ground);
  scene->collisionManager.addSphere(ground);
  // light ball
  auto lightBall = std::make_shared<Sphere>(Vec3(0, 1.0, -3.0), 0.5, *light);
  scene->addBigShape(lightBall);

  auto parallelepiped = std::make_shared<Parallelepiped>(Vec3(0.0, 1.0, 1.0), 
    Vec3(1.0, 0.0, 0.0),
    Vec3(0.0, 1.0, 0.0),
    Vec3(0.0, 0.0, 1.0),
    *greenMetal);
    scene->addSmallShape(parallelepiped);

  auto parallelepiped2 = std::make_shared<Parallelepiped>(Vec3(3.0, 1.0, 1.0), 
    Vec3(1.0, 0.0, 0.0),
    Vec3(0.0, 1.0, 0.0),
    Vec3(0.0, 0.0, 1.0),
    *redStuff);
    scene->addSmallShape(parallelepiped2);

  auto parallelepiped3 = std::make_shared<Parallelepiped>(Vec3(-3.0, 1.0, 1.0), 
    Vec3(1.0, 0.0, 0.0),
    Vec3(0.0, 1.0, 0.0),
    Vec3(0.0, 0.0, 1.0),
    *mirror);
    scene->addSmallShape(parallelepiped3);
  
  unsigned int addedSpheres = 0;
  while (addedSpheres < 10) {
    auto x = getRand(-20.0, 20.0);
    double radius = 0.5;
    //auto y = getRand(radius, radius + 5.0);
    auto y = 0.0;
    auto z = getRand(-10,  0.0);
    Vec3 position(x,y,z);
    auto cp = groundCenter - position;
    position += cp.getNormalized() * (cp.norm() - groundRadius - radius);
    std::shared_ptr<Material> material;
    //Vec3 color = Vec3::getRandomVector(0.0, 1.0);
    auto r = getRand();
    if (r < 0.33) {
      material = redLight;
    } else if (r < 0.66) {
      material = greenLight;
    } else {
      material = blueLight;
    }
    auto shape = std::make_shared<Sphere>(position, radius, *material);
    if (scene->collisionManager.canAddSphere(shape)) {
      scene->addSmallShape(shape);
      scene->collisionManager.addSphere(shape);
      addedSpheres++;
    } 
  }
  double fov = 30;
  double aspectRatio = 1.5;
  Vec3 lookFrom(5, 6, 20);
  Vec3 lookAt(0.0, 1.0, 0.0);
  scene->camera = std::make_shared<Camera>(aspectRatio, imageWidth, fov, raysPerPixel, lookFrom, lookAt, cores);

  return scene;
}