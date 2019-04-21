#include "object/adhoc/sky_box.h"
#include "viewer/camera.h"

using namespace nash;

SkyBox::SkyBox(const CubeMap &cubeMap) : cubeMap(&cubeMap), Object() {
  setShader(Shader::get(Path::getAbsolutePathTo("./shader/sky_box")));

  indices = nanogui::MatrixXu(3, 12);
  indices.col(0) << 0, 1, 2;
  indices.col(1) << 2, 3, 0;
  indices.col(2) << 6, 5, 4;
  indices.col(3) << 7, 6, 4;
  indices.col(4) << 0, 3, 4;
  indices.col(5) << 4, 3, 7;
  indices.col(6) << 2, 1, 5;
  indices.col(7) << 2, 5, 6;
  indices.col(8) << 5, 1, 0;
  indices.col(9) << 0, 4, 5;
  indices.col(10) << 6, 3, 2;
  indices.col(11) << 3, 6, 7;

  positions = nanogui::MatrixXf(3, 8);
  positions.col(0) << -50, 50, 50;
  positions.col(1) << -50, -50, 50;
  positions.col(2) << 50, -50, 50;
  positions.col(3) << 50, 50, 50;
  positions.col(4) << -50, 50, -50;
  positions.col(5) << -50, -50, -50;
  positions.col(6) << 50, -50, -50;
  positions.col(7) << 50, 50, -50;
}

void SkyBox::update() { transform.position = context->getScene().getCamera().position; }

void SkyBox::render() {
  // glDisable(GL_DEPTH_TEST);
  shader->uploadIndices(indices);
  shader->uploadAttr("position", positions);
  shader->setUniform("cubeMap", 10);
  cubeMap->bind(10);
  shader->drawIndexed(GL_TRIANGLES, 0, indices.cols());
  // glEnable(GL_DEPTH_TEST);
}