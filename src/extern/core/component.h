#ifndef TOARU_COMPONENT_H
#define TOARU_COMPONENT_H

#include "entity.h"

namespace toaru {
  class Component {
  public:
    Component();

    virtual void init();
    virtual void update();
    virtual void render();

    bool isEnabled();
    void enable();
    void disable();
    void setEnabled(bool enable);
    void setEnv(Context &context, Entity &entity);

  protected:
    Entity &object();
    Context &context();

  private:
    bool initialized, enabled;
    Entity *currEntity;
    Context *currContext;
  };
}

#endif