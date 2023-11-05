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
  double smallRadius = 10.0;

  auto R = cos(PI/4.0);
  Sphere sphere1(Vec3(0.0,    0.0, -.0), 0.5); shapes.addShape(sphere1);
  Sphere bigSphere(Vec3(0.0, -100.5, -1.0), 100.0); shapes.addShape(bigSphere);
  //Sphere sphere1(Vec3(0, smallRadius, 50.0), smallRadius); shapes.addShape(sphere1);
  //Sphere sphere2(Vec3(-35, smallRadius, 50.0), smallRadius); shapes.addShape(sphere2);
  double bigRadius = 1000;
  //Sphere sphere3(Vec3(0, -bigRadius, 50.0), bigRadius); shapes.addShape(sphere3);
 
  Camera camera(1, 200, 20);
  camera.render(shapes);
  std::cout << "done" << std::endl;
  return 0;
}
