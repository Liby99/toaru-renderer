#include "physics/tetrahedron.h"
#include <iostream>

using namespace toaru;

Tetrahedron::Tetrahedron(const PhysicsMaterial &mat, Point & p0, Point &p1, Point &p2, Point &p3)
  : mat(mat) {
  this->points.insert(this->points.end(), {&p0, &p1, &p2, &p3});
  this->plasticStrain.setZero();
}

const Point &Tetrahedron::getPoint(int i) const {
  assert(i >= 0 && i < 5);
  return *points[i];
}

Vector3f Tetrahedron::getCenter() const {
  return (points[0]->position + points[1]->position + points[2]->position + points[3]->position) / 4.0f;
}

bool Tetrahedron::intersect(const Tetrahedron &other) const {
  Vector3f &p0 = points[0]->position, &p1 = points[1]->position,
           &p2 = points[2]->position, &p3 = points[3]->position;
  Vector3f e1 = p1 - p0, e2 = p2 - p0, e3 = p3 - p0;
  float e1l = e1.norm(), e2l = e2.norm(), e3l = e3.norm();
  Vector3f e1n = e1 / e1l, e2n = e2 / e2l, e3n = e3 / e3l;
  for (auto p : points) {
    Vector3f u = p->position - p0;
    float d1 = u.dot(e1n), d2 = u.dot(e2n), d3 = u.dot(e3n);
    if (d1 < e1l && d2 < e2l && d3 < e3l) {
      return true;
    }
  }
  return false;
}

void Tetrahedron::addFace(const Face &face) {
  faces.push_back(&face);
}

void Tetrahedron::update(float deltaTime) {
  // Step 1: Measure the deformation (strain)

  // Deformation Gradient: F = TR^{-1}
  Matrix3f T = calculateCurrentFrame();
  Matrix3f F = T * invR;

  // Green's Strain Tensor
  Matrix3f elasticStrain = 0.5 * (F.transpose() * F - Matrix3f::Identity());

  // Update plastic strain
  // Step 1 : Calculate amount of deformation
  Matrix3f deform = elasticStrain - ((elasticStrain.trace()) / 3.0) * Matrix3f::Identity();
  float magnitude = deform.norm();

  // If magnitude is larger than gamma1
  if (magnitude > mat.gamma1) {
    // Calculate deltaPlasticStrain
    Matrix3f deltaPlasticStrain = ((magnitude - mat.gamma1) / magnitude) * deform;

    // Update plasticStrain
    plasticStrain -= deltaPlasticStrain;

    // Clip plasticStrain
    plasticStrain *= std::min<float>(1.0, (mat.gamma2)/(plasticStrain.norm()));
  }

  // Final strain tensor = elastic strain + plastic strain
  Matrix3f strain = elasticStrain + plasticStrain;

  // Calculate Delta Strain tensor
  Matrix3f deltaStrain = Matrix3f::Zero();
  if (lastStrain != Matrix3f::Zero()) {
    deltaStrain = strain - lastStrain;
  }
  lastStrain = strain;

  // Calculate Strain rate tensor
  deltaStrain /= deltaTime;

  // Step 2: Relate strain to the internal forces (stress)
  Matrix3f stress = toStress(strain, mat.k) + toStress(deltaStrain, mat.d);

  // Step 3: Turn the internal stress into forces on the particles
  std::for_each(faces.begin(), faces.end(), [this, &F, &stress](const Face *face)
  {
    Vector3f normal = face->restNormal;
    // Vector3f force = 0.5 * F * (normal.transpose() * stress).transpose();
    Vector3f force = 0.5 * F * stress * normal;
    auto & point = face->getOppositePoint();
    point.addForce(force);
  });
}

void Tetrahedron::initRestState() {
  // Build tetrahedral frame axes
  this->axes.resize(3);
  for (int i = 0; i < 3; i++) {
    this->axes[i] = this->points[i]->position - this->points[3]->position;
  }

  // Calculate volume
  constexpr float factor = 1.0 / 6.0;
  this->volume = factor * (this->axes[0].cross(this->axes[1])).dot(this->axes[2]);

  // Calculate mass based on volume and density
  this->mass = volume * mat.density;

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
                [&](Point *p) { p->addMass(mass * 0.25); });
}

Matrix3f Tetrahedron::calculateCurrentFrame() {
  // Build tetrahedral frame axes
  std::vector<Vector3f> axes(3);
  for (int i = 0; i < 3; i++) {
    axes[i] = this->points[i]->position - this->points[3]->position;
  }

  // Build matrix
  Matrix3f T;
  for (int i = 0; i < 3; i++) {
    T.col(i) = axes[i];
  }
  return T;
}


Matrix3f Tetrahedron::toStress(const Matrix3f &strain, const MaterialTensor &t) const {

  // Calculate stress using Lame constant and strain
  auto stress = 2 * t.mu * strain + t.lambda * strain.trace() * Matrix3f::Identity();

  return stress;
}
