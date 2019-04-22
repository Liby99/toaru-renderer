#include <toaru/toaru.h>

using namespace toaru;

class CubePhysicsSystem : public PhysicsSystem {
public:

  bool pressingR = false, pressingT = false;

  virtual void init() {
    auto A = std::make_shared<Point>(Vector3f(-1, 1, -1));
    auto B = std::make_shared<Point>(Vector3f(-1, 1, 1));
    auto C = std::make_shared<Point>(Vector3f(1, 1, 1));
    auto D = std::make_shared<Point>(Vector3f(1, 1, -1));
    auto E = std::make_shared<Point>(Vector3f(-1, -1, 1));
    auto F = std::make_shared<Point>(Vector3f(1, -1, 1));
    auto G = std::make_shared<Point>(Vector3f(1, -1, -1));
    auto H = std::make_shared<Point>(Vector3f(-1, -1, -1));

    auto T1 = std::make_shared<Tetrahedron>(1.f, 0.5f, 0.3f, A, B, C, E);
    auto T2 = std::make_shared<Tetrahedron>(1.f, 0.5f, 0.3f, A, C, D, G);
    auto T3 = std::make_shared<Tetrahedron>(1.f, 0.5f, 0.3f, A, H, E, G);
    auto T4 = std::make_shared<Tetrahedron>(1.f, 0.5f, 0.3f, C, E, F, G);
    auto T5 = std::make_shared<Tetrahedron>(1.f, 0.5f, 0.3f, A, C, G, E);

    tetrahedrons.insert(tetrahedrons.end(), {T1, T2, T3, T4, T5});
    for (auto element : tetrahedrons) {
      element->initRestState();
    }
    points.insert(points.end(), {A, B, C, D, E, F, G, H});
    faces.insert(faces.end(), Face::faces.begin(), Face::faces.end());
  }

  virtual void update() {
    if (!pressingR) {
      if (context().getKey('R')) {
        pressingR = true;
        stepOnce();
      }
    } else {
      if (!context().getKey('R')) {
        pressingR = false;
      }
    }
    if (!pressingT) {
      if (context().getKey('T')) {
        pressingT = true;
        points[0]->addForce(Vector3f(0, -0.2, 0));
      }
    } else {
      if (!context().getKey('T')) {
        pressingT = false;
      }
    }
  }
};

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position = Vector3f(2, 2, 2);
  TwoPointCamera cam;
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
