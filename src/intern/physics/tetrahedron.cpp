#include "physics/tetrahedron.h"
#include <iostream>

using namespace toaru;

Tetrahedron::Tetrahedron(float mass, float e, float v, std::vector<std::shared_ptr<Point>> points) {
  assert(points.size() == 4);
  this->points.insert(this->points.end(), points.begin(), points.end());
  this->mass = mass;
  this->material = std::make_shared<PhysicsMaterial>(e, v);
}

Tetrahedron::Tetrahedron(float mass, float e, float v, std::shared_ptr<Point> p0,
                         std::shared_ptr<Point> p1, std::shared_ptr<Point> p2,
                         std::shared_ptr<Point> p3) {
  this->points.insert(this->points.end(), {p0, p1, p2, p3});
  this->mass = mass;
  this->material = std::make_shared<PhysicsMaterial>(e, v);
}

void Tetrahedron::update(float deltaTime) {
  // Step 1: Measure the deformation (strain)

  // Deformation Gradient: F = TR^{-1} 
  Matrix3f T = calculateCurrentFrame();
  Matrix3f F_m = T * invR;

  // Corotational frame
  Eigen::JacobiSVD<Matrix3f> D(F_m, ComputeFullU| ComputeFullV);

  Matrix3f Q = D.matrixV() * D.matrixU().transpose();
  Matrix3f S = Matrix3f::Zero();
  S.diagonal() << D.singularValues();
  Matrix3f F = D.matrixU() * S * D.matrixU().transpose();

  // Green's Strain Tensor
  Matrix3f strain = 0.5 * (F.transpose() * F - Matrix3f::Identity());

  // Step 2: Relate strain to the internal forces (stress)
  // Reshape strain tensor
  Vector3f offDiagonal;
  offDiagonal << strain(1, 2), strain(0, 2), strain(0, 1);
  offDiagonal *= 2.0;

  // Calculate stress using Lame constant and strain
  Matrix<float, 6, 1> s;
  s << material->uK * strain.diagonal(), material->lK * offDiagonal;

  // Reshape stress tensor
  Matrix3f stress;
  stress.diagonal() << s.block(0, 0, 3, 1);
  stress(1, 2) = stress(2, 1) = s(3, 0);
  stress(0, 2) = stress(2, 0) = s(4, 0);
  stress(0, 1) = stress(1, 0) = s(5, 0);

  // Step 3: Turn the internal stress into forces on the particles
  std::for_each(faces.begin(), faces.end(), [this, &F, &stress](const std::shared_ptr<Face> &face)
  {
    Vector3f normal = face->getNormal(shared_from_this());
    //Vector3f force = 0.5 * F * (normal.transpose() * stress).transpose();
    Vector3f force = 0.5 * F * stress * normal;
    auto point = face->getOppositePoint(shared_from_this());
    point->addForce(force);
  });

}

void Tetrahedron::initRestState() {
  // Build or get four faces
  // 1, 2, 3
  auto f1 = getFace({points[0], points[1], points[2]}, points[3]);
  // 4, 1, 3
  auto f2 = getFace({points[3], points[0], points[2]}, points[1]);
  // 2, 4, 3
  auto f3 = getFace({points[1], points[3], points[2]}, points[0]);
  // 4, 2, 1
  auto f4 = getFace({points[3], points[1], points[0]}, points[2]);

  faces.insert(faces.end(), {f1, f2, f3, f4});

  // Build tetrahedral frame axes
  this->axes.resize(3);
  for (int i = 0; i < 3; i++) {
    this->axes[i] = this->points[i]->position - this->points[3]->position;
  }

  // Calculate volume
  constexpr float factor = 1.0 / 6.0;
  this->volume = factor * (this->axes[0].cross(this->axes[1])).dot(this->axes[2]);

  // Build rest matrix
  bool res;
  Matrix3f R;
  for (int i = 0; i < 3; i++) {
    R.col(i) = axes[i];
  }
  R.computeInverseWithCheck(invR, res);
  assert(res == true);

  distributeForceToPoint();
}

std::shared_ptr<Face> Tetrahedron::getFace(std::initializer_list<std::shared_ptr<Point>> points,
                                           std::shared_ptr<Point> opposite) {
  auto [res, f] = Face::getFace(points);
  if (!res) {
    f->t1 = shared_from_this();
    f->p1 = opposite;
  } else {
    f->t2 = shared_from_this();
    f->p2 = opposite;
  }
  return f;
}

void Tetrahedron::distributeForceToPoint() {
  std::for_each(points.begin(), points.end(),
                [&](const std::shared_ptr<Point> &p) { p->addMass(mass * 0.25); });
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
