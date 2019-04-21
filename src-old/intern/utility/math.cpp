#include "utility/math.h"

using namespace nash;

Vector2f Math::normalCartToPolar(const Vector3f &pos) {
  return Vector2f(acos(pos.z()), atan2(pos.y(), pos.x()));
}

Vector3f Math::polarToCart(const Vector2f &pol) { return polarToCart(pol.x(), pol.y()); }

Vector3f Math::polarToCart(float theta, float phi) {
  float sint = sin(theta), cost = cos(theta), sinp = sin(phi), cosp = cos(phi);
  return Vector3f(sint * cosp, sint * sinp, cost);
}
