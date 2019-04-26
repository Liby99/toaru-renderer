#include "physics/physics_material.h"

using namespace toaru;

PhysicsMaterial::PhysicsMaterial(float density, const MaterialTensor &k, const MaterialTensor &d) 
  : density(density), k(k), d(d) {}