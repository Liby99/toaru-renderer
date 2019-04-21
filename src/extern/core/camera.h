#ifndef TOARU_CAMERA_H
#define TOARU_CAMERA_H

#include "component.h"

namespace toaru {
  class Camera : public Component {
  public:
    virtual void init();
    virtual void update();

  };
}

#endif