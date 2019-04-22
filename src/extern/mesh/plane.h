#ifndef TOARU_PLANE_H
#define TOARU_PLANE_H

#include "core/mesh.h"

namespace toaru {
  class Plane : public Mesh {
  public:
    Plane();
    virtual void init();
  };
}

#endif