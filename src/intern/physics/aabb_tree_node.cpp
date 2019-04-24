#include "physics/aabb_tree_node.h"

using namespace toaru;

AABBTreeNode::AABBTreeNode(std::vector<Tetrahedron *> &allTetras)
  : AABBTreeNode(allTetras, 0, allTetras.size()) {}

AABBTreeNode::AABBTreeNode(std::vector<Tetrahedron *> &allTetras, int start, int amount) 
  : aabb(), startIndex(start), tetraAmount(amount), allTetras(allTetras) {
  int endIndex = start + amount;

  // First build the bounding box
  for (int i = start; i < endIndex; i++) {
    aabb.extend(*allTetras[i]);
  }

  // Get the dimension and midpoint from the bounding box
  Vector3f dim = aabb.getDimension();
  int axis = dim.x() > dim.y() ? (dim.x() > dim.z() ? 0 : 2) : (dim.y() > dim.z() ? 1 : 2);
  float mid = aabb.minCorner(axis) + dim(axis) / 2.0f;

  // Check the amount
  if (amount == 0) {
    leafFlag = false;
  } else if (amount == 1) {
    leafFlag = true;
  } else if (amount == 2) {
    leafFlag = false;
    left = make_unique<AABBTreeNode>(allTetras, start, 1);
    right = make_unique<AABBTreeNode>(allTetras, start + 1, 1);
  } else {

    // Put all the left tetras to the left of this part of the array
    int curr = start;
    for (int i = start; i < endIndex; i++) {
      if (allTetras[i]->getCenter()(axis) < mid) {
        auto temp = allTetras[i];
        allTetras[i] = allTetras[curr];
        allTetras[curr] = temp;
        curr++;
      }
    }

    // Check if left amount is 0 or all
    int leftAmount = curr - start;
    if (leftAmount == 0 || leftAmount == amount) {
      leafFlag = true;
    } else {
      left = make_unique<AABBTreeNode>(allTetras, start, leftAmount);
      right = make_unique<AABBTreeNode>(allTetras, start + leftAmount, amount - leftAmount);
      leafFlag = false;
    }
  }
}

void AABBTreeNode::refit() {
  aabb.reset();
  int endAmount = startIndex + tetraAmount;
  for (int i = startIndex; i < endAmount; i++) {
    aabb.extend(*allTetras[i]);
  }
}

bool AABBTreeNode::isLeaf() {
  return leafFlag;
}

const AABBTreeNode &AABBTreeNode::getLeft() {
  return *left;
}

const AABBTreeNode &AABBTreeNode::getRight() {
  return *right;
}

const AABB &AABBTreeNode::getBoundingBox() {
  return aabb;
}

bool AABBTreeNode::isLeftRightIntersecting() {
  if (leafFlag) return false;
  return left->getBoundingBox().intersect(right->getBoundingBox());
}