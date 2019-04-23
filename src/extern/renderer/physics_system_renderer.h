#ifndef TOARU_PHYSICS_SYSTEM_RENDERER_H
#define TOARU_PHYSICS_SYSTEM_RENDERER_H

#include "core/material.h"
#include "core/renderer.h"
#include "physics/physics_system.h"

namespace toaru {
  class PhysicsSystemRenderer : public Renderer {
  public:
    virtual void render();
  };
}

#endif