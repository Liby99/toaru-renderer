#include <iostream>
#include <nash/nash.h>

using namespace nash;

std::string toString(bool b) { return b ? "true" : "false"; }

void printResult(bool expected, bool get) {
  std::cout << "> Expected: " << toString(expected) << ", Get: " << toString(get) << ". ";
  std::cout << (expected == get ? "Pass. " : "Failed. ") << std::endl;
}

int main() {

  // simple triangle
  std::cout << "Testing triangle intersection..." << std::endl;
  Triangle t({Vector3f(0, 0, 0), Vector3f(0, 0, 3), Vector3f(3, 0, 0)});
  Ray tr(Vector3f(1, 1, 1), Vector3f(-0.5, -1, -0.5));
  Intersection tintersection(tr);
  bool tresult = t.intersect(tr, tintersection);
  printResult(true, tresult);

  // simple box
  std::cout << "Testing bounding box intersection..." << std::endl;
  BoundingBox b({Vector3f(0, 0, 0), Vector3f(1, 1, 1)});
  Ray br(Vector3f(0.5, 0.5, 0.5), Vector3f(1, 1, 1));
  Intersection bintersection(br);
  bool bresult = b.intersect(br, bintersection);
  printResult(true, bresult);
}
