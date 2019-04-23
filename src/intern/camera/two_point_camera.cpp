#include "camera/two_point_camera.h"

using namespace toaru;

TwoPointCamera::TwoPointCamera() : Camera() {}

Matrix4f TwoPointCamera::computeView() {
  Vector3f position = object().transform.position;
  Vector3f z = (position - target).normalized();
  Vector3f x = UP.cross(z).normalized();
  Vector3f y = z.cross(x);
  Matrix4f mat;
  mat << x[0], x[1], x[2], -x.dot(position), y[0], y[1], y[2], -y.dot(position), z[0], z[1], z[2],
      -z.dot(position), 0, 0, 0, 1;
  return mat;
}