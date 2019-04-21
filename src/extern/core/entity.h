#ifndef TOARU_ENTITY_H
#define TOARU_ENTITY_H

#include "context.h"
#include "utility/transform.h"
#include <any>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace toaru {
  class Component;

  class Entity {
  public:
    Transform transform;

    Entity();

    void update(Context &context, Matrix4f &world);
    void render(Context &context);

    void addComponent(const std::string &str, Component &comp);
    void removeComponent(const std::string &str);
    bool hasComponent(const std::string &str);
    template<typename T> T &getComponent(const std::string &str);

    void addChild(Entity &entity);
    void removeChild(int index);
    Entity &getChild(int index);

  protected:
    bool initialized;
    std::vector<Entity *> children;
    std::map<std::string, Component *> components;
  };
}

#endif