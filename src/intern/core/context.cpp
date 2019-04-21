#include "core/context.h"

using namespace toaru;

Context::Context() : keyStatus(512) {

  // Keys
  for (int i = 0; i < keyStatus.size(); i++) {
    keyStatus[i] = false;
  }

  // Modifier keys
  shiftStatus = false;
  ctrlStatus = false;
  altStatus = false;
  superStatus = false;

  // Mouse
  leftButtonDown = false;
  rightButtonDown = false;
  cursorInside = false;
  cursorPosition = Vector2i(0, 0);
  newCursorPosition = Vector2i(0, 0);

  // Scroll
  scrollPosition = Vector2f(0, 0);
  newScrollPosition = Vector2f(0, 0);

  // Time
  using namespace std::chrono;
  start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
  curr = start;
}

GLFWwindow *Context::getGLFWWindow() {
  return window;
}

int Context::getWindowWidth() {
  return windowWidth;
}

int Context::getWindowHeight() {
  return windowHeight;
}

int Context::getCanvasWidth() {
  return canvasWidth;
}

int Context::getCanvasHeight() {
  return canvasHeight;
}

double Context::getDeltaTime() {
  return deltaTime.count() / 1000.0;
}

double Context::getElapsedTime() {
  return elapsedTime.count() / 1000.0;
}

std::chrono::milliseconds Context::getAbsTime() {
  return curr;
}

bool Context::getKey(char key) {
  switch (key) {
    case '\t': return keyStatus[GLFW_KEY_TAB];
    case '\n': return keyStatus[GLFW_KEY_ENTER];
    default: return keyStatus[toupper(key)];
  }
}

bool Context::getDirectionKey(Context::Direction dir) {
  switch (dir) {
    case Context::Direction::Up: return keyStatus[GLFW_KEY_UP];
    case Context::Direction::Down: return keyStatus[GLFW_KEY_DOWN];
    case Context::Direction::Left: return keyStatus[GLFW_KEY_LEFT];
    case Context::Direction::Right: return keyStatus[GLFW_KEY_RIGHT];
  }
}

bool Context::getMouseLeft() {
  return leftButtonDown;
}

bool Context::getMouseRight() {
  return rightButtonDown;
}

bool Context::cursorIsInside() {
  return cursorInside;
}

const Vector2i &Context::getCursorPosition() {
  return exactCursorPosition;
}

Vector2i Context::getCursorMovement() {
  return newCursorPosition - cursorPosition;
}

const Vector2f &Context::getScrollPosition() {
  return scrollPosition;
}

Vector2f Context::getScrollMovement() {
  return newScrollPosition - scrollPosition;
}

Matrix4f &Context::getMainCameraView() {
  return view;
}

Matrix4f &Context::getMainCameraProj() {
  return proj;
}

Matrix4f &Context::getMainCameraViewProj() {
  return viewProj;
}

void Context::resetCursor() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Context::hideCursor() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Context::disableCursor() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Context::setCursorPos(int x, int y) {
  glfwSetCursorPos(window, x, y);
}

void Context::centerCursorPos() {
  setCursorPos(windowWidth / 2, windowHeight / 2);
}

void Context::setMainCameraMatrices(Matrix4f &view, Matrix4f &proj) {
  this->view = view;
  this->proj = proj;
  this->viewProj = proj * view;
}

void Context::beforeDraw() {}

void Context::afterDraw() {
  cursorPosition = newCursorPosition;
  scrollPosition = newScrollPosition;

  // Update the time related
  using namespace std::chrono;
  auto temp = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
  elapsedTime = temp - start;
  deltaTime = temp - curr;
  curr = temp;
}

void Context::bindGLFWWindow(GLFWwindow *win) {
  window = win;
  glfwGetWindowSize(window, &windowWidth, &windowHeight);
}

void Context::setCanvasSize(int width, int height) {
  this->canvasWidth = width;
  this->canvasHeight = height;
}

bool Context::keyboardEvent(int key, int scancode, int action, int modifiers) {

  // Key status
  if (key >= 0 && key < keyStatus.size()) {
    if (action == GLFW_PRESS) {
      keyStatus[key] = true;
    } else if (action == GLFW_RELEASE) {
      keyStatus[key] = false;
    }
  }

  // Modifier status
  extractModifiers(modifiers);

  // Always responding to keyboard event
  return true;
}

bool Context::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) {
  exactCursorPosition = p;
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    rightButtonDown = down;
  } else if (button == GLFW_MOUSE_BUTTON_LEFT) {
    leftButtonDown = down;
  }
  extractModifiers(modifiers);
  return true;
}

bool Context::mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) {
  exactCursorPosition = p;
  newCursorPosition = cursorPosition + rel;
  cursorInside = true;
  extractModifiers(modifiers);
  return true;
}

bool Context::mouseEnterEvent(const Vector2i &p, bool enter) {
  exactCursorPosition = p;
  cursorInside = enter;
  return true;
}

bool Context::scrollEvent(const Vector2i &p, const Vector2f &rel) {
  newScrollPosition = scrollPosition + rel;
  return true;
}

void Context::extractModifiers(int modifiers) {
  shiftStatus = GLFW_MOD_SHIFT & modifiers;
  ctrlStatus = GLFW_MOD_CONTROL & modifiers;
  altStatus = GLFW_MOD_ALT & modifiers;
  superStatus = GLFW_MOD_SUPER & modifiers;
}
