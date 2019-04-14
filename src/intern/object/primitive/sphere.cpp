#include "object/primitive/sphere.h"

using namespace nash;

Sphere::Sphere() : Sphere(SUBDIVISION) {}

Sphere::Sphere(int subdivision) : Mesh(), subdivision(subdivision) { generateMesh(); }

void Sphere::generateMesh() {

  // First generate inital 6 points
  float s = sqrt(2) / 2;
  Vertices vertices;
  vertices.push_back(Vector3f(0, -1, 0));
  vertices.push_back(Vector3f(s, 0, s));
  vertices.push_back(Vector3f(-s, 0, s));
  vertices.push_back(Vector3f(s, 0, -s));
  vertices.push_back(Vector3f(-s, 0, -s));
  vertices.push_back(Vector3f(0, 1, 0));

  // Then generate initial triangles
  Triangles triangles;
  triangles.push_back(Vector3u(0, 2, 1));
  triangles.push_back(Vector3u(0, 1, 3));
  triangles.push_back(Vector3u(1, 2, 5));
  triangles.push_back(Vector3u(2, 0, 4));
  triangles.push_back(Vector3u(3, 4, 0));
  triangles.push_back(Vector3u(4, 5, 2));
  triangles.push_back(Vector3u(5, 3, 1));
  triangles.push_back(Vector3u(5, 4, 3));

  // Subdivide the triangles starting from 0
  subdivide(vertices, triangles, 0);

  // Finally generate from this triangles
  indices = MatrixXu(3, triangles.size());
  for (int i = 0; i < triangles.size(); i++) {
    indices.col(i) << triangles[i].x(), triangles[i].y(), triangles[i].z();
  }
  positions = MatrixXf(3, vertices.size());
  normals = MatrixXf(3, vertices.size());
  for (int i = 0; i < vertices.size(); i++) {
    positions.col(i) << vertices[i].x(), vertices[i].y(), vertices[i].z();
    normals.col(i) << vertices[i].x(), vertices[i].y(), vertices[i].z();
  }
}

void Sphere::subdivide(Vertices &vertices, Triangles &triangles, int step) {

  // Stop if done subdivision
  if (step >= subdivision)
    return;

  // Loop over existing triangles and subdivide a tri to four tris.
  int numTriangles = triangles.size(); // Cache the amount since it will be changing
  for (int i = 0; i < numTriangles; i++) {

    // Fetch information from triangles
    Vector3u &tri = triangles[i];
    int i1 = tri.x(), i2 = tri.y(), i3 = tri.z();
    Vector3f &v1 = vertices[i1], &v2 = vertices[i2], &v3 = vertices[i3];

    // Subdivide using mid points on each edge
    Vector3f m1 = 0.5 * (v1 + v2), m2 = 0.5 * (v2 + v3), m3 = 0.5 * (v3 + v1);
    // Normalize midpoints so that the vertices still lie on unit sphere
    vertices.push_back(m1.normalized());
    vertices.push_back(m2.normalized());
    vertices.push_back(m3.normalized());

    // Existing triangle will now be the center one
    uint32_t curr = vertices.size(), ni1 = curr - 3, ni2 = curr - 2, ni3 = curr - 1;
    triangles.push_back(Vector3u(i1, ni1, ni3));
    triangles.push_back(Vector3u(ni1, i2, ni2));
    triangles.push_back(Vector3u(ni3, ni2, i3));
    triangles[i] << ni1, ni2, ni3;
  }

  // Recursive subdivide. Tail recursion
  subdivide(vertices, triangles, step + 1);
}
