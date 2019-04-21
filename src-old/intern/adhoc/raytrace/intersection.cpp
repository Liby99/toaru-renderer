#include "adhoc/raytrace/intersection.h"

using namespace nash;

Intersection::Intersection(const Ray &ray) {
  hit = false;
  t = 0;
}
