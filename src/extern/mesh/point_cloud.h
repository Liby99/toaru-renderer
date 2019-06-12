#ifndef TOARU_POINT_CLOUD
#define TOARU_POINT_CLOUD

#include "core/component.h"

namespace toaru {
  class PointCloud : public Component {
  public:
    MatrixXu indices;
    MatrixXf positions, colors;
    PointCloud(const std::vector<Vector3f> &ps, const std::vector<Vector3f> &cs);
    int numPositions() const;
  };
}

#endif