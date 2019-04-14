#include "utility/transform.h"

using namespace nash;

nash::Transform::Transform()
    : world(Matrix4f::Identity()), rotation(Quaternionf::Identity()), position(Vector3f::Zero()),
      scale(Vector3f::Zero()) {
  scale << 1, 1, 1;
}

Matrix4f nash::Transform::getTransform() {
  Matrix4f local = Matrix4f::Zero();
  local.col(3) << position.x(), position.y(), position.z(), 1;
  local.topLeftCorner<3, 3>() = rotation.normalized().toRotationMatrix() * Scaling(scale);
  return world * local;
}
