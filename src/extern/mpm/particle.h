#ifndef MPM_PARTICLE_H
#define MPM_PARTICLE_H

#include "utility/math.h"

namespace toaru::mpm {
  class Particle {
  public:
    Vector3f position, velocity;
    float mass;
    Matrix3f F; // Deformation gradient
    Matrix3f C; // Affine momentum of APIC
    float Jp; // Determinant of the deformation gradient
    Matrix3f W; // Kernel Function
    float initialVolume;

    Particle();
    Particle(const Vector3f &pos);
  };
}

#endif