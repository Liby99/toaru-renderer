#include <nash/nash.h>

using namespace nash;

int main(int argc, char *argv[]) {
  Nash::init(argc, argv);

  Scene scene;
  ThirdPersonCamera camCtrl;
  camCtrl.incline = PI / 6;
  scene.getCamera().setController(camCtrl);

  AssimpObject teapot(Path::getAbsolutePathTo("./model/teapot.obj"));
  scene.addObject(teapot);

  Plane plane;
  plane.transform.scale << 5, 5, 5;
  scene.addObject(plane);

  Viewer viewer(1280, 720, "Teapot Scene test", scene);
  viewer.start();

  Nash::shutdown();
}
