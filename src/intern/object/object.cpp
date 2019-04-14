#include "object/object.h"
#include "utility/script.h"

using namespace nash;

Object::Object() : active(true), hidden(false), shader(nullptr), parent(nullptr) {
  // Do nothing
}

bool Object::hasParent() { return parent != nullptr; }

Object &Object::getParent() { return *parent; }

void Object::setParent(Object &p) {
  if (parent != nullptr) {
    removeFromParent();
  }
  parent = &p;
  parent->children.push_back(this);
}

void Object::removeFromParent() {
  if (parent != nullptr) {
    std::vector<Object *> &cs = parent->children;
    cs.erase(std::remove(cs.begin(), cs.end(), this), cs.end());
    parent = nullptr;
  }
}

std::vector<Object *> &Object::getChildren() { return children; }

Object &Object::getChild(int i) { return *(children[i]); }

bool Object::hasShader() { return shader != nullptr; }

Shader &Object::getShader() { return *shader; }

void Object::setShader(Shader &s) { shader = &s; }

void Object::activate() { setActive(true); }

void Object::deactivate() { setActive(false); }

void Object::setActive(bool a) { active = a; }

bool Object::isActive() { return active; }

void Object::show() { hidden = false; }

void Object::hide() { hidden = true; }

void Object::setHidden(bool h) { hidden = h; }

bool Object::isHidden() { return hidden; }

void Object::attachScript(Script<Object> &script) {
  script.onAttach(*this);
  scripts.push_back(&script);
}

Script<Object> &Object::getScript(const std::string &name) {
  auto it = find_if(scripts.begin(), scripts.end(),
                    [&name](const Script<Object> *s) { return s->name == name; });
  if (it != scripts.end()) {
    return **it;
  } else {
    throw std::invalid_argument("Not existed script " + name);
  }
}

void Object::removeScript(const std::string &name) {
  auto it = find_if(scripts.begin(), scripts.end(),
                    [&name](const Script<Object> *s) { return s->name == name; });
  if (it != scripts.end()) {
    scripts.erase(it);
  }
}

void Object::start() {
  // Do nothing
}

void Object::update() {
  // Do nothing
}

void Object::render() {
  // Do nothing
}

void Object::startWrapper(Context &context) {
  this->context = &context;
  if (active) {
    start();
    for (int i = 0; i < scripts.size(); i++) {
      scripts[i]->setContext(context);
      scripts[i]->bind(*this);
      scripts[i]->start();
    }
    for (int i = 0; i < children.size(); i++) {
      children[i]->startWrapper(context);
    }
  }
}

void Object::updateWrapper(Context &context, Matrix4f &world) {
  this->context = &context;
  if (active) {
    transform.world = world;
    Matrix4f currTransform = transform.getTransform();
    update();
    for (int i = 0; i < scripts.size(); i++) {
      scripts[i]->setContext(context);
      scripts[i]->bind(*this);
      scripts[i]->update();
    }
    for (int i = 0; i < children.size(); i++) {
      children[i]->updateWrapper(context, currTransform);
    }
  }
}

void Object::renderWrapper(Context &context, Matrix4f &viewPersp) {
  this->context = &context;
  if (active) {

    // Check if is going to do the rendering
    if (!hidden && hasShader()) {

      // First set basic properties of this object and the camera
      shader->bind();
      shader->setUniform("model", transform.getTransform());
      shader->setUniform("viewPersp", viewPersp);

      // Then run the pre render scripts
      for (int i = 0; i < scripts.size(); i++) {
        scripts[i]->setContext(context);
        scripts[i]->bind(*this);
        scripts[i]->preRender();
      }

      // Render the object
      render();

      // Finally run the post render scripts
      for (int i = 0; i < scripts.size(); i++) {
        scripts[i]->postRender();
      }
    }

    // Render all the children if not hidden
    if (!hidden) {
      for (int i = 0; i < children.size(); i++) {
        children[i]->renderWrapper(context, viewPersp);
      }
    }
  }
}