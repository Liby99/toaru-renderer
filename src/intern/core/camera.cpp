#include "core/camera.h"

using namespace toaru;

const float Camera::DEFAULT_FOVY = 45.0f;

const float Camera::DEFAULT_NEAR = 0.1f;

const float Camera::DEFAULT_FAR = 1000.0f;

const Vector3f Camera::BACKWARD = Vector3f(0, 0, 1);

const Vector3f Camera::UP = Vector3f(0, 1, 0);

Camera::Camera() : fovy(DEFAULT_FOVY), zNear(DEFAULT_NEAR), zFar(DEFAULT_FAR), Component() {}

void Camera::update() {
  view = computeView();
  proj = computeProj();
  context().setMainCameraMatrices(view, proj);
}

Matrix4f Camera::getView() {
  return view;
}

Matrix4f Camera::getProj() {
  return proj;
}

Matrix4f Camera::computeView() {
  Quaternion rotation = object().transform.rotation;
  Vector3f position = object().transform.position;
  Vector3f z = rotation * BACKWARD;
  Vector3f x = UP.cross(z).normalized();
  Vector3f y = z.cross(x);
  Matrix4f mat;
  mat << x[0], x[1], x[2], -x.dot(position), y[0], y[1], y[2], -y.dot(position), z[0], z[1], z[2],
      -z.dot(position), 0, 0, 0, 1;
  return mat;
}

Matrix4f Camera::computeProj() {
  int width = context().getCanvasWidth();
  int height = context().getCanvasHeight();
  float aspect = width / (float)height;
  float theta = fovy / 2, d = cos(theta) / sin(theta);
  float a = (zFar + zNear) / (zNear - zFar);
  float b = 2 * zFar * zNear / (zNear - zFar);
  Eigen::Matrix4f mat;
  mat << d / aspect, 0, 0, 0, 0, d, 0, 0, 0, 0, a, b, 0, 0, -1, 0;
  return mat;
}