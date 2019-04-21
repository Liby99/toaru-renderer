#ifndef TOARU_TETRAHEDRON_H
#define TOARU_TETRAHEDRON_H

#include "utility/math.h"
#include "point.h"
#include "face.h"

namespace toaru {
  class Face;

  class Tetrahedron : public std::enable_shared_from_this<Tetrahedron> {
  public:

    // Mass
    float mass;

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

    Tetrahedron(float mass, std::vector<std::shared_ptr<Point>> points);

    Tetrahedron(float mass, std::initializer_list<std::shared_ptr<Point>> points);

    void update(float deltaTime);

  private:
    void initRestState();

    std::shared_ptr<Face> getFace(std::initializer_list<std::shared_ptr<Point>> points);

    void distributeForceToPoint();

    Matrix3f calculateCurrentFrame();
  };
}
#endif
