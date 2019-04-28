#include "physics/physics_system.h"

using namespace toaru;
using namespace std;

PhysicsSystem::PhysicsSystem() : isPlaying(true), deltaTime(0.0005), step(40) {}

void PhysicsSystem::play() {
  isPlaying = true;
}

void PhysicsSystem::pause() {
  isPlaying = false;
}

void PhysicsSystem::init() {
  initRestStates();
  buildAABBTrees();
}

void PhysicsSystem::update() {
  if (isPlaying) {
    for (int i = 0; i < step; i++) {
      stepOnce();
    }
  }
}

void PhysicsSystem::initRestStates() {
  for (auto &element : tetrahedrons) {
    element->initRestState();
  }
}

void PhysicsSystem::buildAABBTrees() {
  for (auto &obj : objects) {
    obj->buildAABBTree();
  }
}

void PhysicsSystem::updateObjects() {
  for (auto &obj : objects) {
    obj->update();
  }
}

void PhysicsSystem::processCollisions() {
  for (int objIndex = 0; objIndex < objects.size(); objIndex++) {
    auto &obj = objects[objIndex]; // unique_ptr reference
    for (Tetrahedron *tetra : obj->tetrahedrons) {
      for (int otherIndex = objIndex + 1; otherIndex < objects.size(); otherIndex++) {
        auto &otherObj = objects[otherIndex];
        if (otherObj != obj) {
          otherObj->aabbTree->handleCollision(*tetra);
        }
      }
    }
  }
}

void PhysicsSystem::stepOnce() {
  updateObjects();
  processCollisions();
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

  // First get the object and the points for constructing the tetrahedron
  PhysicsObject &obj = *objects[objId];
  const PhysicsMaterial &mat = obj.mat;
  Point &p1 = *points[i1], &p2 = *points[i2], &p3 = *points[i3], &p4 = *points[i4];

  // Cache the tetra id
  int tetraId = tetrahedrons.size();

  // Construct the tetrahedron and give it the corresponding faces
  auto tetra = make_unique<Tetrahedron>(mat, p1, p2, p3, p4);
  tetra->addFace(getFace(i1, i2, i3, i4));
  tetra->addFace(getFace(i4, i1, i3, i2));
  tetra->addFace(getFace(i2, i4, i3, i1));
  tetra->addFace(getFace(i4, i2, i1, i3));

  // Add the tetrahedron to the physics object and add it to the all tetrahedrons list
  obj.addTetrahedron(*tetra);
  tetrahedrons.push_back(move(tetra));
  return tetraId;
}

int PhysicsSystem::createBox(const PhysicsMaterial &mat, Vector3f center, Vector3f size,
                             Vector3u sub) {
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
        int i0 = points[i][j][k], i1 = points[i][j][k + 1], i2 = points[i + 1][j][k + 1],
            i3 = points[i + 1][j][k], i4 = points[i][j + 1][k], i5 = points[i][j + 1][k + 1],
            i6 = points[i + 1][j + 1][k + 1], i7 = points[i + 1][j + 1][k];
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

const Face &PhysicsSystem::getFace(int i1, int i2, int i3, int opposite) {
  std::vector<int> indices = {i1, i2, i3};
  std::sort(indices.begin(), indices.end());
  std::string hash = std::to_string(indices[0]) + "," + std::to_string(indices[1]) + "," +
                     std::to_string(indices[2]);

  // First make the face
  Point &p1 = *points[i1], &p2 = *points[i2], &p3 = *points[i3], &p4 = *points[opposite];
  auto ptr = make_unique<Face>(p1, p2, p3, p4);

  // Then mark it to be internal if the face is already found in the degenerate faces
  if (degenerateFaces.find(hash) != degenerateFaces.end()) {
    ptr->internal = true;
    degenerateFaces[hash]->internal = true;
  } else {
    degenerateFaces[hash] = ptr.get();
  }

  // Still put the face into faces vector and return the face reference
  faces.push_back(move(ptr));
  return *faces[faces.size() - 1].get();
}