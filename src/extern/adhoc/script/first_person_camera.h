#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "adhoc/script/third_person_camera.h"

namespace nash {
  class FirstPersonCamera : public ThirdPersonCamera {
  public:
    FirstPersonCamera();
    virtual void update();
  };
}

#endif