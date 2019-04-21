#ifndef TOARU_TRANSFORMATION_H
#define TOARU_TRANSFORMATION_H

#include "math.h"

namespace toaru {
  class Transform {
  public:
    Matrix4f world;
    Quaternionf rotation;
    Vector3f position;
    Vector3f scale;

    Transform();
    Matrix4f getTransform();
  };
}

#endif
