#include "physics/face.h"

using namespace toaru;

std::tuple<bool, std::shared_ptr<Face>> Face::getFace(
  std::initializer_list<std::shared_ptr<Point>> points) {

  assert(points.size() == 3);

  // Temp face
  Face f(points);
  auto result = std::find_if(faces.begin(), faces.end(), [&](const std::shared_ptr<Face> &face)
  {
    return face->operator==(f);
  });

  // If found
  if (result != faces.end()) {
    return {true, (*result)};
  }

  // if not found, create one
  auto face = std::make_shared<Face>(points);
  face->updateNormal();
  faces.push_back(face);
  return {false, face};
}

bool Face::operator==(const Face &other) {
  return (std::find(this->points.begin(), this->points.end(), other.points[0]) !=
          this->points.end()) &&
         (std::find(this->points.begin(), this->points.end(), other.points[1]) !=
          this->points.end()) &&
         (std::find(this->points.begin(), this->points.end(), other.points[2]) !=
          this->points.end());
}

bool Face::isInternalFace() const {
  return (t2 != nullptr);
}

Vector3f Face::getNormal(const std::shared_ptr<Tetrahedron> &t) {
  updateNormal();
  if (t2 && t == t2) {
    return -normal;
  }
  return normal;
}

std::shared_ptr<Point> Face::getOppositePoint(const std::shared_ptr<Tetrahedron>& t) const {
  if (t2 && t == t2) {
    return p2;
  }
  return p1;
}

void Face::updateNormal() {
  // e1 = v1 - v0; e2 = v2 - v0; normal = e1 x e2;
  Vector3f e1 = this->points[1]->position - this->points[0]->position;
  Vector3f e2 = this->points[2]->position - this->points[0]->position;

  this->normal = e1.cross(e2);
  this->area = this->normal.norm() / 2.0;
  //this->normal.normalize();
}

Face::Face(std::initializer_list<std::shared_ptr<Point>> points) {
  this->points.insert(this->points.end(), points.begin(), points.end());
  
}
