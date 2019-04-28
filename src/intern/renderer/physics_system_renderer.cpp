#include "renderer/physics_system_renderer.h"

using namespace toaru;

void PhysicsSystemRenderer::refresh() {
  cached = false;
}

void PhysicsSystemRenderer::render() {
  if (object().hasComponent("system")) {
    PhysicsSystem &sys = object().getComponent<PhysicsSystem>("system");
    if (object().hasComponent("material")) {
      Material &mat = object().getComponent<Material>("material");

      // Bind the shader
      Shader &shader = mat.getShader();
      shader.bind();

      // Get the transform matrices
      shader.setUniform("model", object().transform.getTransform());
      shader.setUniform("viewProj", context().getMainCameraViewProj());

      // If not cached, cache indices data (this could be done only once)
      if (!cached) {
        cached = true;
        cacheIndicesData(sys);
      }

      // Load the positions and normals (this has to be done for each frame)
      loadPosAndNorm(sys);

      // Pass to shader
      shader.uploadIndices(indices);
      shader.uploadAttr("position", positions);
      shader.uploadAttr("normal", normals);

      // Material
      mat.prerender();

      // Set render mode
      if (renderMode == Mode::LINE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      } else if (renderMode == Mode::POINT) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      }

      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      shader.drawIndexed(GL_TRIANGLES, 0, indices.cols());

      // Set back to mode fill
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
  }
}

void PhysicsSystemRenderer::cacheIndicesData(const PhysicsSystem &sys) {
  // First go through indices
  int maxFaceAmount = sys.faces.size();
  int faceAmount = 0, vertAmount = 0;
  indices = MatrixXu(3, maxFaceAmount);

  // Go through
  for (int i = 0; i < sys.faces.size(); i++) {
    if (renderAllFaces || !sys.faces[i]->internal) {

      // Add a face
      faces.push_back(sys.faces[i].get());
      indices.col(faceAmount) << vertAmount, vertAmount + 1, vertAmount + 2;
      faceAmount += 1;
      vertAmount += 3;
    }
  }

  // Resize the positions and normals
  indices.conservativeResize(3, faceAmount);
}

void PhysicsSystemRenderer::loadPosAndNorm(const PhysicsSystem &sys) {
  // First go through indices
  positions = MatrixXf(3, faces.size() * 3);
  normals = MatrixXf(3, faces.size() * 3);

  // Go through
  int vertAmount = 0;
  for (auto &face : faces) {

    // First get all points
    auto p0 = face->getPoint(0).position;
    auto p1 = face->getPoint(1).position;
    auto p2 = face->getPoint(2).position;

    // Then get the normal
    Vector3f n = face->getNormal();

    // Add three normals
    normals.col(vertAmount) = n;
    normals.col(vertAmount + 1) = n;
    normals.col(vertAmount + 2) = n;

    // Add three positions
    positions.col(vertAmount) = p0;
    positions.col(vertAmount + 1) = p1;
    positions.col(vertAmount + 2) = p2;

    // Increment the vertices
    vertAmount += 3;
  }
}