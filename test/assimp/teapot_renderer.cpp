/**
 * In this example we demonstrated how do we use assimp object to
 */

#include <nash/nash.h>

using namespace nash;

int main(int argc, char *argv[]) {
  Nash::init(argc, argv);

  Scene scene;
  ThirdPersonCamera camCtrl;
  scene.getCamera().setController(camCtrl);

  AssimpObject teapot(Path::getAbsolutePathTo("./model/teapot.obj"));
  Rotator rotator("rotator");
  teapot.attachScript(rotator);
  scene.addObject(teapot);

  Viewer viewer(1280, 720, "Teapot Viewer test", scene);
  viewer.start();

  Nash::shutdown();
}
