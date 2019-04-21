#ifndef TOARU_MESH_H
#define TOARU_MESH_H

#include "component.h"

namespace toaru {
  class Mesh : public Component {
  public:
    MatrixXf positions;
    MatrixXf normals;
    MatrixXf texCoords;
    MatrixXu indices;

    Mesh();

    int numVertices();
    int numFaces();
  };
}

#endif