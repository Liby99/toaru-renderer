#include "physics/tetrahedron.h"

using namespace toaru;

Tetrahedron::Tetrahedron(float mass, std::vector<std::shared_ptr<Point>> points) {
  assert(points.size() == 4);
  this->points.insert(this->points.end(), points.begin(), points.end());
  this->mass = mass;
  initRestState();

}

Tetrahedron::Tetrahedron(float mass, std::initializer_list<std::shared_ptr<Point>> points) {
  assert(points.size() == 4);
  this->points.insert(this->points.end(), points.begin(), points.end());
  this->mass = mass;
  initRestState();
}

void Tetrahedron::update(float deltaTime) {
  // Step 1: Measure the deformation (strain)

  // Deformation Gradient: F = TR^{-1} 
  Matrix3f T = calculateCurrentFrame();
  Matrix3f F = T * invR;

  // Green's Strain Tensor
  Matrix3f epsilon = 0.5 * (F.transpose() * F - Matrix3f::Identity());

  // Step 2: Relate strain to the internal forces (stress)
  // Reshape strain tensor
  Vector3f offDiagonal;
  offDiagonal << epsilon(1, 2), epsilon(0, 2), epsilon(0, 1);
  offDiagonal *= 2.0;

  Matrix<float, 6, 1> strain;
  strain << epsilon.diagonal(), offDiagonal;

  // Step 3: Turn the internal stress into forces on the particles

  /*
  R << 1, 2, 3, 
       4, 5, 6, 
       7, 8, 9;
  std::cout << R*/
}

void Tetrahedron::initRestState() {
  // Build or get four faces
  // 1, 2, 3
  auto f1 = getFace({points[0], points[1], points[2]});
  // 4, 1, 3
  auto f2 = getFace({points[3], points[0], points[2]});
  // 2, 4, 3
  auto f3 = getFace({points[1], points[3], points[2]});
  // 4, 2, 1
  auto f4 = getFace({points[3], points[1], points[0]});

  faces.insert(faces.end(), {f1, f2, f3, f4});

  // Build tetrahedral frame axes
  for (int i = 0; i < 3; i++) {
    this->axes[i] = this->points[i]->position - this->points[4]->position;
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

std::shared_ptr<Face> Tetrahedron::getFace(std::initializer_list<std::shared_ptr<Point>> points) {
  auto [res, f] = Face::getFace(points);
  if (res) {
    f->t1 = shared_from_this();
  } else {
    f->t2 = shared_from_this();
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
    axes[i] = this->points[i]->position - this->points[4]->position;
  }

  // Build matrix
  Matrix3f T;
  for (int i = 0; i < 3; i++) {
    T.col(i) = axes[i];
  }
  return T;
}
