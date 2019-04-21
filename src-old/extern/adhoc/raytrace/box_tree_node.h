#ifndef BOX_TREE_NODE_H
#define BOX_TREE_NODE_H

#include "adhoc/raytrace/bounding_box.h"
#include "adhoc/raytrace/triangle.h"

namespace nash {
  class BoxTreeMesh;
  class BoxTreeNode {
  public:
    BoxTreeNode(const MatrixXu &indices, const BoxTreeMesh *boxTreeMesh);
    bool intersect(const Ray &ray, Intersection &intersection);
    bool isLeaf() const;

  private:
    void partition();
    BoxTreeNode *left;
    BoxTreeNode *right;

    BoundingBox *boundingBox;
    Triangle *triangle;
    const MatrixXu &indices;
    const BoxTreeMesh *boxTreeMesh;
    // std::vector<Vector3f> primitives;
  };
}

#endif
