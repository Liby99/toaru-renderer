#ifndef TOARU_PHYSICS_SYSTEM_RENDERER_H
#define TOARU_PHYSICS_SYSTEM_RENDERER_H

#include "core/material.h"
#include "core/renderer.h"
#include "physics/physics_system.h"

namespace toaru {
  class PhysicsSystemRenderer : public Renderer {
  public:
    bool renderAllFaces = false, cached = false;
    void refresh();
    virtual void render();

  protected:
    std::vector<const Face *> faces;
    MatrixXu indices;
    MatrixXf positions, normals;
    void cacheIndicesData(const PhysicsSystem &sys);
    void loadPosAndNorm(const PhysicsSystem &sys);
  };
}

#endif