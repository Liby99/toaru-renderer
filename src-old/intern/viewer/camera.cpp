#include "viewer/camera.h"

using namespace nash;

Camera::Camera()
    : position(0, 0, -1), target(0, 0, 0), up(0, 1, 0), width(1280), height(720), zNear(0.1),
      zFar(100), aspect(1.7777778), fovy(45), control(nullptr) {
  // Do nothing
}

void Camera::start(Context &context) {
  if (hasController()) {
    control->setContext(context);
    control->bind(*this);
    control->start();
  }
}

void Camera::update(Context &context) {
  if (hasController()) {
    control->setContext(context);
    control->bind(*this);
    control->update();
  }
}

bool Camera::hasController() { return control != nullptr; }

void Camera::setController(Script<Camera> &ctrl) {
  ctrl.onAttach(*this);
  control = &ctrl;
}

Script<Camera> &Camera::getController() { return *control; }

void Camera::setSize(int width, int height) {
  this->width = width;
  this->height = height;
  this->aspect = (float)width / (float)height;
}

Matrix4f Camera::getViewPerspective() { return getPerspective() * getView(); }

Matrix4f Camera::getView() {
  Vector3f z = position - target, x = up.cross(z), y = z.cross(x);
  x.normalize();
  y.normalize();
  z.normalize();
  Matrix4f mat;
  mat << x[0], x[1], x[2], -x.dot(position), y[0], y[1], y[2], -y.dot(position), z[0], z[1], z[2],
      -z.dot(position), 0, 0, 0, 1;
  return mat;
}

Matrix4f Camera::getPerspective() {
  float theta = fovy / 2, d = cos(theta) / sin(theta);
  float a = (zFar + zNear) / (zNear - zFar), b = 2 * zFar * zNear / (zNear - zFar);
  Eigen::Matrix4f mat;
  mat << d / aspect, 0, 0, 0, 0, d, 0, 0, 0, 0, a, b, 0, 0, -1, 0;
  return mat;
}
