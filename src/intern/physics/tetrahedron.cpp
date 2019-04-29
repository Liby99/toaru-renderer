#include "physics/tetrahedron.h"
#include "physics/convex_hull.h"
#include <iostream>

using namespace toaru;

Tetrahedron::Tetrahedron(const PhysicsMaterial &mat, Point &p0, Point &p1, Point &p2, Point &p3)
  : mat(mat) {
  this->points.insert(this->points.end(), {&p0, &p1, &p2, &p3});
  this->plasticStrain.setZero();
}

const Point &Tetrahedron::getPoint(int i) const {
  assert(i >= 0 && i < 5);
  return *points[i];
}

Vector3f Tetrahedron::getCenter() const {
  return (points[0]->position + points[1]->position + points[2]->position + points[3]->position) /
         4.0f;
}

bool Tetrahedron::contains(const Vector3f &p) const {
  // Implementation: The point should be behind all four faces
  for (const Face *face : faces) {
    if (!face->isAtBackSide(p)) {
      return false;
    }
  }
  return true;
}

void Tetrahedron::addFace(const Face &face) {
  faces.push_back(&face);
}

void Tetrahedron::handleCollision(Tetrahedron &other) {
  // TODO: Handle the collision between this tetrahedron and the other one.

  // Check Collision and get intersection volume.
  float intersectVolume = -1.0f;
  Vector3f intersectCenter;

  // if edges(6) of this intersect with faces(3) of other
  // List to save intersection points
  std::vector<Vector3f> intersects;

  checkIntersect(intersects, *this, other);
  checkIntersect(intersects, other, *this);

  for (auto &element : this->points) {
    if (other.contains(element->position)) {
      intersects.push_back(element->position);
    }
  }

  for (auto &element : other.points) {
    if (this->contains(element->position)) {
      intersects.push_back(element->position);
    }
  }

  if (intersects.size() > 3) {
    // std::cout << intersects.size() << std::endl;

    // Construct Convex Hull based on intersection points.
    ConvexHull ch(intersects);
    intersectVolume = ch.volume;
    intersectCenter = ch.center;
  }

  if (intersectVolume > 0) {
    // Have intersect volume, TODO: do proper collision response
    this->addForceAt((this->getCenter() - intersectCenter).normalized() * intersectVolume *
                     10000000,
                     intersectCenter);
    other.addForceAt(-(this->getCenter() - intersectCenter).normalized() * intersectVolume *
                     10000000,
                     intersectCenter);
  }
}

void Tetrahedron::checkIntersect(std::vector<Vector3f> &intersects, Tetrahedron &a,
                                 Tetrahedron &b) {
  // Construct edge
  auto e0 = a.points[1]->position - a.points[0]->position;
  auto e1 = a.points[2]->position - a.points[0]->position;
  auto e2 = a.points[3]->position - a.points[0]->position;
  auto e3 = a.points[2]->position - a.points[1]->position;
  auto e4 = a.points[3]->position - a.points[1]->position;
  auto e5 = a.points[3]->position - a.points[2]->position;

  // List to save rays;
  std::vector<Ray> rays;

  // Construct rays
  rays.emplace_back(a.points[0]->position, e0);
  rays.emplace_back(a.points[0]->position, e1);
  rays.emplace_back(a.points[0]->position, e2);
  rays.emplace_back(a.points[1]->position, e3);
  rays.emplace_back(a.points[1]->position, e4);
  rays.emplace_back(a.points[2]->position, e5);

  // Rays to faces test
  for (int i = 0; i < b.faces.size(); i++) {
    const auto &face = b.faces[i];
    Vector3f pos;
    Vector3f normal;
    for (auto r : rays) {
      if (face->intersect(r, pos, normal)) {
        // if intersect, save pos
        intersects.push_back(pos);
        // TODO: normal
      }
    }
  }
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
    plasticStrain *= std::min<float>(1.0, (mat.gamma2) / (plasticStrain.norm()));
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
    auto &point = face->getOppositePoint();
    point.addForce(force);
  });
}

void Tetrahedron::addForceAt(const Vector3f &f, const Vector3f &p) {
  // Barycentric coord of tetrahedron: T^{-1} (p - p3)
  // Construct T
  Matrix3f T;
  const auto &p0 = points[0]->position;
  const auto &p1 = points[1]->position;
  const auto &p2 = points[2]->position;
  const auto &p3 = points[3]->position;
  T << p0.x() - p3.x(), p1.x() - p3.x(), p2.x() - p3.x(), p0.y() - p3.y(), p1.y() - p3.y(),
    p2.y() - p3.y(), p0.z() - p3.z(), p1.z() - p3.z(), p2.z() - p3.z();

  Matrix3f invT = T.inverse();
  Vector3f coord = invT * (p - p3);
  float w = 1 - coord.x() - coord.y() - coord.z();
  if (coord.x() > 0 && coord.y() > 0 && coord.z() > 0 && w > 0) {
    points[0]->addForce(f * coord.x());
    points[1]->addForce(f * coord.y());
    points[2]->addForce(f * coord.z());
    points[3]->addForce(f * w);
  }
  //
}

float Tetrahedron::getVolume() const {
  // Build tetrahedral frame axes
  std::vector<Vector3f> axes(3);
  for (int i = 0; i < 3; i++) {
    axes[i] = this->points[i]->position - this->points[3]->position;
  }
  // Calculate volume
  constexpr float factor = 1.0 / 6.0;
  float volume = factor * (axes[0].cross(axes[1])).dot(axes[2]);
  return volume;
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
  std::for_each(points.begin(), points.end(), [&](Point *p) { p->addMass(mass * 0.25); });
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
