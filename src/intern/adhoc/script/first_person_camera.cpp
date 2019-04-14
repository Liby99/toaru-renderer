#include "adhoc/script/first_person_camera.h"

using namespace nash;

FirstPersonCamera::FirstPersonCamera() : ThirdPersonCamera() { distance = 0.001; }

void FirstPersonCamera::update() {
  double dt = context->getDeltaTime();
  updateAngle(dt);
  updateCameraTarget(dt);
  updateCameraPosition();
}