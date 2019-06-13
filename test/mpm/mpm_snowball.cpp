#include <toaru/toaru.h>
#include <random>
#include <fstream>

using namespace toaru;

int particlePerBox = 1000, numKeyframes = 150, skipFrame = 20;

void sampleSphere(std::vector<Vector3f> &samples, Vector3f center, float radius) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 gen(seed);
  std::uniform_real_distribution<double> uniform(0.0, 1.0);

  int count = samples.size();

  for (int i = 0; i < count; i++) {
    double theta = 2 * Math::PI * uniform(gen);
    double phi = acos(1 - 2 * uniform(gen));
    double x = sin(phi) * cos(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(phi);
    double r = std::cbrt(uniform(gen)) * radius;
    samples[i] = Vector3f(x * r + center.x(), y * r + center.y(),
                          z * r + center.z());
  }
}

int main(int argc, char **argv) {

  // First setup the mpm grid
  mpm::Grid grid(Vector3f(5, 5, 5), Vector3f(10, 10, 10), Vector3u(100, 100, 100));

  std::vector<Vector3f> samples(8000);
  sampleSphere(samples, Vector3f(5,5,5), 0.5);

  for (int i = 0; i < samples.size(); i++) {
    mpm::Particle* p = new mpm::Particle(samples[i]);
    p->mass = 1.0;
    p->velocity = Vector3f(-200, 0, -200);
    p->color = Vector3f(1,0,0);
    grid.addParticle(*p);
  }
  grid.xi = 5;
  grid.nu = 0.2f;
  grid.theta_c = 2.5 * 1e-2;
  grid.theta_s = 7.5 * 1e-3;
  grid.e = 10000.0f;
  grid.deltaTime = 0.0002f;

  grid.init();
  grid.hasGravity = true;
  grid.step();

  for (int i = 0; i < numKeyframes; i++) {
    std::string filename = "frame_" + std::to_string(i) + ".json";
    
    std::ofstream file;
    file.open(filename);
    file << "[";
    for (int j = 0; j < grid.particles.size(); j++) {
      auto p = grid.particles[j].position;
      file << "[" << p.x() << "," << p.y() << "," << p.z() << "]";
      if (j < grid.particles.size() - 1) {
        file << ",";
      }
    }
    file << "]";
    file.close();

    for (int j = 0; j < skipFrame; j++) {
      grid.step();
    }
    std::cout << "Step: " << i << std::endl;
  }

  exit(0);
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
