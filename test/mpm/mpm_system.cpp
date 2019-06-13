#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char **argv) {

  // First setup the mpm grid
  mpm::Grid grid(Vector3f(5, 5, 5), Vector3f(10, 10, 10), Vector3u(100, 100, 100));
  for (int i = 0; i < 600; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+2.1, Math::random()+1.6, Math::random()+2.1));
    p->mass = 3.0;
    p->color = Vector3f(1,0,0);
    grid.addParticle(*p);
  }
  for (int i = 0; i < 600; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+2.7, Math::random()+2.7, Math::random()+2.7));
    p->mass = 3.0;
    p->color = Vector3f(0,1,0);
    grid.addParticle(*p);
  }
  for (int i = 0; i < 600; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+1.5, Math::random()+0.5, Math::random()+1.5));
    p->mass = 3.0;
    p->color = Vector3f(0,0,1);
    grid.addParticle(*p);
  }

  // Viscosity
  /*grid.xi = 10;
  grid.nu = 0.495f;
  grid.theta_c = 2.5 * 1e-4;
  grid.theta_s = 7.5 * 1e-5;
  grid.e = 140000.0f;
  grid.deltaTime = 0.0002f;*/

  grid.xi = 50;
  grid.nu = 0.2f;
  grid.theta_c = 2.5 * 1e-2;
  grid.theta_s = 7.5 * 1e-3;
  grid.e = 140000.0f;
  grid.deltaTime = 0.0002f;
  grid.generateVolume = false;
  grid.initialVolume = 1.0f;

  grid.init();
  grid.hasGravity = true;
  // Start the application
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position << -3, 3, -3;
  ThirdPersonCamera cam;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  Entity pcHolder;
  mpm::System system(grid);
  PointCloudRenderer pcRenderer("mpm-system");
  pcHolder.addComponent("mpm-system", system);
  pcHolder.addComponent("renderer", pcRenderer);
  scene.root.addChild(pcHolder);

  Entity floorHolder;
  floorHolder.transform.scale << 100, 100, 100;
  floorHolder.transform.position << 0, 0, 0;
  Plane plane;
  Lambertian floorMat;
  MeshRenderer meshRenderer;
  floorMat.diffuseColor << 0.3, 0.3, 0.3;
  floorHolder.addComponent("mesh", plane);
  floorHolder.addComponent("material", floorMat);
  floorHolder.addComponent("renderer", meshRenderer);
  scene.root.addChild(floorHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}