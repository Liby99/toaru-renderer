#ifndef COLORED_POINT_CLOUD
#define COLORED_POINT_CLOUD

#include "object/adhoc/point_cloud.h"

namespace nash {
  class ColoredPointCloud : public PointCloud {
  public:
    ColoredPointCloud(const std::vector<Vector3f> &ps, const std::vector<Vector4u> &cs);
    virtual void render();

  private:
    MatrixXf colors;
  };
}

#endif