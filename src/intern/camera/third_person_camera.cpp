#include "camera/third_person_camera.h"

using namespace toaru;

const float ThirdPersonCamera::MOVE_SPEED = 2.0f;

const float ThirdPersonCamera::SCROLL_SPEED = 1.5f;

const float ThirdPersonCamera::ROTATE_SPEED = 0.8f;

const float ThirdPersonCamera::MAX_INCLINE = Math::PI / 2 - 0.01f;

const float ThirdPersonCamera::MIN_INCLINE = -Math::PI / 2 + 0.01f;

ThirdPersonCamera::ThirdPersonCamera()
  : azimuth(0), incline(0), distance(3),
    moveSpeed(MOVE_SPEED), scrollSpeed(SCROLL_SPEED), rotateSpeed(ROTATE_SPEED),
    allowMove(true), allowScroll(true), allowRotate(true),
    TwoPointCamera() {}

void ThirdPersonCamera::init() {
  Vector3f diff = object().transform.position - target;
  distance = diff.norm();
  azimuth = atan2(diff.x(), diff.y());
  incline = atan(diff.y() / Vector2f(diff.z(), diff.x()).norm());
}

void ThirdPersonCamera::update() {

  // First update the position and target
  double dt = context().getDeltaTime();
  if (allowRotate) updateAngle(dt);
  if (allowScroll) updateDistance(dt);
  if (allowMove) updateCameraTarget(dt);
  updateCameraPosition();

  // Then use parent's update
  TwoPointCamera::update();
}

void ThirdPersonCamera::updateCameraTarget(double dt) {
  Vector3f vel = Vector3f(0, 0, 0);
  if (context().getKey(' '))
    vel.y() += 1;
  if (context().getKey('x'))
    vel.y() -= 1;
  if (context().getKey('w'))
    vel -= Vector3f(sin(azimuth), 0, cos(azimuth));
  if (context().getKey('s'))
    vel += Vector3f(sin(azimuth), 0, cos(azimuth));
  if (context().getKey('a'))
    vel += Vector3f(-cos(azimuth), 0, sin(azimuth));
  if (context().getKey('d'))
    vel += Vector3f(cos(azimuth), 0, -sin(azimuth));
  if (vel.dot(vel) > 0)
    target += vel.normalized() * moveSpeed * dt;
}

void ThirdPersonCamera::updateAngle(double dt) {
  if (context().getMouseLeft()) {
    Vector2i curRel = context().getCursorMovement();
    azimuth -= curRel.x() * rotateSpeed * dt;
    float rawIncline = incline + curRel.y() * rotateSpeed * dt;
    incline = fmaxf(MIN_INCLINE, fminf(MAX_INCLINE, rawIncline));
  }
}

void ThirdPersonCamera::updateDistance(double dt) {
  distance = fmaxf(0.01f, distance - context().getScrollMovement().y() * scrollSpeed * dt);
}

void ThirdPersonCamera::updateCameraPosition() {
  float x = target.x() + sin(azimuth) * cos(incline) * distance;
  float y = target.y() + sin(incline) * distance;
  float z = target.z() + cos(azimuth) * cos(incline) * distance;
  object().transform.position = Vector3f(x, y, z);
}