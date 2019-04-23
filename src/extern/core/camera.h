#ifndef TOARU_CAMERA_H
#define TOARU_CAMERA_H

#include "component.h"
#include "utility/math.h"

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

    float fovy, zNear, zFar;
    Matrix4f view, proj;
  };
}

#endif