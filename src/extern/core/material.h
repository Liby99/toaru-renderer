#ifndef TOARU_MATERIAL_H
#define TOARU_MATERIAL_H

#include "core/shader.h"
#include "core/component.h"

namespace toaru {
  class Material : public Component {
  public:
    Material();
    Material(Shader &shader);

    virtual void init();
    virtual void prerender();

    Shader &getShader();

  protected:
    Shader *shader;
  };
}

#endif