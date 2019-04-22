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

    auto T1 = std::make_shared<Tetrahedron>(1.f, 18.f, 0.01f, A, B, C, E);
    auto T2 = std::make_shared<Tetrahedron>(1.f, 18.f, 0.01f, A, C, D, G);
    auto T3 = std::make_shared<Tetrahedron>(1.f, 18.f, 0.01f, A, H, E, G);
    auto T4 = std::make_shared<Tetrahedron>(1.f, 18.f, 0.01f, C, E, F, G);
    auto T5 = std::make_shared<Tetrahedron>(1.f, 18.f, 0.01f, A, C, G, E);

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
        play();
      }
    } else {
      if (!context().getKey('R')) {
        pressingR = false;
      }
    }
    if (!pressingT) {
      if (context().getKey('T')) {
        //pressingT = true;
        points[0]->position(1,0) += 0.1;
        std::cout << points[0]->position << std::endl;
      }
    } else {
      if (!context().getKey('T')) {
        pressingT = false;
      }
    }
    PhysicsSystem::update();
  }
};

int main(int argc, char *argv[]) {
  toaru::init(argc, argv);

  Scene scene;

  Entity camHolder;
  camHolder.transform.position = Vector3f(3, 3, 3);
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
