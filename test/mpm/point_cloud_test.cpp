#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char **argv) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  ThirdPersonCamera cam;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  Entity pcHolder;
  PointCloud pc({
    Vector3f(0, 0, 0),
    Vector3f(0, 0, 1),
    Vector3f(1, 0, 0),
    Vector3f(1, 0, 1)
  }, {
    Vector3f(1, 1, 1),
    Vector3f(1, 1, 1),
    Vector3f(1, 1, 1),
    Vector3f(1, 1, 1)
  });
  PointCloudRenderer pcRenderer("point_cloud");
  pcHolder.addComponent("point_cloud", pc);
  pcHolder.addComponent("renderer", pcRenderer);
  scene.root.addChild(pcHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}