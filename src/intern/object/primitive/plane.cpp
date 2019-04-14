#include "object/primitive/plane.h"

using namespace nash;

Plane::Plane() : Mesh() {
  indices = MatrixXu(3, 2);
  indices.col(0) << 0, 1, 3;
  indices.col(1) << 1, 2, 3;

  positions = MatrixXf(3, 4);
  positions.col(0) << -0.5, 0, -0.5;
  positions.col(1) << -0.5, 0, 0.5;
  positions.col(2) << 0.5, 0, 0.5;
  positions.col(3) << 0.5, 0, -0.5;

  normals = MatrixXf(3, 4);
  normals.col(0) << 0, 1, 0;
  normals.col(1) << 0, 1, 0;
  normals.col(2) << 0, 1, 0;
  normals.col(3) << 0, 1, 0;

  texCoords = MatrixXf(2, 4);
  texCoords.col(0) << 0.0, 0.0;
  texCoords.col(1) << 0.0, 1.0;
  texCoords.col(2) << 1.0, 1.0;
  texCoords.col(3) << 1.0, 0.0;
}
