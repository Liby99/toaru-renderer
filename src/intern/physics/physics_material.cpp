#include "physics/physics_system.h"

using namespace toaru;

PhysicsMaterial::PhysicsMaterial(float e, float v) {
  this->e = e;
  this->v = v;

  // Calculate Lame constants
  lambda = e * v / ((1.0 + v) * (1.0 - 2.0 * v));
  mu = e / (2.0 * (1.0 + v));

  // Construct Stiffness matrix
  K = Matrix<float, 6, 6>::Zero();
  c = 2.0 * mu + lambda;
  // TODO: leave actual K matrix out for now

  // Upper K
  uK.fill(lambda);
  uK.diagonal() << c, c, c;

  // Lower K
  lK.setZero();
  lK.diagonal() << mu, mu, mu;
}
