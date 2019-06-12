#include "renderer/point_cloud_renderer.h"

using namespace toaru;

const float PointCloudRenderer::DEFAULT_POINT_SIZE = 3.0f;

PointCloudRenderer::PointCloudRenderer(const std::string &name) :
  PointCloudRenderer(name, DEFAULT_POINT_SIZE) {}

PointCloudRenderer::PointCloudRenderer(const std::string &name, float pointSize) :
  pointCloudName(name),
  shader(Shader::get(Path::getAbsolutePathTo("./shader/colored_point_cloud"))),
  pointSize(pointSize),
  Renderer() {}

void PointCloudRenderer::init() {
  shader.init();
}

void PointCloudRenderer::render() {
  if (object().hasComponent(pointCloudName)) {
    PointCloud &pc = object().getComponent<PointCloud>(pointCloudName);

    // Start rendering
    shader.bind();

    // Get the transform matrices
    shader.setUniform("model", object().transform.getTransform());
    shader.setUniform("viewPersp", context().getMainCameraViewProj());

    // Keep the old point size
    GLfloat oldSize;
    glGetFloatv(GL_POINT_SIZE, &oldSize);
    glPointSize(pointSize);

    // Then upload all the properties of the mesh
    shader.uploadIndices(pc.indices);
    shader.uploadAttr("point", pc.positions);
    shader.uploadAttr("color", pc.colors);

    // Draw the points
    shader.drawIndexed(GL_POINTS, 0, pc.numPositions());

    // Restore the old point size
    glPointSize(oldSize);
  }
}