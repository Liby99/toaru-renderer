#include <toaru/toaru.h>

using namespace toaru;

class CubePhysicsSystem : public PhysicsSystem {
public:
  bool pressingR = false, pressingP = false, pressingY = false;

  virtual void init() {

    int countX = 3;
    int countY = 10;
    int countZ = 8;
    Vector3f extent = Vector3f(1, 1, 1);
    Vector3f start = Vector3f(-countX * extent.x(), 0, -countZ * extent.z());

    Ks.push_back(std::make_unique<PhysicsMaterial>(3200000.0f, 0.499f));
    Ds.push_back(std::make_unique<PhysicsMaterial>(160000.0f, 150000.0f, false));

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

    PhysicsSystem::init();
  }

  virtual void update() {
    if (!pressingP) {
      if (context().getKey('P')) {
        pressingP = true;
        if (isPlaying) pause();
        else play();
      }
    } else {
      if (!context().getKey('P')) {
        pressingP = false;
      }
    }

    if (isPlaying) {
      for (auto & element : points) {
        if (element->position.y() > 4.0) {
          if (context().getDirectionKey(Context::Direction::Down))
            element->addForce(Vector3f(0, 0, 10000));
          if (context().getDirectionKey(Context::Direction::Up))
            element->addForce(Vector3f(0, 0, -10000));
          if (context().getDirectionKey(Context::Direction::Right))
            element->addForce(Vector3f(10000, 0, 0));
          if (context().getDirectionKey(Context::Direction::Left))
            element->addForce(Vector3f(-10000, 0, 0));
        }
      }
    }

    if (context().getKey('R')) {
      for (auto & element : points) {
        element->position.y() += 1;
      }
    }

    PhysicsSystem::update();
  }
};

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position = Vector3f(-1, 30, 29);
  ThirdPersonCamera cam;
  cam.target = Vector3f(-1, 0, -1);
  // cam.allowRotate = false;
  cam.moveSpeed = 10.0f;
  cam.scrollSpeed = 5.0f;
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
  floorHolder.transform.position << -1, -2, -1;
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
