#ifndef SAMPLER_H
#define SAMPLER_H

#include "utility/math.h"
#include <vector>

namespace nash {
  class Sampler {
  public:
    static void sampleSphere(std::vector<Vector3f> &samples);

    static Vector3f sampleSphere();

    static void sampleHemiSphere(std::vector<Vector3f> &samples, Vector3f normal);

    static Vector3f sampleHemiSphere(Vector3f normal);
  };
}

#endif
