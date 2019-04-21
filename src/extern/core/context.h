#ifndef TOARU_CONTEXT_H
#define TOARU_CONTEXT_H

#include "utility/math.h"
#include <chrono>
#include <ctype.h>
#include <nanogui/nanogui.h>
#include <vector>

namespace toaru {
  class Context {
  public:
    enum Direction { Up, Down, Left, Right };

    Context();

    // Component side getters
    GLFWwindow *getGLFWWindow();
    int getWindowWidth();
    int getWindowHeight();
    int getCanvasWidth();
    int getCanvasHeight();
    double getDeltaTime();
    double getElapsedTime();
    std::chrono::milliseconds getAbsTime();
    bool getKey(char key);
    bool getDirectionKey(Direction dir);
    bool getMouseLeft();
    bool getMouseRight();
    bool cursorIsInside();
    const Vector2i &getCursorPosition();
    Vector2i getCursorMovement();
    const Vector2f &getScrollPosition();
    Vector2f getScrollMovement();
    Matrix4f &getMainCameraView();
    Matrix4f &getMainCameraProj();
    Matrix4f &getMainCameraViewProj();

    // Component side operators
    void resetCursor();
    void hideCursor();
    void disableCursor();
    void setCursorPos(int x, int y);
    void centerCursorPos();
    void setMainCameraMatrices(Matrix4f &view, Matrix4f &proj);

    // Controller side
    void beforeDraw();
    void afterDraw();
    void bindGLFWWindow(GLFWwindow *window);
    void setCanvasSize(int width, int height);
    bool keyboardEvent(int key, int scancode, int action, int modifiers);
    bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    bool mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers);
    bool mouseEnterEvent(const Vector2i &p, bool enter);
    bool scrollEvent(const Vector2i &p, const Vector2f &rel);

  private:
    GLFWwindow *window;
    int windowWidth, windowHeight;
    int canvasWidth, canvasHeight;

    std::vector<bool> keyStatus;
    bool shiftStatus, ctrlStatus, altStatus, superStatus;
    bool leftButtonDown, rightButtonDown, cursorInside;
    Vector2i exactCursorPosition, cursorPosition, newCursorPosition;
    Vector2f scrollPosition, newScrollPosition;

    std::chrono::milliseconds start, curr, elapsedTime, deltaTime;

    Matrix4f view, proj, viewProj;

    void extractModifiers(int modifiers);
  };
}

#endif