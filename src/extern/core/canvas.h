#ifndef TOARU_CANVAS_H
#define TOARU_CANVAS_H

#include "scene.h"
#include <nanogui/nanogui.h>

namespace toaru {
  class Canvas : public nanogui::GLCanvas {
  public:
    Canvas(nanogui::Widget *parent, Scene &scene);

    void setSize(int width, int height);
    void bindGLFWWindow(GLFWwindow *window);

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    virtual bool mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button,
                                  int modifiers);
    virtual bool mouseEnterEvent(const Vector2i &p, bool enter);
    virtual bool scrollEvent(const Vector2i &p, const Vector2f &rel);

    virtual void drawGL();

  private:
    Scene &scene;
    Context context;
  };
}

#endif