#ifndef TOARU_TWO_POINT_CAMERA_H
#define TOARU_TWO_POINT_CAMERA_H

#include "core/camera.h"

namespace toaru {
  class TwoPointCamera : public Camera {
  public:
    Vector3f target;
    TwoPointCamera();

  protected:
    virtual Matrix4f computeView();
  };
}

#endif