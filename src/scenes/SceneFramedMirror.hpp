
#include "../Scene.hpp"
#include "../shapes/Shapes.hpp"
#include "../shapes/Sphere.hpp"
#include "../shapes/Quad.hpp"
#include "../Camera.hpp"
#include "../shapes/BVH.hpp"
#include "../shapes/FramedQuad.hpp"
#include "../SphereCollisionManager.hpp"




std::shared_ptr<Scene> createSceneFramedMirror(unsigned int imageWidth,
  unsigned int raysPerPixel,
  unsigned int cores) 
{
  auto scene = std::make_shared<Scene>();
  
  auto diffuseGreen = std::make_shared<Material>(0.0, 0.0, 1.0, 0.0, Vec3(0., 0.5, 0.));
  auto greenMetal = std::make_shared<Material>(0.0, 0.5, 0.0, 0.0, Vec3(0., 0.5, 0.));
  auto blueStuff = std::make_shared<Material>(0.0, 0.0, 0.3, 0.5, Vec3(0., 0., 0.7));
  auto redStuff = std::make_shared<Material>(0.0, 0.0, 0.5, 0.5, Vec3(0.7, 0., 0.));
  auto mirror = std::make_shared<Material>(0.0, 1.0, 0.0, 0.0, Vec3(1, 1, 1));
  auto mirrorAttenuated = std::make_shared<Material>(0.3, 0.8, 0.0, 0.0, Vec3(1, 1, 1));

  scene->materials.push_back(diffuseGreen);
  scene->materials.push_back(greenMetal);
  scene->materials.push_back(blueStuff);
  scene->materials.push_back(redStuff);
  scene->materials.push_back(mirror);
  scene->materials.push_back(mirrorAttenuated);

  double groundRadius = 300;
  Vec3 groundCenter(0.0, -groundRadius, 1.0);
  auto bigBallRadius = 2.5;
  auto bigBallDepth = 5.0;
  // ground
  auto ground = std::make_shared<Sphere>(groundCenter, groundRadius, *greenMetal);
  scene->addBigShape(ground);
  scene->collisionManager.addSphere(ground);
  // reflective big ball
  /*
  auto bigBall = std::make_shared<Sphere>(Vec3(0, bigBallRadius, bigBallDepth), bigBallRadius, mirror);
  bigWorld.addShape(bigBall.get());
  shapeContainer.addSphere(bigBall);
  */
  // reflective wall
  auto quadW = 15.0;
  auto quadH = 5.0;
  auto quadDepth = 5.1 + bigBallDepth;
  auto quad = std::make_shared<FramedQuad>(Vec3(-quadW / 2.0, 0.0, quadDepth),
      Vec3(quadW, 0.0, 0.0),
      Vec3(0.0, quadH, 0.0),
      0.2,
      *mirrorAttenuated,
      *redStuff);
  scene->addBigShape(quad);

  
  unsigned int addedSpheres = 0;
  while (addedSpheres < 300) {
    auto x = getRand(-20.0, 20.0);
    double radius = 0.5;
    //auto y = getRand(radius, radius + 5.0);
    auto y = 0.0;
    auto z = getRand(-20, quadDepth - 5.0);
    Vec3 position(x,y,z);
    auto cp = groundCenter - position;
    position += cp.getNormalized() * (cp.norm() - groundRadius - radius);
    std::shared_ptr<Material> material;
    Vec3 color = Vec3::getRandomVector(0.0, 1.0);
    auto r = getRand();
    if (r < 1.0) {
      material = std::make_shared<Material>(0.0, 0.0, 0.5, 0.5, color);
    } else {
      material = std::make_shared<Material>(0.0, 1.0, 0.0, 0.0, Vec3(1.0, 1.0, 1.0));
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
  Vec3 lookFrom(0, 6, -20);
  Vec3 lookAt(0.0, 1.0, 0.0);
  scene->camera = std::make_shared<Camera>(aspectRatio, imageWidth, fov, raysPerPixel, lookFrom, lookAt, cores);

  return scene;
}