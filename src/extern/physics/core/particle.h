#ifndef PARTICLE_H
#define PARTICLE_H

#include "utility/math.h"

namespace nash {

  /**
   * Dimension of particle:
   *  0d: Point
   *  1d: Segment
   *  2d: Triangle
   *  3d: Tetrahedron
   */
  template<int dimension>
  class Particle {
  public:
    Matrix<float, 3, dimension + 1> position;
    Matrix<float, 3, dimension + 1> velocity;
    Matrix<float, 3, dimension + 1> acceleration;
    Matrix<float, 3, dimension + 1> forces;

    void addForce(Vector3f f);
  };
}

#endif