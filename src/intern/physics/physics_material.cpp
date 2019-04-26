#include "physics/physics_material.h"

using namespace toaru;

PhysicsMaterial::PhysicsMaterial(float density, float gamma1, float gamma2, const MaterialTensor &k, const MaterialTensor &d) 
  : density(density), gamma1(gamma1), gamma2(gamma2), k(k), d(d) {}