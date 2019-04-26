#include "physics/material_tensor.h"

using namespace toaru;

MaterialTensor::MaterialTensor(float e, float v, bool calculateLame) : e(e), v(v) {
  if (calculateLame) {
    // Calculate Lame constants
    lambda = e * v / ((1.0 + v) * (1.0 - 2.0 * v));
    mu = e / (2.0 * (1.0 + v));
  } else {
    lambda = e;
    mu = v;
  }

  // Construct Stiffness matrix
  mat = Matrix<float, 6, 6>::Zero();
  c = 2.0 * mu + lambda;
  // TODO: leave actual K matrix out for now

  // Upper K
  upper.fill(lambda);
  upper.diagonal() << c, c, c;

  // Lower K
  lower.setZero();
  lower.diagonal() << mu, mu, mu;
}
