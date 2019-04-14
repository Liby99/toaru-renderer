#include <chrono>
#include <nash/nash.h>

using namespace nash;

class ColoredMesh : public Mesh {
public:
  ColoredMesh(const AssimpMesh &assimpMesh, const MatrixXf &color) : Mesh(), color(color) {
    positions = assimpMesh.getPositions();
    normals = assimpMesh.getNormals();
    indices = assimpMesh.getIndices();
    setShader(Shader::get(Path::getAbsolutePathTo("./shader/plain_color")));
  }

  virtual void render() {
    shader->uploadAttr("color", color);
    Mesh::render();
  }

private:
  MatrixXf color;
};

int main(int argc, char *argv[]) {
  Nash::init(argc, argv);

  AssimpObject teapot(Path::getAbsolutePathTo("./model/teapot.obj"));
  const AssimpMesh &mesh = *(teapot.getMeshes()[0]);
  std::cerr << "Start" << std::endl;
  auto start = std::chrono::system_clock::now();
  BoxTreeMesh teapotBTMesh(mesh);

  std::vector<Vector3f> samples(100);
  Sampler::sampleSphere(samples);

  std::cerr << "Iteration:" << mesh.getPositions().cols() << std::endl;

  MatrixXf color(3, mesh.getPositions().cols());

#pragma omp parallel for
  for (int i = 0; i < mesh.getPositions().cols(); i++) {
    int count = 0;
    Vector3f currPos = mesh.getPositions().col(i);
    for (int j = 0; j < samples.size(); j++) {
      Ray ray(currPos, samples[j]);
      ray.step();
      Intersection intersection1(ray);
      bool result = teapotBTMesh.intersect(ray, intersection1);
      if (intersection1.hit) {
        count++;
      }
    }
    float percent = (float)count / samples.size();
    color.col(i) = Vector3f(1 - percent, 1 - percent, 1 - percent);
  }

  auto end = std::chrono::system_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cerr << elapsed.count() << '\n';

  Scene scene;
  ThirdPersonCamera camCtrl;
  scene.getCamera().setController(camCtrl);

  ColoredMesh coloredMesh(*teapot.getMeshes()[0], color);
  scene.addObject(coloredMesh);

  Viewer viewer(1280, 720, "Intersection test", scene);
  viewer.start();

  Nash::shutdown();
}
