#include <iostream>
#include <fstream>

#include <toaru/toaru.h>

using namespace toaru;

int main() {

  int particlePerBox = 1000, numKeyframes = 200, skipFrame = 10;

  mpm::Grid grid(Vector3f(5, 5, 5), Vector3f(10, 10, 10), Vector3u(100, 100, 100));
  for (int i = 0; i < particlePerBox; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+2.1, Math::random()+1.6, Math::random()+2.1));
    p->mass = 1.5;
    p->color = Vector3f(1,0,0);
    grid.addParticle(*p);
  }
  for (int i = 0; i < particlePerBox; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+2.7, Math::random()+2.7, Math::random()+2.7));
    p->mass = 3.5;
    p->color = Vector3f(0,1,0);
    grid.addParticle(*p);
  }
  for (int i = 0; i < particlePerBox; i++) {
    mpm::Particle* p = new mpm::Particle(Vector3f(Math::random()+1.5, Math::random()+0.5, Math::random()+1.5));
    p->mass = 1.1;
    p->color = Vector3f(0,0,1);
    grid.addParticle(*p);
  }
  grid.init();
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

    for (int j = 0; j < skipFrame; j++) {
      grid.step();
    }
  }
}