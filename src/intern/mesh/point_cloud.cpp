#include "mesh/point_cloud.h"

using namespace toaru;

PointCloud::PointCloud(
  const std::vector<Vector3f> &ps,
  const std::vector<Vector3f> &cs
) : Component() {
  indices = MatrixXu(1, ps.size());
  positions = MatrixXf(3, ps.size());
  colors = MatrixXf(3, cs.size());
  for (int i = 0; i < ps.size(); i++) {
    indices.col(i) << i;
    positions.col(i) = ps[i];
    colors.col(i) = cs[i];
  }
}

int PointCloud::numPositions() const {
  return positions.cols();
}