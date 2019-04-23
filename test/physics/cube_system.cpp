#include <toaru/toaru.h>

using namespace toaru;

class CubePhysicsSystem : public PhysicsSystem {
public:
  bool pressingR = false, pressingT = false;

  virtual void init() {

    int countX = 2;
    int countY = 10;
    int countZ = 4;
    Vector3f extent = Vector3f(1, 1, 1);
    Vector3f start = Vector3f(-countX * extent(0, 0), 0, -countZ * extent(2, 0));
    for (unsigned int i = 0; i < countX; i++) {
      for (unsigned int j = 0; j < countY; j++) {
        for (unsigned int k = 0; k < countZ; k++) {
          Vector3f pos =
              Vector3f(start(0, 0) + i * extent(0, 0) * 2.0, start(1, 0) + j * extent(1, 0) * 2.0,
                       start(2, 0) + k * extent(2, 0) * 2.0);
          createUnitCube(pos, extent, 0.1f, 100.0f, 0.1f);
        }
      }
    }

    // createUnitCube(Vector3f(-3,0,0), Vector3f(1, 1, 1), 1.f, 180.0f, 0.4f);
    // createUnitCube(Vector3f(-1,0,0), Vector3f(1, 1, 1), 1.f, 180.0f, 0.4f);
    // createUnitCube(Vector3f(1,0,0), Vector3f(1, 1, 1), 1.f, 180.0f, 0.4f);
    // createUnitCube(Vector3f(3,0,0), Vector3f(1, 1, 1), 1.f, 180.0f, 0.4f);

    for (auto element : tetrahedrons) {
      element->initRestState();
      for (auto f : element->faces) {
        faces.push_back(f);
      }
    }
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
    if (context().getKey('T')) {
      for (auto element : points) {
        if (element->position(1,0) > 6.0)
          element->addForce(Vector3f(10, 10, 10));
      }
      std::cout << points[0]->position << std::endl;
    }

    if (context().getKey('G')) {
      for (auto element : points) {
        element->position(1, 0) += 10;
      }
      std::cout << points[0]->position << std::endl;
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
  cam.target = Vector3f(0, 0, 0);
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

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}
