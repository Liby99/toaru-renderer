#include "physics/field/gravity.h"

using namespace nash;

const Vector3f Gravity::DEFAULT_DIRECTION = Vector3f(0, -1, 0);

const float Gravity::G = 9.8f;

Gravity::Gravity() : Gravity(DEFAULT_DIRECTION, G) {}

Gravity::Gravity(float g) : Gravity(DEFAULT_DIRECTION, g) {}

Gravity::Gravity(Vector3f direction, float g) : direction(direction), g(g), Field() {}
