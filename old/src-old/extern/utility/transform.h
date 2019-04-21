#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include "utility/math.h"

namespace nash {
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
