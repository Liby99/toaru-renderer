#include <toaru/toaru.h>
#include <iostream>

using namespace toaru;

class AABBRenderMode : public Component {
public:
  bool pressingPlus = false, pressingMinus = false, pressingN = false;
  virtual void update() {
    AABBRenderer &renderer = object().getComponent<AABBRenderer>("aabb-renderer");

    if (!pressingPlus) {
      if (context().getKey(']')) {
        pressingPlus = true;
        renderer.depth++;
      }
    } else {
      if (!context().getKey(']')) {
        pressingPlus = false;
      }
    }

    if (!pressingMinus) {
      if (context().getKey('[')) {
        pressingMinus = true;
        renderer.depth--;
      }
    } else {
      if (!context().getKey('[')) {
        pressingMinus = false;
      }
    }

    if (!pressingN) {
      if (context().getKey('n')) {
        pressingN = true;
        if (renderer.isEnabled()) {
          renderer.setEnabled(false);
        } else {
          renderer.setEnabled(true);
        }
      }
    } else {
      if (!context().getKey('n')) {
        pressingN = false;
      }
    }
  }
};

class JellySystemRenderSwitch : public Component {
public:
  bool pressingM = false;
  virtual void update() {
    if (!pressingM) {
      if (context().getKey('M')) {
        pressingM = true;
        Lambertian &material = object().getComponent<Lambertian>("material");
        PhysicsSystemRenderer &renderer = object().getComponent<PhysicsSystemRenderer>("renderer");
        if (renderer.renderMode == Renderer::Mode::FACE) {
          renderer.renderMode = Renderer::Mode::LINE;
          material.ambientColor = Vector3f(1, 1, 1);
        } else {
          renderer.renderMode = Renderer::Mode::FACE;
          material.ambientColor = Vector3f(0.1, 0.1, 0.1);
        }
      }
    } else {
      if (!context().getKey('M')) {
        pressingM = false;
      }
    }
  }
};

class JellySystem : public PhysicsSystem {
public:
  bool pressingR = false, pressingP = false, pressingB = false;
  std::unique_ptr<MaterialTensor> k, d;
  std::unique_ptr<PhysicsMaterial> mat;

  JellySystem() : PhysicsSystem() {
    isPlaying = false;
  }

  virtual void init() {
    k = make_unique<MaterialTensor>(2000000.0f, 0.1f);
    d = make_unique<MaterialTensor>(1000.0f, 1000.0f, false);
    mat = make_unique<PhysicsMaterial>(1000.f, 0.001f, 0.1f, *k, *d);
    createBox(*mat, Vector3f(0, 5, 0), Vector3f(2, 5, 4), Vector3u(4, 10, 8));
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

    if (context().getKey('R')) {
      for (auto & element : points) {
        element->position.y() += 5;
      }
    }

    if (!pressingB) {
      if (context().getKey('B')) {
        pressingB = true;
        buildAABBTrees();
      }
    } else {
      if (!context().getKey('B')) {
        pressingB = false;
      }
    }

    PhysicsSystem::update();
  }
};

int main(int argc, char * argv[]) {
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
  JellySystem sys;
  Lambertian lambMat;
  PhysicsSystemRenderer renderer;
  AABBRenderer aabbRenderer;
  AABBRenderMode aabbSwitcher;
  JellySystemRenderSwitch switcher;
  physicsHolder.addComponent("system", sys);
  physicsHolder.addComponent("material", lambMat);
  physicsHolder.addComponent("renderer", renderer);
  physicsHolder.addComponent("switcher", switcher);
  physicsHolder.addComponent("aabb-renderer", aabbRenderer);
  physicsHolder.addComponent("aabb-switcher", aabbSwitcher);
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