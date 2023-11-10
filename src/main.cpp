#include <iostream>
#include <assert.h>
#include <chrono>
#include "Common.hpp"
#include "Shape.hpp"
#include "Shapes.hpp"
#include "Camera.hpp"
#include "BVH.hpp"
#include "FramedQuad.hpp"

/*
 *  A few unit tests
 *  A bit dirty but this is a toy project
 */
void unittest()
{
  Vec3 v1(1.0, 2.0, 3.0);
  Vec3 v2(2.0, 2.0, 0.0);
  assert(v1 + v2 == Vec3(3,4,3));
  assert(v1 - v2 == Vec3(-1,0,3));
  assert(v1 * v2 == 6.0);
  Vec3 cross = v1 ^ v2;
  assert(cross == Vec3(-6,6,-2));

  AABB aabb(Interval(-1, 1),Interval(-1, 1), Interval(-1, 1));
  Ray ray(Vec3(0.0, 0.0, 0.0), Vec3(1.0, 1.0, 1.0));
  assert(aabb.hit(ray));
}


class CollisionChecker {
public:
  CollisionChecker() {}
  virtual ~CollisionChecker() {}
  bool canAddSphere(std::shared_ptr<Sphere> sphere) {
    for (auto sp: _spheres) {
      if (sp->intersect(*sphere)) {
        return false;
      }
    }
    return true;
  }
  void addSphere(std::shared_ptr<Sphere> sphere) {
      _spheres.push_back(sphere);
  }
private:
  std::vector<std::shared_ptr<Sphere> >_spheres;
};

int main(int, char **)
{
  unittest();

  auto start = std::chrono::high_resolution_clock::now();
  std::cout << "Creating the scene..." << std::endl;
  // we don't put the big objects in the BVH, because it messes up
  // the BVH
  Shapes world;
  Shapes bigWorld;
  world.addShape(&bigWorld);
  Shapes smallWorld;
  Material defaultMaterial;
  Material diffuseGreen(0.0, 0.0, 1.0, 0.0, Vec3(0., 0.5, 0.));
  Material greenMetal(0.0, 0.5, 0.0, 0.0, Vec3(0., 0.5, 0.));
  Material blueStuff(0.0, 0.0, 0.3, 0.5, Vec3(0., 0., 0.7));
  Material redStuff(0.0, 0.0, 0.5, 0.5, Vec3(0.7, 0., 0.));
  Material mirror(0.0, 1.0, 0.0, 0.0, Vec3(1, 1, 1));
  CollisionChecker collisionChecker;
  double groundRadius=300;
  Vec3 groundCenter(0.0, -groundRadius, 1.0);
  auto bigBallRadius = 2.5;
  auto bigBallDepth = 5.0;
  // ground
  auto ground = std::make_shared<Sphere>(groundCenter, groundRadius, greenMetal);
  bigWorld.addShape(ground.get());
  collisionChecker.addSphere(ground);
  // reflective big ball
  /*
  auto bigBall = std::make_shared<Sphere>(Vec3(0, bigBallRadius, bigBallDepth), bigBallRadius, mirror);
  bigWorld.addShape(bigBall.get());
  collisionChecker.addSphere(bigBall);
  */
  // reflective wall
  auto quadSize = bigBallRadius * 3.0;
  auto quadDepth = 5.1 + bigBallDepth;
  auto quad = std::make_shared<FramedQuad>(Vec3(-quadSize / 2.0, 0.001, quadDepth),
      Vec3(quadSize, 0.0, 0.0),
      Vec3(0.0, quadSize, 0.0),
      0.2,
      blueStuff,
      redStuff);
  bigWorld.addShape(quad.get());
  
  unsigned int addedSpheres = 0;
  while (addedSpheres < 100) {
    auto x = getRand(-20.0, 20.0);
    double radius = 0.3;
    //auto y = getRand(radius, radius + 5.0);
    auto y = 0.0;
    auto z = getRand(-10, 40);
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
    if (collisionChecker.canAddSphere(shape)) {
      smallWorld.addShape(shape.get());
      collisionChecker.addSphere(shape);
      addedSpheres++;
    } 
  }
  BVH smallWorldBVH(smallWorld.getShapes());
  world.addShape(&smallWorldBVH);
  double fov = 30;
  double raysPerPixel = 10;
  double aspectRatio = 1.5;
  unsigned int imageWidth = 600;
  Vec3 lookFrom(0, 6, -20);
  Vec3 lookAt(0.0, 1.0, 0.0);
  unsigned int cores = 4;
  std::cout << "Start ray tracing" << std::endl;
  Camera camera(aspectRatio, imageWidth, fov, raysPerPixel, lookFrom, lookAt, cores);
  camera.render(world);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "done in " << duration.count() << "ms" << std::endl;
  return 0;
}
