/**
 * In this example you can use `wasd` to move your self around, use space (' ')
 * to move yourself upward, and use `x` to move yourself downward. You can also
 * use your mouse to rotate, and scroll wheel to move yourself toward or away
 * from the target. We placed three cubes in the scene to let you get a precise
 * sense of travelling.
 *
 * Note that we only need to attach the `ThirdPersonCamera` script to the camera
 * to get this effect. `ThirdPersonCamera` is inherited from `Script<Camera>`
 */

#include <nash/nash.h>

using namespace nash;

class TintColor : public Script<Object> {
public:
  TintColor(const std::string &name, const Vector3f &color) : Script<Object>(name), color(color) {}

  void preRender() {
    if (target->hasShader()) {
      target->getShader().setUniform("DiffuseColor", color);
    }
  }

  void postRender() {
    if (target->hasShader()) {
      target->getShader().setUniform("DiffuseColor", Vector3f(0.1, 0.1, 0.1));
    }
  }

private:
  Vector3f color;
};

int main(int argc, char *argv[]) {
  Nash::init(argc, argv);

  Scene scene;
  FirstPersonCamera camCtrl;
  scene.getCamera().setController(camCtrl);

  Cube cube;
  cube.transform.scale << 1.2, 1, 0.8;
  cube.transform.position << 0.8, -0.1, -1;
  TintColor tintGreen("tint-green", Vector3f(0.1, 0.5, 0.3));
  cube.attachScript(tintGreen);
  scene.addObject(cube);

  Cube cube2;
  cube2.transform.scale << 1, 0.4, 0.8;
  cube2.transform.position << -1.2, -0.45, 0.3;
  cube2.transform.rotation = Quaternionf(AngleAxisf(1.0, Vector3f::UnitY()));
  TintColor tintBlue("tint-blue", Vector3f(0.1, 0.5, 0.5));
  cube2.attachScript(tintBlue);
  scene.addObject(cube2);

  Cube plane;
  plane.transform.scale << 4, 0.1, 4;
  plane.transform.position << 0, -0.65, 0;
  TintColor tintRed("tint-red", Vector3f(0.3, 0.1, 0.1));
  plane.attachScript(tintRed);
  scene.addObject(plane);

  Viewer viewer(1280, 720, "Cube test", scene);
  viewer.start();

  Nash::shutdown();
}
