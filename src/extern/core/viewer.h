#ifndef TOARU_VIEWER_H
#define TOARU_VIEWER_H

#include <nanogui/nanogui.h>
#include "canvas.h"

namespace toaru {
  class Viewer : public nanogui::Screen {
  public:
    static const nanogui::Color BACKGROUND;
    static const int DEFAULT_WIDTH, DEFAULT_HEIGHT;
    static const std::string DEFAULT_WINDOW_TITLE;
    static const int HEADER_HEIGHT, PADDING;
    static const int MIN_REFRESH_INTERVAL;

    Viewer(Scene &scene);
    Viewer(int width, int height, const std::string &name, Scene &scene);

    void start();

    virtual void draw(NVGcontext *);

  protected:
    nanogui::Window *sceneWindow;
    Canvas *sceneCanvas;
  };
}

#endif