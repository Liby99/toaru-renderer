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

    // Constant
    float e, v;

    // Lame constants
    float lambda, mu, c;

    // Stiffness tensor K
    Matrix<float, 6, 6> K;

    // Upper K
    Matrix<float, 3, 3> uK;

    // Lower K
    Matrix<float, 3, 3> lK;

    Tetrahedron(float mass, float e, float v, std::vector<std::shared_ptr<Point>> points);

    Tetrahedron(float mass, float e, float v, std::initializer_list<std::shared_ptr<Point>> points);

    void update(float deltaTime);

  private:
    virtual void initRestState();

    virtual std::shared_ptr<Face> getFace(std::initializer_list<std::shared_ptr<Point>> points,
                                          std::shared_ptr<Point> opposite);

    virtual void distributeForceToPoint();

    virtual Matrix3f calculateCurrentFrame();
  };
}
#endif
