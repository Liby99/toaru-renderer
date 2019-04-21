#include "core/viewer.h"

using namespace toaru;

const nanogui::Color Viewer::BACKGROUND = {30, 30, 30, 255};

const int Viewer::DEFAULT_WIDTH = 1280;

const int Viewer::DEFAULT_HEIGHT = 720;

const std::string Viewer::DEFAULT_WINDOW_TITLE = "Toaru";

const int Viewer::HEADER_HEIGHT = 28;

const int Viewer::PADDING = 5;

const int Viewer::MIN_REFRESH_INTERVAL = 10;

Viewer::Viewer(Scene &scene) : Viewer(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_WINDOW_TITLE, scene) {}

Viewer::Viewer(int w, int h, const std::string &name, Scene &scene)
    : nanogui::Screen(Vector2i(w, h), name, false) {
  using namespace nanogui;
  setBackground(BACKGROUND);
  sceneWindow = new Window(this, name);
  sceneWindow->setLayout(new GroupLayout(PADDING));
  sceneCanvas = new Canvas(sceneWindow, scene);
  sceneCanvas->setBackgroundColor(BACKGROUND);
  sceneCanvas->setSize(w - 2 * PADDING, h - HEADER_HEIGHT - 2 * PADDING);
}

void Viewer::start() {

  // Setting up
  sceneCanvas->bindGLFWWindow(glfwWindow());

  // Layout
  performLayout();
  drawAll();
  setVisible(true);

  // Focus on canvas
  sceneCanvas->requestFocus();

  // Do the main loop
  nanogui::mainloop(MIN_REFRESH_INTERVAL);
}

void Viewer::draw(NVGcontext *ctx) {
  Screen::draw(ctx);
}
