#ifndef TOARU_TETRAHEDRON_H
#define TOARU_TETRAHEDRON_H

#include "face.h"
#include "physics_material.h"
#include "physics_system.h"
#include "point.h"
#include "utility/math.h"

namespace toaru {
  class Face;

  class Tetrahedron{
  public:
    // Mass
    float mass;

    // Points
    std::vector<Point *> points;

    // Faces
    std::vector<Face *> faces;

    // Rest axis
    std::vector<Vector3f> axes;

    // Rest volume
    float volume;

    // Rest tetrahedral frame matrix
    Matrix3f invR;

    // Physics properties
    const PhysicsMaterial &mat;

    Tetrahedron(const PhysicsMaterial &mat, Point & p0, Point &p1, Point &p2, Point &p3);

    const Point &getPoint(int i) const;
    Vector3f getCenter() const;
    bool isInside(const Vector3f &p) const;
    bool intersect(const Tetrahedron &other) const;

    void update(float deltaTime);

    void initRestState();

  private:
    virtual void distributeForceToPoint();

    Matrix3f calculateCurrentFrame();

    Matrix3f toStress(const Matrix3f &strain, const MaterialTensor &t) const;

    Matrix3f lastStrain;
  };
}
#endif
