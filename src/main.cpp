#include <iostream>
#include <assert.h>
#include <chrono>
#include "Common.hpp"
#include "Shape.hpp"
#include "Camera.hpp"


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
  std::vector<std::shared_ptr<Sphere> > _spheres;
};

int main(int argc, char **argv)
{
  unittest();

  auto start = std::chrono::high_resolution_clock::now();
  std::cout << "Start ray racing" << std::endl;
  // init the shapes 
  Shapes shapes;
  auto R = cos(PI/4.0);
  Material defaultMaterial;
  Material ambiantGreen(0.0, 0.0, 0.0, 1.0, Vec3(0., 1.0, 0.));
  Material diffuseGreen(0.0, 0.0, 0.5, 0.5, Vec3(0., 0.5, 0.));
  Material greenMetal(0.0, 0.5, 0.0, 0.5, Vec3(0., 1.0, 0.));
  //greenMetal.setFuzz(0.1);
  Material blueStuff(0.0, 0.0, 0.3, 0.5, Vec3(0., 0., 1.0));
  Material redStuff(0.0, 0.0, 0.0, 0.5, Vec3(1.0, 0., 0.));
  Material greyStuff(0.0, 0.0, 0.5, 0.5, Vec3(0.7, 0.7, 0.7));
  Material mirror(0.0, 0.8, 0.0, 0.0, Vec3(1, 1, 1));
  Material redReflectiveBall(0.0, 0.8, 0.0, 0.2, Vec3(1, 0, 0));
  CollisionChecker collisionChecker;
  // ground
  auto ground = std::make_shared<Sphere>(Vec3(0.0, -1000.0, 1.0), 1000, greyStuff);
  shapes.addShape(ground);
  collisionChecker.addSphere(ground);
  // reflective big ball
  auto bigBall = std::make_shared<Sphere>(Vec3(0, 2.0  , 3.0), 2.0, mirror);
  shapes.addShape(bigBall);
  collisionChecker.addSphere(bigBall);
  

  unsigned int addedSpheres = 0;
  while (addedSpheres < 40  ) {
    auto x = getRand(-5.0, 5.0);
    double radius = getRand(0.1, 0.5);
    auto z = getRand(-10.0, 10.0);
    std::cout << x << " " << z << std::endl;
    std::shared_ptr<Material> material;
    Vec3 color = Vec3::getRandomVector(0.0, 1.0);
    auto r = getRand();
    if (r < 0.8) {
      material = std::make_shared<Material>(0.0, 0.0, 0.6, 0.4, color);
    } else {
      material = std::make_shared<Material>(0.0, 0.8, 0.0, 0.0, color);
    }
    auto shape = std::make_shared<Sphere>(Vec3(x, radius, z), radius, *material);
    if (collisionChecker.canAddSphere(shape)) {
      shapes.addShape(shape);
      collisionChecker.addSphere(shape);
      addedSpheres++;
    } 
  }

  double fov = 20;
  double raysPerPixel = 20;
  double aspectRatio = 1.5;
  unsigned int imageWidth = 1200;
  
  Vec3 lookFrom(0, 4, -20);
  Vec3 lookAt(0.0, 1.0, 0.0);
  unsigned int cores = 10;
  Camera camera(aspectRatio, imageWidth, fov, raysPerPixel, lookFrom, lookAt, cores);
  camera.render(shapes);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "done in " << duration.count() << "ms" << std::endl;
  return 0;
}
