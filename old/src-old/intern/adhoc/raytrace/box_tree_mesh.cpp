#include <adhoc/raytrace/box_tree_mesh.h>

using namespace nash;

BoxTreeMesh::BoxTreeMesh(const Mesh &mesh) : mesh(mesh) { partition(); }

bool BoxTreeMesh::intersect(const Ray &ray, Intersection &intersection) {
  return root->intersect(ray, intersection);
}

BoxTreeMesh::~BoxTreeMesh() { delete root; }

const Mesh &BoxTreeMesh::getMesh() const { return mesh; }

void BoxTreeMesh::partition() { root = new BoxTreeNode(mesh.getIndices(), this); }
