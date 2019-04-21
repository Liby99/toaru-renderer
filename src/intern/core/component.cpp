#include "core/component.h"

using namespace toaru;

Component::Component() : initialized(false), enabled(true) {
  currEntity = nullptr;
  currContext = nullptr;
}

void Component::init() {}

void Component::update() {}

void Component::render() {}

bool Component::isEnabled() {
  return enabled;
}

void Component::enable() {
  enabled = true;
}

void Component::disable() {
  enabled = false;
}

void Component::setEnabled(bool enable) {
  enabled = enable;
}

void Component::setEnv(Context &context, Entity &entity) {
  currContext = &context;
  currEntity = &entity;
}

Entity &Component::object() {
  return *currEntity;
}

Context &Component::context() {
  return *currContext;
}