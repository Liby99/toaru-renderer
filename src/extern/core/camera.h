#ifndef TOARU_CAMERA_H
#define TOARU_CAMERA_H

#include "utility/math.h"
#include "component.h"

namespace toaru {
  class Camera : public Component {
  public:
    const static float DEFAULT_FOVY, DEFAULT_NEAR, DEFAULT_FAR;
    const static Vector3f BACKWARD, UP;

    Camera();

    virtual void update();

    Matrix4f getView();
    Matrix4f getProj();

  protected:
    virtual Matrix4f computeView();
    virtual Matrix4f computeProj();

    float fovy, near, far;
    Matrix4f view, proj;
  };
}

#endif