#include "adhoc/script/third_person_camera.h"

using namespace nash;

const float ThirdPersonCamera::MOVE_SPEED = 2;
const float ThirdPersonCamera::SCROLL_SPEED = 1.5;
const float ThirdPersonCamera::ROTATE_SPEED = 0.8;

ThirdPersonCamera::ThirdPersonCamera()
    : Script<Camera>("third-person-camera"), azimuth(0), incline(0), distance(3) {}

void ThirdPersonCamera::update() {
  double dt = context->getDeltaTime();
  updateAngle(dt);
  updateDistance(dt);
  updateCameraTarget(dt);
  updateCameraPosition();
}

void ThirdPersonCamera::updateCameraTarget(double dt) {
  Vector3f vel = Vector3f(0, 0, 0);
  if (context->getKey(' '))
    vel.y() += 1;
  if (context->getKey('x'))
    vel.y() -= 1;
  if (context->getKey('w'))
    vel -= Vector3f(sin(azimuth), 0, cos(azimuth));
  if (context->getKey('s'))
    vel += Vector3f(sin(azimuth), 0, cos(azimuth));
  if (context->getKey('a'))
    vel += Vector3f(-cos(azimuth), 0, sin(azimuth));
  if (context->getKey('d'))
    vel += Vector3f(cos(azimuth), 0, -sin(azimuth));
  if (vel.dot(vel) > 0)
    target->target += vel.normalized() * MOVE_SPEED * dt;
}

void ThirdPersonCamera::updateAngle(double dt) {
  if (context->getMouseLeft()) {
    Vector2i curRel = context->getCursorMovement();
    azimuth -= curRel.x() * ROTATE_SPEED * dt;
    incline =
        fmaxf(-PI / 2 + 0.01f, fminf(PI / 2 - 0.01f, incline + curRel.y() * ROTATE_SPEED * dt));
  }
}

void ThirdPersonCamera::updateDistance(double dt) {
  distance = fmaxf(0.01f, distance - context->getScrollMovement().y() * SCROLL_SPEED * dt);
}

void ThirdPersonCamera::updateCameraPosition() {
  float x = target->target.x() + sin(azimuth) * cos(incline) * distance;
  float y = target->target.y() + sin(incline) * distance;
  float z = target->target.z() + cos(azimuth) * cos(incline) * distance;
  target->position = Vector3f(x, y, z);
}