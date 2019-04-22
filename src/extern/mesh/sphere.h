#ifndef TOARU_SPHERE_H
#define TOARU_SPHERE_H

#include "core/mesh.h"

namespace toaru {
  class Sphere : public Mesh {
  public:
    const static int SUBDIVISION;
    Sphere();
    Sphere(int subdivision);
    virtual void init();
  protected:
    int subdivision;
    typedef std::vector<Vector3f> Vertices;
    typedef std::vector<Vector3u> Triangles;
    void generateMesh();
    void subdivide(Vertices &vertices, Triangles &triangles, int step);
  };
}

#endif