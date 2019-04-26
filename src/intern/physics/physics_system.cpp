#include "physics/physics_system.h"
#ifdef _WIN32
#include "omp.h"
#endif

using namespace toaru;
using namespace std;

PhysicsSystem::PhysicsSystem() : isPlaying(true), deltaTime(0.0005), step(40) {}

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

int PhysicsSystem::addObject(const PhysicsMaterial &mat) {
  int id = objects.size();
  auto obj = make_unique<PhysicsObject>(mat);
  objects.push_back(move(obj));
  return id;
}

int PhysicsSystem::addPoint(const Vector3f &p, bool isFixed) {
  int id = points.size();
  auto point = make_unique<Point>(p, id, isFixed);
  points.push_back(move(point));
  return id;
}

int PhysicsSystem::addTetrahedron(int objId, int i1, int i2, int i3, int i4) {
  PhysicsObject &obj = *objects[objId];
  const PhysicsMaterial &mat = obj.mat;
  Point &p1 = *points[i1], &p2 = *points[i2], &p3 = *points[i3], &p4 = *points[i4];
  int tetraId = tetrahedrons.size();
  auto tetra = make_unique<Tetrahedron>(mat, p1, p2, p3, p4);
  obj.addTetrahedron(*tetra);
  tetrahedrons.push_back(move(tetra));
  return tetraId;
}

int PhysicsSystem::createBox(const PhysicsMaterial &mat, Vector3f center, Vector3f size, Vector3u sub) {
  int objId = addObject(mat);
  Vector3f start = center - size / 2.0f;
  Vector3f step = Vector3f(size.x() / sub.x(), size.y() / sub.y(), size.z() / sub.z());
  assert(sub.x() > 0 && sub.y() > 0 && sub.z() > 0);

  // First construct all the points
  std::vector<std::vector<std::vector<int>>> points(sub.x() + 1);
  for (int i = 0; i < sub.x() + 1; i++) {
    points[i] = std::vector<std::vector<int>>(sub.y() + 1);
    float x = start.x() + i * step.x();
    for (int j = 0; j < sub.y() + 1; j++) {
      points[i][j] = std::vector<int>(sub.z() + 1);
      float y = start.y() + j * step.y();
      for (int k = 0; k < sub.z() + 1; k++) {
        float z = start.z() + k * step.z();
        points[i][j][k] = addPoint(Vector3f(x, y, z));
      }
    }
  }

  // Then construct all the tetrahedrons
  for (int i = 0; i < sub.x(); i++) {
    for (int j = 0; j < sub.y(); j++) {
      for (int k = 0; k < sub.z(); k++) {
        bool startFrom000 = (i + j + k) % 2 == 0;
        int i0 = points[i][j][k], i1 = points[i + 1][j][k], i2 = points[i + 1][j][k + 1], 
            i3 = points[i][j][k + 1], i4 = points[i][j + 1][k], i5 = points[i + 1][j + 1][k], 
            i6 = points[i + 1][j + 1][k + 1], i7 = points[i][j + 1][k + 1];
        if (startFrom000) {
          addTetrahedron(objId, i0, i2, i1, i5);
          addTetrahedron(objId, i0, i3, i2, i7);
          addTetrahedron(objId, i0, i7, i2, i5);
          addTetrahedron(objId, i0, i5, i4, i7);
          addTetrahedron(objId, i6, i5, i2, i7);
        } else {
          addTetrahedron(objId, i0, i3, i1, i4);
          addTetrahedron(objId, i1, i3, i2, i6);
          addTetrahedron(objId, i1, i4, i3, i6);
          addTetrahedron(objId, i1, i4, i6, i5);
          addTetrahedron(objId, i4, i3, i6, i7);
        }
      }
    }
  }

  return objId;
}

void PhysicsSystem::createUnitCube(Vector3f position, Vector3f extents, const PhysicsMaterial &mat) {
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

  auto T1 = make_unique<Tetrahedron>(mat, getPoint(A), getPoint(B), getPoint(C), getPoint(E));
  auto T2 = make_unique<Tetrahedron>(mat, getPoint(A), getPoint(C), getPoint(D), getPoint(G));
  auto T3 = make_unique<Tetrahedron>(mat, getPoint(A), getPoint(H), getPoint(E), getPoint(G));
  auto T4 = make_unique<Tetrahedron>(mat, getPoint(C), getPoint(E), getPoint(F), getPoint(G));
  auto T5 = make_unique<Tetrahedron>(mat, getPoint(A), getPoint(C), getPoint(G), getPoint(E));

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
  auto point = make_unique<Point>(position, 0);
  points.push_back(move(point));
  return *points[points.size()-1];
}

void PhysicsSystem::makeFace(std::unique_ptr<Tetrahedron> &tet) {
  // Build or get four faces
  // 1, 2, 3
  auto f1 = std::make_unique<Face>(*tet->points[0], *tet->points[1], *tet->points[2], *tet->points[3]);
  // 4, 1, 3
  auto f2 = std::make_unique<Face>(*tet->points[3], *tet->points[0], *tet->points[2], *tet->points[1]);
  // 2, 4, 3
  auto f3 = std::make_unique<Face>(*tet->points[1], *tet->points[3], *tet->points[2], *tet->points[0]);
  // 4, 2, 1
  auto f4 = std::make_unique<Face>(*tet->points[3], *tet->points[1], *tet->points[0], *tet->points[2]);

  faces.push_back(move(f1));
  tet->faces.push_back(faces[faces.size() - 1].get());
  faces.push_back(move(f2));
  tet->faces.push_back(faces[faces.size() - 1].get());
  faces.push_back(move(f3));
  tet->faces.push_back(faces[faces.size() - 1].get());
  faces.push_back(move(f4));
  tet->faces.push_back(faces[faces.size() - 1].get());
}
