#include <iostream>
#include <assert.h>
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




int main(int argc, char **argv)
{
  unittest();

  std::cout << "Start ray racing" << std::endl;
  // init the shapes 
  Shapes shapes;
  auto R = cos(PI/4.0);
  Material defaultMaterial;
  Material ambiantGreen(0.0, 0.0, 0.0, 1.0, Vec3(0., 1.0, 0.));
  Material greenMetal(0.0, 0.5, 0.0, 0.5, Vec3(0., 1.0, 0.));
  greenMetal.setFuzz(0.1);
  Material blueStuff(0.0, 0.0, 0.3, 0.5, Vec3(0., 0., 1.0));
  Material redStuff(0.0, 0.0, 0.0, 0.5, Vec3(1.0, 0., 0.));
  Material mirror(0.2, 0.8, 0.0, 0.0, Vec3(1, 1, 1));
  Sphere sphere1(Vec3(-1.0, 0.0, -1.0), 0.5); sphere1.setMaterial(redStuff); shapes.addShape(sphere1);
  Sphere sphere2(Vec3(0.0, 0.0, -1.0), 0.5); sphere2.setMaterial(greenMetal); shapes.addShape(sphere2);
  //Sphere sphere3(Vec3(1.0, 0.0, -1.0), 0.5); sphere3.setMaterial(blueStuff); shapes.addShape(sphere3);
  Sphere bigSphere(Vec3(0.0, -100.5, -1.0), 100.0); bigSphere.setMaterial(mirror); shapes.addShape(bigSphere);
 
  Camera camera(1, 400, 60, 10);
  camera.render(shapes);
  std::cout << "done" << std::endl;
  return 0;
}
