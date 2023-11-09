#pragma once

#include <unordered_set>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "Shape.hpp"
#include "AABB.hpp"

using ShapeSet = std::unordered_set<Shape *>;

class BVHNode {
public:
  /**
   *  Constructor
   *  @param shapes: the shapes to store
   *  @param axis the current axis (0, 1, 2 for x, y, z) to split
   */
  BVHNode(const std::vector<Shape *> &shapes, unsigned int axis) {
    assert(axis < 3);
    if (shapes.size() == 1) {
      // leaf case
      _shapes.insert(shapes[0]);
      _aabb = shapes[0]->getAABB();
      return;
    }
    // internal node
    auto sortedShapes = shapes;
    // sort the shapes according to their center on the current axis
    std::sort(sortedShapes.begin(), sortedShapes.end(), 
        [axis](Shape *s1, Shape *s2) {
            return s1->getAABB().getInterval(axis).getCenter() > 
                   s2->getAABB().getInterval(axis).getCenter();
          }
        ); 
    // update the bounding box
    for (auto shape: shapes) {
      _aabb.unionWith(shape->getAABB());
    }
    std::cout << "Box with " << shapes.size() << " shapes, " << _aabb << std::endl;
    // split the shapes in to sets of same size
    std::vector<Shape *> leftShapes;
    std::vector<Shape *> rightShapes;
    auto middle = (sortedShapes.size() + 1) / 2;
    for (unsigned int i = 0; i < middle; ++i) {
      leftShapes.push_back(sortedShapes[i]);
    }
    for (unsigned int i = middle; i < sortedShapes.size(); ++i) {
      rightShapes.push_back(sortedShapes[i]);
    }
    // recursive call. We change the axis
    auto nextAxis = (axis + 1) % 3;
    _left = std::make_shared<BVHNode>(leftShapes, nextAxis);
    _right = std::make_shared<BVHNode>(rightShapes, nextAxis);
  }

  /**
   *  Return true if this is a terminal node
   */
  bool isLeaf() const {return _left.get() == nullptr;}

  /**
   *  Get all candidate shapes that the ray might intersect
   */
  void getHitCandidates(const Ray &ray, ShapeSet &candidates) const {
    if (isLeaf()) {
      // leaf case
      for (auto shape: _shapes) {
        candidates.insert(shape);
      }   
      return;
    }
    // internal node case
    if (!_aabb.hit(ray)) {
      // no intersection with the shapes under this node
      return;
    }
    // there might be some intersection, continue the traversal
    assert(_left.get());
    assert(_right.get());
    _left->getHitCandidates(ray, candidates);
    _right->getHitCandidates(ray, candidates);
  }
private:
  std::shared_ptr<BVHNode> _left;
  std::shared_ptr<BVHNode> _right;
  ShapeSet _shapes; // only relevant for the leaves
  AABB _aabb; // bounding box of this node
};

/**
 *  Bounding volume hierarchies
 *  Structure used to access the list of shapes that a ray might
 *  intersect in log(n) where n is the number of shapes.
 */
class BVH {
public:
  /**
   *  Constructor
   *  @param shapes Shapes to be stored in the BVH
   */
  BVH(const std::vector<Shape *> &shapes): _root(shapes, 0) { }
  
  /**
   *  Return all the shapes that the ray might interesct (log complexity)
   */
  void getHitCandidates(const Ray &ray, ShapeSet &candidates) const {
    _root.getHitCandidates(ray, candidates);
  }
private:
  BVHNode _root; // root of the binary tree
};




