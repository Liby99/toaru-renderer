#include "mpm/system.h"
//#include <Windows.h>

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
bool pressed = false;
void System::update() {
  //if(GetAsyncKeyState(VK_INSERT)){
    //if(!pressed){
      grid.step(); // First step
      //pressed = true;
    //}
  //}else {
  //  pressed = false;
  //}
  //if(GetAsyncKeyState(VK_HOME)) {
  //  grid.step(); 
  //}
  // clear(); // Clear the point cloud
  fill(); // Fill in the data
}

void System::fill() {
  int particleAmount = grid.particles.size();
  for (int i = 0; i < particleAmount; i++) {
    const Particle &p = grid.particles[i];
    positions.col(i) = p.position;
    colors.col(i) = p.color; //p.velocity;
  }
}