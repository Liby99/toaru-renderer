#ifndef TOARU_POINT_CLOUD_RENDERER_H
#define TOARU_POINT_CLOUD_RENDERER_H

#include "core/renderer.h"
#include "core/shader.h"
#include "mesh/point_cloud.h"

namespace toaru {
  class PointCloudRenderer : public Renderer {
  public:
    const static float DEFAULT_POINT_SIZE;

    std::string pointCloudName;
    Shader &shader;
    float pointSize;

    PointCloudRenderer(const std::string &name);
    PointCloudRenderer(const std::string &name, float pointSize);

    virtual void init();
    virtual void render();
  };
}

#endif