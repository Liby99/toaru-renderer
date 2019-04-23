#include <toaru/toaru.h>

using namespace toaru;

class CubePhysicsSystem : public PhysicsSystem {
public:
  bool pressingR = false, pressingP = false, pressingY = false;

  virtual void init() {

    int countX = 2;
    int countY = 5;
    int countZ = 2;
    Vector3f extent = Vector3f(1, 1, 1);
    Vector3f start = Vector3f(-countX * extent(0), 0, -countZ * extent(2));
    for (unsigned int i = 0; i < countX; i++) {
      for (unsigned int j = 0; j < countY; j++) {
        for (unsigned int k = 0; k < countZ; k++) {
          Vector3f pos =
              Vector3f(start(0, 0) + i * extent(0) * 2.0, start(1) + j * extent(1) * 2.0,
                       start(2, 0) + k * extent.z() * 2.0);
          createUnitCube(pos, extent, 0.1f, 240.0f, 0.15f);
        }
      }
    }

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

    if (!pressingP) {
      if (context().getKey('P')) {
        pressingR = true;
        pause();
      }
    } else {
      if (!context().getKey('P')) {
        pressingR = false;
      }
    }

    if (context().getKey('T')) {
      for (auto element : points) {
        if (element->position(1, 0) > 4.5) {
          element->addForce(Vector3f(10, 10, 10));
        }
      }
      // std::cout << points[0]->position << std::endl;
    }

    if (context().getKey('G')) {
      for (auto element : points) {
        element->position(1, 0) += 3;
      }
      // std::cout << points[0]->position << std::endl;
    }
    PhysicsSystem::update();
  }
};

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position = Vector3f(3, 3, 3);
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
