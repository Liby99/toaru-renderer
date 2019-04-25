#include "physics/tetrahedron.h"
#include <iostream>

using namespace toaru;

Tetrahedron::Tetrahedron(float density, const PhysicsMaterial &K, const PhysicsMaterial &D,
                         Point &p0, Point &p1, Point &p2, Point &p3)
  : K(K), D(D) {
  this->points.insert(this->points.end(), {p0, p1, p2, p3});
  this->density = density;
}

void Tetrahedron::update(float deltaTime) {
  // Step 1: Measure the deformation (strain)

  // Deformation Gradient: F = TR^{-1}
  Matrix3f T = calculateCurrentFrame();
  Matrix3f F = T * invR;

  // Green's Strain Tensor
  Matrix3f strain = 0.5 * (F.transpose() * F - Matrix3f::Identity());
  // Calculate Delta Strain tensor
  Matrix3f deltaStrain = Matrix3f::Zero();
  if (lastStrain != Matrix3f::Zero()) {
    deltaStrain = strain - lastStrain;
  }
  lastStrain = strain;

  // Calculate Strain rate tensor
  deltaStrain /= deltaTime;

  // Step 2: Relate strain to the internal forces (stress)
  Matrix3f stress = toStress(strain, this->K) + toStress(deltaStrain, this->D);

  // Step 3: Turn the internal stress into forces on the particles
  std::for_each(faces.begin(), faces.end(), [this, &F, &stress](Face &face)
  {
    Vector3f normal = face.getNormal();
    // Vector3f force = 0.5 * F * (normal.transpose() * stress).transpose();
    Vector3f force = 0.5 * F * stress * normal;
    auto & point = face.getOppositePoint();
    point.addForce(force);
  });
}

void Tetrahedron::initRestState() {
  // Build tetrahedral frame axes
  this->axes.resize(3);
  for (int i = 0; i < 3; i++) {
    this->axes[i] = this->points[i].get().position - this->points[3].get().position;
  }

  // Calculate volume
  constexpr float factor = 1.0 / 6.0;
  this->volume = factor * (this->axes[0].cross(this->axes[1])).dot(this->axes[2]);

  // Calculate mass based on volume and density
  this->mass = volume * density;

  // Build rest matrix
  bool res;
  Matrix3f R;
  for (int i = 0; i < 3; i++) {
    R.col(i) = axes[i];
  }
  R.computeInverseWithCheck(invR, res);
  assert(res == true);

  distributeForceToPoint();

  // initialize last strain
  lastStrain = Matrix3f::Zero();
}

void Tetrahedron::distributeForceToPoint() {
  std::for_each(points.begin(), points.end(),
                [&](const std::reference_wrapper<Point> &p) { p.get().addMass(mass * 0.25); });
}

Matrix3f Tetrahedron::calculateCurrentFrame() {
  // Build tetrahedral frame axes
  std::vector<Vector3f> axes(3);
  for (int i = 0; i < 3; i++) {
    axes[i] = this->points[i].get().position - this->points[3].get().position;
  }

  // Build matrix
  Matrix3f T;
  for (int i = 0; i < 3; i++) {
    T.col(i) = axes[i];
  }
  return T;
}


Matrix3f Tetrahedron::toStress(const Matrix3f &strain, const PhysicsMaterial &K) const {
  // Reshape strain tensor
  Vector3f offDiagonal;
  offDiagonal << strain(1, 2), strain(0, 2), strain(0, 1);
  offDiagonal *= 2.0;

  // Calculate stress using Lame constant and strain
  Matrix<float, 6, 1> s;
  s << K.upper * strain.diagonal(), K.lower * offDiagonal;

  // Reshape stress tensor
  Matrix3f stress;
  stress.diagonal() << s.block(0, 0, 3, 1);
  stress(1, 2) = stress(2, 1) = s(3, 0);
  stress(0, 2) = stress(2, 0) = s(4, 0);
  stress(0, 1) = stress(1, 0) = s(5, 0);

  return stress;
}
