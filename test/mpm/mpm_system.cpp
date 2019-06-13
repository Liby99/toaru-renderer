#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char **argv) {

  // First setup the mpm grid
  mpm::Grid grid(Vector3f(5, 5, 5), Vector3f(10, 10, 10), Vector3u(80, 80, 80));
  for (int i = 0; i < 1000; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+2, Math::random()+3, Math::random()+2));
    p->mass = 10.0;
    p->color = Vector3f(1,0,0);
    grid.addParticle(*p);
  }
  for (int i = 0; i < 1000; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+2.5, Math::random()+5, Math::random()+2.5));
    p->mass = 10.0;
    p->color = Vector3f(0,1,0);
    grid.addParticle(*p);
  }
  for (int i = 0; i < 1000; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+1.5, Math::random()+1, Math::random()+1.5));
    p->mass = 10.0;
    p->color = Vector3f(0,0,1);
    grid.addParticle(*p);
  }
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