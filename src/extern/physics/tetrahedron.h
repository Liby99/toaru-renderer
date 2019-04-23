#ifndef TOARU_TETRAHEDRON_H
#define TOARU_TETRAHEDRON_H

#include "utility/math.h"
#include "point.h"
#include "face.h"
#include "physics/physics_material.h"
#include "physics_system.h"

namespace toaru {
  class Face;
  class Tetrahedron : public std::enable_shared_from_this<Tetrahedron> {
  public:

    // Mass
    float mass;

    // Density
    float density;

    // Points
    std::vector<std::shared_ptr<Point>> points;

    // Faces
    std::vector<std::shared_ptr<Face>> faces;

    // Rest axis
    std::vector<Vector3f> axes;

    // Rest volume
    float volume;

    // Rest tetrahedral frame matrix
    Matrix3f invR;

    // Physics properties
    std::shared_ptr<PhysicsMaterial> material;

    Tetrahedron(float density, float e, float v, std::vector<std::shared_ptr<Point>> points);

    Tetrahedron(float density, float e, float v, std::shared_ptr<Point> p0,
                std::shared_ptr<Point> p1,
                std::shared_ptr<Point> p2, std::shared_ptr<Point> p3);

    virtual void update(float deltaTime);

    virtual void initRestState();

  private:

    virtual void distributeForceToPoint();

    Matrix3f calculateCurrentFrame();

    std::shared_ptr<Face> makeFace(std::initializer_list<std::shared_ptr<Point>> points,
                                   std::shared_ptr<Point> opposite);
  };
}
#endif
