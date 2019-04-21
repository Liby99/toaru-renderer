#ifndef TOARU_SCENE_H
#define TOARU_SCENE_H

#include "context.h"

namespace toaru {
  class Scene {
  public:
    Scene();
    void update(Context &context);
    void render(Context &context);
  private:
  };
}

#endif