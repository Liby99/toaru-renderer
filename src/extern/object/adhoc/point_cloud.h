#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include "object/object.h"

namespace nash {
  class PointCloud : public Object {
  public:
    static const int POINT_SIZE;

    PointCloud(const std::vector<Vector3f> &points);
    virtual void render();

  private:
    MatrixXu indices;
    MatrixXf points;
  };
}

#endif