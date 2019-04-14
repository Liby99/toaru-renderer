#include <adhoc/raytrace/bounding_box.h>

using namespace nash;

BoundingBox::BoundingBox(const Vector3f min, const Vector3f max) {
  vertices_.push_back(min);
  vertices_.push_back(max);
}

BoundingBox::BoundingBox(const std::initializer_list<Vector3f> vertices) {
  this->vertices_.insert(this->vertices_.end(), vertices.begin(), vertices.end());
  calculateBoundingBox();
}

BoundingBox::BoundingBox(const std::vector<Vector3f> vertices) {
  // calculate bounding box given vertices
  this->vertices_.insert(this->vertices_.end(), vertices.begin(), vertices.end());
  calculateBoundingBox();
}

void BoundingBox::calculateBoundingBox() {
  if (vertices_.size() < 2) {
    throw std::runtime_error("Wrong number of vertices");
  } else if (vertices_.size() == 2) {
    // TODO
    return;
  } else {
    float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
    float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
    for (int i = 0; i < vertices_.size(); i++) {
      Vector3f curr = vertices_[i];
      minX = std::min(minX, curr[0]);
      maxX = std::max(maxX, curr[0]);
      minY = std::min(minY, curr[1]);
      maxY = std::max(maxY, curr[1]);
      minZ = std::min(minZ, curr[2]);
      maxZ = std::max(maxZ, curr[2]);
    }
    vertices_.clear();
    vertices_.push_back(Vector3f(minX, minY, minZ));
    vertices_.push_back(Vector3f(maxX, maxY, maxZ));
  }

  // calculate extents
  extents =
      Vector3f((vertices_[1][0] - vertices_[0][0]) / 2, (vertices_[1][1] - vertices_[0][1]) / 2,
               (vertices_[1][2] - vertices_[0][2]) / 2);

  // calculate center
  center = (vertices_[0] + vertices_[1]) / 2;
}

const std::vector<Vector3f> &BoundingBox::getVertices_() const { return vertices_; }

const Vector3f &BoundingBox::getExtents() const { return extents; }

const Vector3f &BoundingBox::getCenter() const { return center; }

bool BoundingBox::intersect(const Ray &ray, Intersection &intersection) {
  float t = 0;
  float lbx = (vertices_[0][0] - ray.pos[0]) * ray.invDir[0];
  float rtx = (vertices_[1][0] - ray.pos[0]) * ray.invDir[0];
  float lby = (vertices_[0][1] - ray.pos[1]) * ray.invDir[1];
  float rty = (vertices_[1][1] - ray.pos[1]) * ray.invDir[1];
  float lbz = (vertices_[0][2] - ray.pos[2]) * ray.invDir[2];
  float rtz = (vertices_[1][2] - ray.pos[2]) * ray.invDir[2];

  float tmin = std::max(std::max(std::min(lbx, rtx), std::min(lby, rty)), std::min(lbz, rtz));
  float tmax = std::min(std::min(std::max(lbx, rtx), std::max(lby, rty)), std::max(lbz, rtz));

  // AABB is behind the ray
  if (tmax < 0) {
    return false;
  }

  // doesn't intersect
  if (tmin > tmax) {
    return false;
  }

  // intersect, at O + tD
  if (tmin < 0) {
    t = tmax;
  } else {
    t = tmin;
  }

  return true;
}
