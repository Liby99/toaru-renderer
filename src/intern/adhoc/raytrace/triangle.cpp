#include <adhoc/raytrace/triangle.h>

using namespace nash;

Triangle::Triangle(const Vector3f a, const Vector3f b, const Vector3f c) {
  vertices.push_back(a);
  vertices.push_back(b);
  vertices.push_back(c);
  calculateNormal();
}

Triangle::Triangle(const std::initializer_list<Vector3f> vertices) {
  if (vertices.size() != 3) {
    throw std::runtime_error("Wrong number of vertices");
  }
  this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
  calculateNormal();
}

bool Triangle::intersect(const Ray &ray, Intersection &intersection) {
  // O + tD = A + u(B-A) + v(C-A)
  float t = 0, u = 0, v = 0;
  Vector3f edge_1 = vertices[1] - vertices[0];
  Vector3f edge_2 = vertices[2] - vertices[0];
  Vector3f P = ray.dir.cross(edge_2);
  float det = P.dot(edge_1);

  // ray dir parallel to triangle plane
  if (fabs(det) < 1e-10)
    return false;

  float invDet = 1 / det;
  Vector3f T = ray.pos - vertices[0];
  Vector3f Q = T.cross(edge_1);

  u = invDet * P.dot(T);
  if (u < 0 || u > 1)
    // intersection is outside of triangle
    return false;

  v = invDet * Q.dot(ray.dir);
  if (v < 0 || u + v > 1)
    // intersection is outside of triangle
    return false;

  // intersection is in the triangle
  t = invDet * Q.dot(edge_2);

  if (t < 0)
    // intersection is on the other side
    return false;

  // calculate intersection position and update intersection
  if (!intersection.hit || intersection.t > t) {
    intersection.hit = true;
    intersection.t = t;
    intersection.position = ray.pos + ray.dir * t;
    intersection.normal = (det > 0) ? normal : -1 * normal;
  }

  return true;
}

void Triangle::calculateNormal() {
  Vector3f e1 = vertices[1] - vertices[0]; // edge 1
  Vector3f e2 = vertices[2] - vertices[0]; // edge 2
  normal = e1.cross(e2);
  normal.normalize();
}
