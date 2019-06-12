#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char **argv) {

  // First setup the mpm grid
  mpm::Grid grid(Vector3f(0.5, 0.5, 0.5), Vector3f(1, 1, 1), Vector3u(100, 100, 100));
  for (int i = 0; i < 1000; i++) {
    std::unique_ptr<mpm::Particle> p = std::make_unique<mpm::Particle>(Vector3f(Math::random(), Math::random(), Math::random()));
    grid.addParticle(*p);
  }

  // Start the application
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position << 3, 3, 3;
  ThirdPersonCamera cam;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  Entity pcHolder;
  mpm::System system(grid);
  PointCloudRenderer pcRenderer("mpm-system");
  pcHolder.addComponent("mpm-system", system);
  pcHolder.addComponent("renderer", pcRenderer);
  scene.root.addChild(pcHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}