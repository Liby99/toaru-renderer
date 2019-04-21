#include <nash/nash.h>

using namespace nash;

class TexturedPlane : public Plane {
public:
  TexturedPlane(const std::string &imageName, int texPos) : Plane(), texPos(texPos) {
    texImage = new Image(Path::getAbsolutePathTo(imageName));
    texture = new Texture2D(*texImage);
    setShader(Shader::get(Path::getAbsolutePathTo("./shader/plain_texture")));
  }

  ~TexturedPlane() {
    delete texture;
    delete texImage;
  }

  virtual void render() {
    texture->bind(texPos);
    getShader().setUniform("tex", texPos);
    Plane::render();
  }

private:
  Image *texImage;
  Texture2D *texture;
  int texPos;
};

int main(int argc, char *argv[]) {
  Nash::init(argc, argv);

  Scene scene;
  ThirdPersonCamera camCtrl;
  camCtrl.incline = PI / 4;
  scene.getCamera().setController(camCtrl);

  TexturedPlane plane1("./image/bridge3_posx.jpg", 1);
  plane1.transform.position << -1, 0, 0;
  scene.addObject(plane1);

  TexturedPlane plane2("./image/gogo.png", 2);
  // plane2 will stay at origin
  scene.addObject(plane2);

  TexturedPlane plane3("./image/head.bmp", 3);
  plane3.transform.position << 1, 0, 0;
  scene.addObject(plane3);

  Viewer viewer(1280, 720, "Plane test", scene);
  viewer.start();

  Nash::shutdown();
}
