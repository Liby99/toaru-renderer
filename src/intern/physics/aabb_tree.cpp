#include "physics/aabb_tree.h"

using namespace toaru;

AABBTree::AABBTree(std::vector<const Tetrahedron *> &allTetras) {
  root = make_unique<AABBTreeNode>(allTetras);
}

std::optional<Collision> AABBTree::collide(const Tetrahedron &tetra) {
  return root.collide(tetra);
}