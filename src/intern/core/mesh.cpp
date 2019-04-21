#include "core/mesh.h"

using namespace toaru;

Mesh::Mesh() : Component() {}

int Mesh::numVertices() {
  return positions.size();
}

int Mesh::numFaces() {
  return indices.size();
}