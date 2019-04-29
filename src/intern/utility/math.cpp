#include "utility/math.h"

using namespace toaru;

const double Math::PI = 3.14159265358979323846;

Vector2f Math::normalCartToPolar(const Vector3f &pos) {
  return Vector2f(acos(pos.z()), atan2(pos.y(), pos.x()));
}

Vector3f Math::polarToCart(const Vector2f &pol) {
  return polarToCart(pol.x(), pol.y());
}

Vector3f Math::polarToCart(float theta, float phi) {
  float sint = sin(theta), cost = cos(theta), sinp = sin(phi), cosp = cos(phi);
  return Vector3f(sint * cosp, sint * sinp, cost);
}

float Math::random(float low, float up) {
  return low + ((float)rand() / (float)RAND_MAX) * (up - low);
}

float Math::random(float up) {
  return ((float)rand() / (float)RAND_MAX) * up;
}

bool Math::isEqual(float f1, float f2) {
  return abs(f1 - f2) < std::numeric_limits<float>::epsilon();
}
