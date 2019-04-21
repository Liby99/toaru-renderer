#include "object/adhoc/point_cloud.h"

using namespace nash;

const int PointCloud::POINT_SIZE = 3;

PointCloud::PointCloud(const std::vector<Vector3f> &ps) : Object() {
  setShader(Shader::get(Path::getAbsolutePathTo("./shader/point_cloud")));

  points = MatrixXf(3, ps.size());
  indices = MatrixXu(1, ps.size());
  for (int i = 0; i < ps.size(); i++) {
    points.col(i) = ps[i];
    indices.col(i) << i;
  }
}

void PointCloud::render() {
  GLfloat oldSize;
  glGetFloatv(GL_POINT_SIZE, &oldSize);
  glPointSize(POINT_SIZE);
  shader->uploadIndices(indices);
  shader->uploadAttr("point", points);
  shader->drawIndexed(GL_POINTS, 0, points.size());
  glPointSize(oldSize);
}