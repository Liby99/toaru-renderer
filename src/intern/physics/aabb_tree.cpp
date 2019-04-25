#include "physics/aabb_tree.h"

using namespace toaru;

AABBTree::AABBTree(std::vector<Tetrahedron *> &allTetras) {
  root = make_unique<AABBTreeNode>(allTetras);
}

std::optional<Collision> AABBTree::collide(Tetrahedron &tetra) {
  return std::nullopt;
}