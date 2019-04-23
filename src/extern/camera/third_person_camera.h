#ifndef TOARU_THIRD_PERSON_CAMERA_H
#define TOARU_THIRD_PERSON_CAMERA_H

#include "two_point_camera.h"

namespace toaru {
  class ThirdPersonCamera : public TwoPointCamera {
  public:
    static const float MOVE_SPEED;
    static const float SCROLL_SPEED;
    static const float ROTATE_SPEED;

    float azimuth, incline, distance;

    ThirdPersonCamera();

    virtual void update();

  protected:
    void updateCameraTarget(double dt);
    void updateAngle(double dt);
    void updateDistance(double dt);
    void updateCameraPosition();
  };
}

#endif