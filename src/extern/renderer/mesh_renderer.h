#ifndef TOARU_MESH_RENDERER_H
#define TOARU_MESH_RENDERER_H

#include "core/renderer.h"
#include "core/mesh.h"
#include "core/material.h"

namespace toaru {
  class MeshRenderer : public Renderer {
  public:
    const static std::string DEFAULT_MESH_NAME;
    const static std::string DEFAULT_MATERIAL_NAME;

    MeshRenderer();
    MeshRenderer(const std::string &meshName, const std::string &matName);

    virtual void render();

  protected:
    const std::string &meshName, &matName;
  };
}

#endif