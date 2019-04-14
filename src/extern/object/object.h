#ifndef OBJECT_H
#define OBJECT_H

#include "utility/script.h"
#include "utility/shader.h"
#include "utility/transform.h"
#include "viewer/context.h"
#include "viewer/scene.h"
#include <stdexcept>
#include <vector>

namespace nash {
  class Object {
  public:
    nash::Transform transform;

    Object();

    // Scene tree
    bool hasParent();
    Object &getParent();
    void setParent(Object &p);
    void removeFromParent();
    std::vector<Object *> &getChildren();
    Object &getChild(int i);

    // Shader spec
    bool hasShader();
    Shader &getShader();
    virtual void setShader(Shader &shader);

    // Activation
    void activate();
    void deactivate();
    void setActive(bool active);
    bool isActive();
    void show();
    void hide();
    void setHidden(bool h);
    bool isHidden();

    // Scripting
    void attachScript(Script<Object> &script);
    Script<Object> &getScript(const std::string &name);
    void removeScript(const std::string &name);

    // Object Life Cycle pipeline.
    virtual void start();
    virtual void update();
    virtual void render();

    // Base wrapper which could not be changed.
    void startWrapper(Context &context);
    void updateWrapper(Context &context, Matrix4f &world);
    void renderWrapper(Context &context, Matrix4f &viewPersp);

  protected:
    bool active, hidden;
    Context *context;
    Shader *shader;
    Object *parent;
    std::vector<Object *> children;
    std::vector<Script<Object> *> scripts;
  };
}

#endif
