#include "renderer/physics_system_renderer.h"

using namespace toaru;

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

      // First go through indices
      int maxFaceAmount = sys.faces.size();
      int faceAmount = 0, vertAmount = 0;
      MatrixXu indices(3, maxFaceAmount);
      MatrixXf positions(3, maxFaceAmount * 3);
      MatrixXf normals(3, maxFaceAmount * 3);

      // Go through
      for (int i = 0; i < sys.faces.size(); i++) {

        // First get all points
        auto p1 = sys.faces[i]->points[0];
        auto p2 = sys.faces[i]->points[1];
        auto p3 = sys.faces[i]->points[2];

        // Then get the normal
        Vector3f n = sys.faces[i]->normal.normalized();

        // Add a face
        indices.col(faceAmount) << vertAmount, vertAmount + 1, vertAmount + 2;
        faceAmount += 1;

        // Add three normals
        normals.col(vertAmount) = n;
        normals.col(vertAmount + 1) = n;
        normals.col(vertAmount + 2) = n;

        // Add three positions
        positions.col(vertAmount) = p1->position;
        positions.col(vertAmount + 1) = p2->position;
        positions.col(vertAmount + 2) = p3->position;

        // Increment the vertices
        vertAmount += 3;
      }

      // Resize the positions and normals
      indices.conservativeResize(3, faceAmount);
      positions.conservativeResize(3, vertAmount);
      normals.conservativeResize(3, vertAmount);

      // std::cout << indices << std::endl;
      // std::cout << positions << std::endl;
      // std::cout << normals << std::endl;

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
