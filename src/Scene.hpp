#pragma once

#include "shapes/Shapes.hpp"
#include "shapes/BVH.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "SphereCollisionManager.hpp"


struct Scene {
  Scene() {
  }

  void addSmallShape(std::shared_ptr<Shape> shape) {
    smallShapes.addShape(shape.get());
    allShapes.push_back(shape);
  }

  void addBigShape(std::shared_ptr<Shape> shape) {
    world.addShape(shape.get());
    allShapes.push_back(shape);
  }

  void beforeRender() {
    auto bvh = std::make_shared<BVH>(smallShapes.getShapes());
    addBigShape(bvh);
    world.addShape(&smallShapes);
  }

  // the scene
  Shapes world;
  Shapes smallShapes;
  std::shared_ptr<Camera> camera; 

  // buffers to keep a pointer to the objects that should
  // not be deleted
  std::vector<std::shared_ptr<Shape> > allShapes;
  SphereCollisionManager collisionManager;
  std::vector<std::shared_ptr<Material> > materials;
};
