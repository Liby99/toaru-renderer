#include "adhoc/script/rotator.h"

using namespace nash;

Rotator::Rotator(const std::string &name) : Rotator(name, 1, Vector3f(0, 1, 0)) {}

Rotator::Rotator(const std::string &name, float speed, Vector3f axis)
    : Script(name), elapsedTime(0), rotating(true), speed(speed), axis(axis) {}

void Rotator::pause() { rotating = false; }

void Rotator::resume() { rotating = true; }

void Rotator::stop() {
  rotating = false;
  elapsedTime = 0;
}

void Rotator::reset() { elapsedTime = 0; }

void Rotator::update() {
  if (rotating) {
    elapsedTime += context->getDeltaTime();
    target->transform.rotation = AngleAxisf(elapsedTime * speed, axis);
  }
}
