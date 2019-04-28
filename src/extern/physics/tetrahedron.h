#ifndef TOARU_TETRAHEDRON_H
#define TOARU_TETRAHEDRON_H

#include "face.h"
#include "physics_material.h"
#include "point.h"
#include "utility/math.h"

namespace toaru {
  class Tetrahedron {
  public:
    // Mass
    float mass;

    // Points
    std::vector<Point *> points;

    // Faces
    std::vector<const Face *> faces;

    // Rest axis
    std::vector<Vector3f> axes;

    // Rest volume
    float volume;

    // Rest tetrahedral frame matrix
    Matrix3f invR;

    // Plastic strain tensor
    Matrix3f plasticStrain;

    // Physics properties
    const PhysicsMaterial &mat;

    Tetrahedron(const PhysicsMaterial &mat, Point &p0, Point &p1, Point &p2, Point &p3);

    const Point &getPoint(int i) const;
    Vector3f getCenter() const;
    bool contains(const Vector3f &p) const;

    void addFace(const Face &face);
    void initRestState();

    void handleCollision(Tetrahedron &other);
    void update(float deltaTime);

  private:
    Matrix3f lastStrain;
    virtual void distributeForceToPoint();
    Matrix3f calculateCurrentFrame();
    Matrix3f toStress(const Matrix3f &strain, const MaterialTensor &t) const;
  };
}
#endif
