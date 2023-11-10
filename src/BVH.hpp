#pragma once

#include <unordered_set>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "AABB.hpp"

using ShapeSet = std::unordered_set<Shape *>;

class BVHNode {
public:
  /**
   *  Constructor
   *  @param shapes: the shapes to store
   *  @param axis the current axis (0, 1, 2 for x, y, z) to split
   */
  BVHNode(const std::vector<Shape *> &shapes, unsigned int axis): _isLeaf(false) {
    assert(axis < 3);
    for (auto shape: shapes) {
      _aabb.unionWith(shape->getAABB());
    }
    if (shapes.size() <= 4) {
      _isLeaf = true;
      // leaf case
      for (auto shape: shapes) {
        _shapes.insert(shape);
      }
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
  bool isLeaf() const {return _isLeaf;}

  bool hit(const Ray &ray, double minDist, Hit &hit) const {
 // void getHitCandidates(const Ray &ray, ShapeSet &candidates) const {
    bool ok = false;
    if (isLeaf()) {
      // leaf case
      for (auto shape: _shapes) {
        ok |= shape->hit(ray, minDist, hit);
      }   
      return ok;
    }
    // internal node case
    if (!_aabb.hit(ray)) {
      // no intersection with the shapes under this node
      return false;
    }
    // there might be some intersection, continue the traversal
    assert(_left.get());
    assert(_right.get());
    ok |= _left->hit(ray, minDist, hit);
    ok |= _right->hit(ray, minDist, hit);
    return ok;
  }
private:
  bool _isLeaf;
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
class BVH: public Shape {
public:
  /**
   *  Constructor
   *  @param shapes Shapes to be stored in the BVH
   */
  BVH(const std::vector<Shape *> &shapes): _root(shapes, 0) { }
  
  virtual bool hit(const Ray &ray, double minDist, Hit &hit) const {
    return _root.hit(ray, minDist, hit);
  }
private:
  BVHNode _root; // root of the binary tree
};




