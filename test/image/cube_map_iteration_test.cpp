#include <nash/nash.h>

using namespace nash;

const int GAP = 5;

int main(int argc, char *argv[]) {
  Nash::init(argc, argv);

  Scene scene;
  ThirdPersonCamera camCtrl;
  scene.getCamera().setController(camCtrl);

  Image negx(Path::getAbsolutePathTo("./image/cubemap/room/negx.jpg"));
  Image posx(Path::getAbsolutePathTo("./image/cubemap/room/posx.jpg"));
  Image negy(Path::getAbsolutePathTo("./image/cubemap/room/negy.jpg"));
  Image posy(Path::getAbsolutePathTo("./image/cubemap/room/posy.jpg"));
  Image negz(Path::getAbsolutePathTo("./image/cubemap/room/negz.jpg"));
  Image posz(Path::getAbsolutePathTo("./image/cubemap/room/posz.jpg"));
  CubeMap cubeMap(posy, negy, negx, posx, posz, negz);

  std::vector<Vector3f> positions;
  std::vector<Vector4u> colors;

  for (CubeMap::Face face : CubeMap::FACES) {
    const Image &img = cubeMap.getImage(face);
    for (int i = 0; i < img.width; i += GAP) {
      for (int j = 0; j < img.height; j += GAP) {
        Vector4u color = img.getColor(i, j);
        Vector3f dir = cubeMap.getDirectionTo(face, i, j);
        positions.push_back(dir);
        colors.push_back(color);
      }
    }

    Vector3f centralDir = cubeMap.getCentralDirection(face);
    Vector4u color = cubeMap.getColor(centralDir);
    positions.push_back(centralDir * 1.5);
    colors.push_back(color);
  }

  ColoredPointCloud pc(positions, colors);
  scene.addObject(pc);

  SkyBox skybox(cubeMap);
  scene.addObject(skybox);

  Viewer viewer(1280, 720, "Skybox Test", scene);
  viewer.start();

  Nash::shutdown();
}