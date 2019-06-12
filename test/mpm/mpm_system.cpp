#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char **argv) {

  // First setup the mpm grid
  mpm::Grid grid(Vector3f(10, 10, 10), Vector3f(20, 20, 20), Vector3u(20, 20, 20));
  for (int i = 0; i < 1000; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()*20, Math::random()*20, Math::random()*20));
    p->mass = 0.01;
    grid.addParticle(*p);
  }
  grid.hasGravity = true;
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