#ifndef TOARU_TETRAHEDRON_H
#define TOARU_TETRAHEDRON_H

#include "face.h"
#include "physics/physics_material.h"
#include "physics_system.h"
#include "point.h"
#include "utility/math.h"

namespace toaru {
  class Face;

  class Tetrahedron{
  public:
    // Mass
    float mass;

    // Density
    float density;

    // Points
    std::vector<std::reference_wrapper<Point>> points;

    // Faces
    std::vector<std::reference_wrapper<Face>> faces;

    // Rest axis
    std::vector<Vector3f> axes;

    // Rest volume
    float volume;

    // Rest tetrahedral frame matrix
    Matrix3f invR;

    // Physics properties
    const PhysicsMaterial &K;
    const PhysicsMaterial &D;

    Tetrahedron(float density, const PhysicsMaterial &K, const PhysicsMaterial &D, Point & p0,
                Point &p1, Point &p2, Point &p3);

    const Point &getPoint(int i) const;
    Vector3f getCenter() const;
    bool isInside(const Vector3f &p) const;
    bool intersect(const Tetrahedron &other) const;

    void update(float deltaTime);

    void initRestState();

  private:
    virtual void distributeForceToPoint();

    Matrix3f calculateCurrentFrame();

    Matrix3f toStress(const Matrix3f &strain, const PhysicsMaterial &K) const;

    Matrix3f lastStrain;
  };
}
#endif
