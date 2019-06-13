#include <iostream>
#include <fstream>

#include <toaru/toaru.h>

using namespace toaru;

int main() {

  int particlePerBox = 1000, numKeyframes = 200;

  std::vector<Vector3f> boxCenters = {
    Vector3f(0, 1, 0),
    Vector3f(-0.5f, 4, -0.5f),
    Vector3f(0.5f, 5, 0.5f)
  };

  mpm::Grid grid(Vector3f(0, 0, 5), Vector3f(10, 10, 10), Vector3u(100, 100, 100));
  for (int i = 0; i < boxCenters.size(); i++) {
    auto p = boxCenters[i];
    for (int j = 0; j < particlePerBox; j++) {
      grid.addParticle(mpm::Particle(Vector3f(
        Math::random(p.x() - 0.5f, p.x() + 0.5f),
        Math::random(p.y() - 0.5f, p.y() + 0.5f),
        Math::random(p.z() - 0.5f, p.z() + 0.5f)
      )));
    }
  }
  grid.hasGravity = true;

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

    grid.step();
  }
}