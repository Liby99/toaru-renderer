#include "physics/aabb_tree.h"

using namespace toaru;

AABBTree::AABBTree(std::vector<Tetrahedron *> &allTetras) {
  root = make_unique<AABBTreeNode>(allTetras);
}

void AABBTree::handleCollision(Tetrahedron &tetra) {
  root->handleCollision(tetra);
}

void AABBTree::refit() {
  root->refit();
}