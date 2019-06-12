#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char **argv) {
  mpm::Grid grid(Vector3f(0.5, 0.5, 0.5), Vector3f(1, 1, 1), Vector3u(100, 100, 100));
  for (int i = 0; i < 1000; i++) {
    std::unique_ptr<mpm::Particle> p = std::make_unique<mpm::Particle>(Vector3f(Math::random(), Math::random(), Math::random()));
    grid.addParticle(*p);
  }
}