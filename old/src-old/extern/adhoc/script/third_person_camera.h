#ifndef THIRD_PERSON_CAMERA_H
#define THIRD_PERSON_CAMERA_H

#include "utility/script.h"
#include "viewer/camera.h"

namespace nash {
  class ThirdPersonCamera : public Script<Camera> {
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
