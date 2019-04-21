#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char *argv[]) {
  toaru::init();

  Scene scene;

  Entity camHolder;
  Camera cam;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}