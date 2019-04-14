#include "viewer/canvas.h"
#include "viewer/context.h"

using namespace nash;

Canvas::Canvas(nanogui::Widget *parent, Scene &s)
    : nanogui::GLCanvas(parent), scene(&s), context(new Context()) {
  // Do nothing
}

Canvas::~Canvas() { delete context; }

void Canvas::setSize(int width, int height) {
  GLCanvas::setSize({width, height});
  scene->setSize(width, height);
}

void Canvas::bindGLFWWindow(GLFWwindow *window) { context->bindGLFWWindow(window); }

void Canvas::init() {
  context->init();
  scene->start(*context);
}

bool Canvas::keyboardEvent(int key, int scancode, int action, int modifiers) {
  return context->keyboardEvent(key, scancode, action, modifiers);
}

bool Canvas::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) {
  return context->mouseButtonEvent(p, button, down, modifiers);
}

bool Canvas::mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) {
  return context->mouseMotionEvent(p, rel, button, modifiers);
}

bool Canvas::mouseEnterEvent(const Vector2i &p, bool enter) {
  return context->mouseEnterEvent(p, enter);
}

bool Canvas::scrollEvent(const Vector2i &p, const Vector2f &rel) {
  return context->scrollEvent(p, rel);
}

void Canvas::drawGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  scene->update(*context);
  scene->render(*context);
  context->endOfFrameCycle();
  glDisable(GL_DEPTH_TEST);
}
