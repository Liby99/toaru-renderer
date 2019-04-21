#ifndef SCENE_H
#define SCENE_H

namespace nash {
  class Context;
  class Camera;
  class Object;

  class Scene {
  public:
    Scene();
    ~Scene();

    void setSize(int width, int height);

    Camera &getCamera();
    Object &getRoot();
    void addObject(Object &object);

    void start(Context &context);
    void update(Context &context);
    void render(Context &context);

  private:
    Camera *camera;
    Object *root;
  };
}

#endif
