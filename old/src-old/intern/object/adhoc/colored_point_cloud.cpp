#include "object/adhoc/colored_point_cloud.h"

using namespace nash;

ColoredPointCloud::ColoredPointCloud(const std::vector<Vector3f> &ps,
                                     const std::vector<Vector4u> &cs)
    : PointCloud(ps) {
  setShader(Shader::get(Path::getAbsolutePathTo("./shader/colored_point_cloud")));
  colors = MatrixXf(3, cs.size());
  for (int i = 0; i < cs.size(); i++) {
    colors.col(i) << cs[i].x() / 255.0f, cs[i].y() / 255.0f, cs[i].z() / 255.0f;
  }
}

void ColoredPointCloud::render() {
  shader->uploadAttr("color", colors);
  PointCloud::render();
}