#include "physics/physics_system.h"
#ifdef _WIN32
#include "omp.h"
#endif

using namespace toaru;
using namespace std;

PhysicsSystem::PhysicsSystem()
  : isPlaying(false),
    deltaTime(0.0005),
    step(40) {
}

void PhysicsSystem::play() {
  isPlaying = true;
}

void PhysicsSystem::pause() {
  isPlaying = false;
}

void PhysicsSystem::stepOnce() {
#pragma omp parallel for
  for (int i = 0; i < tetrahedrons.size(); i++) {
    auto &element = tetrahedrons[i];
    element->update(deltaTime);
  }
#pragma omp parallel for
  for (int i = 0; i < points.size(); i++) {
    auto &element = points[i];
    element->update(deltaTime);
  }
}

void PhysicsSystem::init() {
  for (auto &element : tetrahedrons) {
    makeFace(element);
    element->initRestState();
  }
}

void PhysicsSystem::update() {
  if (isPlaying) {
    for (int i = 0; i < step; i++) {
      stepOnce();
    }
  }
}

void PhysicsSystem::createUnitCube(Vector3f position, Vector3f extents, float density,
                                   const PhysicsMaterial &KMat, const PhysicsMaterial &DMat) {
  Vector3f pos = position;
  Vector3f ext = extents;
  auto A = Vector3f(-ext(0, 0) + pos(0, 0), ext(1, 0) + pos(1, 0), -ext(2, 0) + pos(2, 0));
  auto B = Vector3f(-ext(0, 0) + pos(0, 0), ext(1, 0) + pos(1, 0), ext(2, 0) + pos(2, 0));
  auto C = Vector3f(ext(0, 0) + pos(0, 0), ext(1, 0) + pos(1, 0), ext(2, 0) + pos(2, 0));
  auto D = Vector3f(ext(0, 0) + pos(0, 0), ext(1, 0) + pos(1, 0), -ext(2, 0) + pos(2, 0));
  auto E = Vector3f(-ext(0, 0) + pos(0, 0), -ext(1, 0) + pos(1, 0), ext(2, 0) + pos(2, 0));
  auto F = Vector3f(ext(0, 0) + pos(0, 0), -ext(1, 0) + pos(1, 0), ext(2, 0) + pos(2, 0));
  auto G = Vector3f(ext(0, 0) + pos(0, 0), -ext(1, 0) + pos(1, 0), -ext(2, 0) + pos(2, 0));
  auto H = Vector3f(-ext(0, 0) + pos(0, 0), -ext(1, 0) + pos(1, 0), -ext(2, 0) + pos(2, 0));

  auto T1 = make_unique<Tetrahedron>(density, KMat, DMat, getPoint(A), getPoint(B), getPoint(C),
                                     getPoint(E));
  auto T2 = make_unique<Tetrahedron>(density, KMat, DMat, getPoint(A), getPoint(C), getPoint(D),
                                     getPoint(G));
  auto T3 = make_unique<Tetrahedron>(density, KMat, DMat, getPoint(A), getPoint(H),
                                     getPoint(E),
                                     getPoint(G));
  auto T4 = make_unique<Tetrahedron>(density, KMat, DMat, getPoint(C), getPoint(E), getPoint(F),
                                     getPoint(G));
  auto T5 = make_unique<Tetrahedron>(density, KMat, DMat, getPoint(A), getPoint(C), getPoint(G),
                                     getPoint(E));

  tetrahedrons.push_back(move(T1));
  tetrahedrons.push_back(move(T2));
  tetrahedrons.push_back(move(T3));
  tetrahedrons.push_back(move(T4));
  tetrahedrons.push_back(move(T5)); 
}

Point &PhysicsSystem::getPoint(Vector3f position) {
  // Temp face
  // float i = std::numeric_limits<float>::epsilon() * 3.0;
  // std::cout << i << std::endl;
  auto result =
    find_if(points.begin(), points.end(), [&](const unique_ptr<Point> &point)
    {
      // std::numeric_limits<float>::epsilon()
      return (point->position - position).norm() <= numeric_limits<float>::epsilon() * 3.0;
    });

  // If found
  if (result != points.end()) {
    return (**result);
  }

  // if not found, create one
  auto point = make_unique<Point>(position);
  points.push_back(move(point));
  return *points[points.size()-1];
}

void PhysicsSystem::makeFace(std::unique_ptr<Tetrahedron> &tet) {
  // Build or get four faces
  // 1, 2, 3
  auto f1 = std::make_unique<Face>(*tet->points[0], *tet->points[1], *tet->points[2], *tet->points[3]);
  f1->updateNormal();
  // 4, 1, 3
  auto f2 = std::make_unique<Face>(*tet->points[3], *tet->points[0], *tet->points[2], *tet->points[1]);
  f2->updateNormal();
  // 2, 4, 3
  auto f3 = std::make_unique<Face>(*tet->points[1], *tet->points[3], *tet->points[2], *tet->points[0]);
  f3->updateNormal();
  // 4, 2, 1
  auto f4 = std::make_unique<Face>(*tet->points[3], *tet->points[1], *tet->points[0], *tet->points[2]);
  f4->updateNormal();

  faces.push_back(move(f1));
  tet->faces.push_back(*faces[faces.size() - 1]);
  faces.push_back(move(f2));
  tet->faces.push_back(*faces[faces.size() - 1]);
  faces.push_back(move(f3));
  tet->faces.push_back(*faces[faces.size() - 1]);
  faces.push_back(move(f4));
  tet->faces.push_back(*faces[faces.size() - 1]);
}
