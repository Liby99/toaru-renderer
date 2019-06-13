#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char **argv) {

  // First setup the mpm grid
  mpm::Grid grid(Vector3f(5, 5, 5), Vector3f(10, 10, 10), Vector3u(80, 80, 80));
  for (int i = 0; i < 5000; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+2, Math::random()+2, Math::random()+2));
    p->mass = 1.0;
    grid.addParticle(*p);
  }
  grid.hasGravity = true;
  // Start the application
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position << -3, -3, -3;
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