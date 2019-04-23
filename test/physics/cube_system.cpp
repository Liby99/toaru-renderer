#include <toaru/toaru.h>

using namespace toaru;

class CubePhysicsSystem : public PhysicsSystem {
public:
  bool pressingR = false, pressingP = false;

  virtual void init() {

    int countX = 2;
    int countY = 8;
    int countZ = 2;
    Vector3f extent = Vector3f(1, 1, 1);
    Vector3f start = Vector3f(-countX * extent.x(), 0, -countZ * extent.z());

    Ks.push_back(std::make_unique<PhysicsMaterial>(1000000.0f, 0.1f));
    Ds.push_back(std::make_unique<PhysicsMaterial>(3000.0f, 6000.f, false));

    for (unsigned int i = 0; i < countX; i++) {
      for (unsigned int j = 0; j < countY; j++) {
        for (unsigned int k = 0; k < countZ; k++) {
          Vector3f pos =
            Vector3f(start.x() + i * extent.x() * 2.0, start.y() + j * extent.y() * 2.0,
                     start.z() + k * extent.z() * 2.0);
          createUnitCube(pos, extent, 1000.f, *Ks[0], *Ds[0]);
        }
      }
    }

    // createUnitCube(Vector3f(-3,0,0), Vector3f(1, 1, 1), 1.f, 180.0f, 0.4f);
    // createUnitCube(Vector3f(-1,0,0), Vector3f(1, 1, 1), 1.f, 180.0f, 0.4f);
    // createUnitCube(Vector3f(1,0,0), Vector3f(1, 1, 1), 1.f, 180.0f, 0.4f);
    // createUnitCube(Vector3f(3,0,0), Vector3f(1, 1, 1), 1.f, 180.0f, 0.4f);

    PhysicsSystem::init();
  }

  virtual void update() {
    if (!pressingR) {
      if (context().getKey('R')) {
        pressingR = true;
        play();
      }
    } else {
      if (!context().getKey('R')) {
        pressingR = false;
      }
    }

    if (!pressingR) {
      if (context().getKey('P')) {
        pressingP = true;
        pause();
      }
    } else {
      if (!context().getKey('P')) {
        pressingP = false;
      }
    }

    if (context().getKey('T')) {
      for (auto element : points) {
        if (element->position(1, 0) > 4.0)
          element->addForce(Vector3f(10000, 10000, 10000));
      }
    }

    if (context().getKey('G')) {
      for (auto element : points) {
        element->position(1, 0) += 10;
      }
    }
    PhysicsSystem::update();
  }
};

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position = Vector3f(18, 18, 18);
  ThirdPersonCamera cam;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  Entity physicsHolder;
  CubePhysicsSystem sys;
  Lambertian lambMat;
  PhysicsSystemRenderer renderer;
  physicsHolder.addComponent("system", sys);
  physicsHolder.addComponent("material", lambMat);
  physicsHolder.addComponent("renderer", renderer);
  scene.root.addChild(physicsHolder);

  Entity floorHolder;
  floorHolder.transform.scale << 100, 100, 100;
  floorHolder.transform.position << 0, -2, 0;
  Plane plane;
  Lambertian floorMat;
  MeshRenderer meshRenderer;
  floorMat.diffuseColor << 0.3, 0.3, 0.3;
  floorHolder.addComponent("mesh", plane);
  floorHolder.addComponent("material", floorMat);
  floorHolder.addComponent("renderer", meshRenderer);
  scene.root.addChild(floorHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}
