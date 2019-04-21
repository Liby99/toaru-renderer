#ifndef CAMERA_H
#define CAMERA_H

#include "utility/math.h"
#include "utility/script.h"

using namespace Eigen;

namespace nash {
  class Camera {
  public:
    Vector3f position;
    Vector3f target;
    Vector3f up;

    int width, height;
    float zNear, zFar;
    float aspect;
    float fovy;

    Camera();

    void start(Context &context);
    void update(Context &context);
    bool hasController();
    void setController(Script<Camera> &ctrl);
    Script<Camera> &getController();
    void setSize(int width, int height);
    Matrix4f getViewPerspective();

  protected:
    Script<Camera> *control;
    Matrix4f getView();
    Matrix4f getPerspective();
  };
}

#endif
