#ifndef TOARU_SCENE_H
#define TOARU_SCENE_H

#include "context.h"
#include "entity.h"

namespace toaru {
  class Scene {
  public:
    Entity root;
    Scene();
    void update(Context &context);
    void render(Context &context);
  };
}

#endif