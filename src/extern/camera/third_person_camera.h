#ifndef TOARU_THIRD_PERSON_CAMERA_H
#define TOARU_THIRD_PERSON_CAMERA_H

#include "two_point_camera.h"

namespace toaru {
  class ThirdPersonCamera : public TwoPointCamera {
  public:
    static const float MOVE_SPEED, SCROLL_SPEED, ROTATE_SPEED;
    static const float MAX_INCLINE, MIN_INCLINE;

    float azimuth, incline, distance;
    float moveSpeed, scrollSpeed, rotateSpeed;
    bool allowMove, allowScroll, allowRotate;

    ThirdPersonCamera();

    virtual void init();
    virtual void update();

  protected:
    void updateCameraTarget(double dt);
    void updateAngle(double dt);
    void updateDistance(double dt);
    void updateCameraPosition();
  };
}

#endif