#ifndef TOARU_PHYSICS_AABB_TREE_NODE_H
#define TOARU_PHYSICS_AABB_TREE_NODE_H

#include <optional>
#include "collision.h"
#include "aabb.h"
#include "tetrahedron.h"

namespace toaru {
  class AABBTreeNode {
  public:
    AABB aabb;
    int startIndex, tetraAmount;
    std::vector<const Tetrahedron *> &allTetras;
    std::unique_ptr<AABBTreeNode> left, right;
    bool leafFlag;

    AABBTreeNode(std::vector<const Tetrahedron *> &allTetras);
    AABBTreeNode(std::vector<const Tetrahedron *> &allTetras, int start, int amount);

    void refit();

    bool isLeaf();
    const AABBTreeNode &getLeft();
    const AABBTreeNode &getRight();
    const AABB &getBoundingBox();
    bool isLeftRightIntersecting();

	std::optional<Collision> collide(const Tetrahedron &tetra);
  };
}

#endif