#include "mpm/system.h"

using namespace toaru::mpm;

System::System(Grid &grid) : grid(grid), PointCloud({}, {}) {}

void System::init() {
  int particleAmount = grid.particles.size();
  indices = MatrixXu(1, particleAmount);
  positions = MatrixXf(3, particleAmount);
  colors = MatrixXf(3, particleAmount);
  for (int i = 0; i < particleAmount; i++) {
    indices.col(i) << i;
  }
}

void System::update() {
  grid.step(); // First step
  // clear(); // Clear the point cloud
  fill(); // Fill in the data
}

void System::fill() {
  int particleAmount = grid.particles.size();
  for (int i = 0; i < particleAmount; i++) {
    const Particle &p = grid.particles[i];
    positions.col(i) = p.position;
    colors.col(i) = Vector3f(p.Jp, p.Jp, p.Jp);
  }
}