/**
 * Simple test of whether the system can show a rotating cube on the screen. You
 * can notice that we have attached a script `Rotator` to the cube, and that is
 * the main reason that the cube is rotating (around y-axis)
 */

#include <nash/nash.h>

using namespace nash;

int main(int argc, char *argv[]) {
  Nash::init(argc, argv);

  Scene scene;
  scene.getCamera().position << 3, 3, 3;
  Cube cube;
  scene.addObject(cube);

  // ========   ATTENTION   ======== //
  Rotator rotator("rotator");
  cube.attachScript(rotator);
  // ======== END ATTENTION ======== //

  Viewer viewer(1280, 720, "Rotating Cube test", scene);
  viewer.start();

  Nash::shutdown();
}
