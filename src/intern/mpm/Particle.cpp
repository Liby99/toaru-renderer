#include "mpm/particle.h"

using namespace toaru::mpm;

Particle::Particle() : Particle(Vector3f::Zero()) {}

Particle::Particle(const Vector3f &pos) :
  position(pos),
  velocity(Vector3f::Zero()),
  mass(1.0f),
  F(Matrix3f::Identity()),
  C(Matrix3f::Zero()),
  Jp(1.0f),
  W(Matrix3f::Zero()),
  initialVolume(0.0f){}