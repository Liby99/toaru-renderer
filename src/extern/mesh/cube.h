#ifndef TOARU_CUBE_H
#define TOARU_CUBE_H

#include "core/mesh.h"

namespace toaru {
  class Cube : public Mesh {
  public:
    Cube();

    virtual void init();
  };
}

#endif