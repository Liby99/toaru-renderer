#include "core/entity.h"
#include "core/component.h"

using namespace toaru;

Entity::Entity() : initialized(false) {
  // Do nothing
}

void Entity::init(Context &context) {
  // Change the initialized state
  initialized = true;

  // First initialize components
  for (auto it = components.begin(); it != components.end(); it++) {
    auto comp = it->second;
    if (comp->isEnabled()) {
      comp->setEnv(context, *this);
      comp->init();
    }
  }

  // Then initialize children
  for (int i = 0; i < children.size(); i++) {
    children[i]->init(context);
  }
}

void Entity::update(Context &context, Matrix4f &world) {
  // Initialize if not initialized
  if (!initialized) init(context);

  // First set the world matrix of our own transform
  transform.world = world;
  Matrix4f currTransf = transform.getTransform();

  // Then go through all the components
  for (auto it = components.begin(); it != components.end(); it++) {
    auto comp = it->second;
    if (comp->isEnabled()) {
      comp->setEnv(context, *this);
      comp->update();
    }
  }

  // Finally recursively update all the children
  for (int i = 0; i < children.size(); i++) {
    children[i]->update(context, currTransf);
  }
}

void Entity::render(Context &context) {
  // Go through all the render in components
  for (auto it = components.begin(); it != components.end(); it++) {
    auto comp = it->second;
    if (comp->isEnabled()) {
      comp->setEnv(context, *this);
      comp->render();
    }
  }

  // Recursively render all the children
  for (int i = 0; i < children.size(); i++) {
    children[i]->render(context);
  }
}

void Entity::addComponent(const std::string &key, Component &comp) {
  components[key] = &comp;
}

void Entity::removeComponent(const std::string &key) {
  components.erase(key);
}

bool Entity::hasComponent(const std::string &key) {
  return components.find(key) == components.end();
}

Component &Entity::getComponent(const std::string &key) {
  return *components[key];
}

void Entity::addChild(Entity &entity) {
  children.push_back(&entity);
}

void Entity::removeChild(int index) {
  children.erase(children.begin() + index);
}

Entity &Entity::getChild(int index) {
  return *children[index];
}