/**
 * In this example we tested if the space (' ') key can trigger game event. In
 * the initial state, the cube will be shown on the screen. When you press the
 * space (' ') bar the cube will disappear and the sphere will show up. Each
 * time you press space (' ') you are toggling the show/hide state of these two
 * objects.
 *
 * We are using an Adhoc Toggle class for this process. You can go to
 * `adhoc/script/Toggle.h` for more details.
 */

#include <nash/nash.h>

using namespace nash;

int main(int argc, char *argv[]) {
  Nash::init(argc, argv);

  Scene scene;
  scene.getCamera().position << 3, 3, 3;

  Cube cube;
  Toggle cubeToggle("toggle",
                    true); // `true` means the initial state is showing
  cube.attachScript(cubeToggle);
  scene.addObject(cube);

  Sphere sphere;
  Toggle sphereToggle("toggle",
                      false); // `false` means the initial state is hiding
  sphere.attachScript(sphereToggle);
  scene.addObject(sphere);

  Viewer viewer(1280, 720, "Cube test", scene);
  viewer.start();

  Nash::shutdown();
}
