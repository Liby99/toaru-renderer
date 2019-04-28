#include <toaru/toaru.h>

using namespace toaru;

class TetraSystem : public PhysicsSystem {
public:
  bool pressingR = false, pressingP = false, pressingY = false;
  std::unique_ptr<MaterialTensor> k, d;
  std::unique_ptr<PhysicsMaterial> mat;
  int tetraObj;

  TetraSystem() : PhysicsSystem() {
    isPlaying = false;
  }

  virtual void init() {
    k = make_unique<MaterialTensor>(2000000.0f, 0.1f);
    d = make_unique<MaterialTensor>(1000.0f, 1000.0f, false);
    mat = make_unique<PhysicsMaterial>(1000.f, 0.001f, 0.1f, *k, *d);
    tetraObj = addObject(*mat);

    int p0 = addPoint(Vector3f(0, 3, 0));
    int p1 = addPoint(Vector3f(0, 2, 1));
    int p2 = addPoint(Vector3f(-1, 2, -1));
    int p3 = addPoint(Vector3f(1, 2, -1));

    addTetrahedron(tetraObj, p0, p2, p1, p3);

    PhysicsSystem::init();
  }

  virtual void update() {
    if (!pressingP) {
      if (context().getKey('P')) {
        pressingP = true;
        if (isPlaying)
          pause();
        else
          play();
      }
    } else {
      if (!context().getKey('P')) {
        pressingP = false;
      }
    }

    if (context().getKey('R')) {
      for (auto &element : points) {
        element->position.y() += 2;
      }
    }

    PhysicsSystem::update();
  }
};

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position = Vector3f(10, 10, 10);
  ThirdPersonCamera cam;
  cam.moveSpeed = 10.0f;
  cam.scrollSpeed = 5.0f;
  camHolder.addComponent("camera", cam);
  scene.root.addChild(camHolder);

  Entity physicsHolder;
  TetraSystem sys;
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
  floorMat.diffuseColor << 0.5, 0.5, 0.5;
  floorHolder.addComponent("mesh", plane);
  floorHolder.addComponent("material", floorMat);
  floorHolder.addComponent("renderer", meshRenderer);
  scene.root.addChild(floorHolder);

  Viewer viewer(scene);
  viewer.start();

  toaru::exit();
}