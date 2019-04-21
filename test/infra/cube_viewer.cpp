#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position = Vector3f(0, 0, 5);
  Camera cam;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  Entity cubeHolder;
  Cube cubeMesh;
  Lambertian lambMat;
  MeshRenderer renderer;
  cubeHolder.addComponent("mesh", cubeMesh);
  cubeHolder.addComponent("material", lambMat);
  cubeHolder.addComponent("renderer", renderer);
  scene.root.addChild(cubeHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}