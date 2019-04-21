#ifndef MESH_H
#define MESH_H

#include "object/object.h"

namespace nash {
  class Mesh : public Object {
  public:
    Mesh();
    virtual void render();

    int getNumVertices() const;
    int getNumFaces() const;

    const MatrixXu &getIndices() const;
    const MatrixXf &getPositions() const;
    const MatrixXf &getNormals() const;

  protected:
    MatrixXu indices;
    MatrixXf positions;
    MatrixXf normals;
    MatrixXf texCoords;
  };
}

#endif
