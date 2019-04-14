#ifndef BOX_TREE_MESH_H
#define BOX_TREE_MESH_H

#include "adhoc/raytrace/box_tree_node.h"
#include "object/mesh.h"

namespace nash {
  class BoxTreeMesh {
  public:
    BoxTreeMesh(const Mesh &mesh);
    bool intersect(const Ray &ray, Intersection &intersection);
    ~BoxTreeMesh();
    const Mesh &getMesh() const;
    int leafSize = 1;

  private:
    void partition();
    const Mesh &mesh;
    BoxTreeNode *root;
  };
}

#endif
