#include "renderer/mesh_renderer.h"

using namespace toaru;

const std::string MeshRenderer::DEFAULT_MESH_NAME = "mesh";
const std::string MeshRenderer::DEFAULT_MATERIAL_NAME = "material";

MeshRenderer::MeshRenderer() : MeshRenderer(DEFAULT_MESH_NAME, DEFAULT_MATERIAL_NAME) {}

MeshRenderer::MeshRenderer(const std::string &meshName, const std::string &matName)
    : meshName(meshName), matName(matName), Renderer() {}

void MeshRenderer::setMode(MeshRenderer::Mode mode) {
  renderMode = mode;
}

MeshRenderer::Mode MeshRenderer::getMode() {
  return renderMode;
}

void MeshRenderer::render() {

  // First get the mesh and the materials
  if (object().hasComponent(meshName)) {
    Mesh &mesh = object().getComponent<Mesh>(meshName);
    if (object().hasComponent(matName)) {
      Material &mat = object().getComponent<Material>(matName);

      // First bind the shader
      Shader &shader = mat.getShader();
      shader.bind();

      // Get the transform matrices
      shader.setUniform("model", object().transform.getTransform());
      shader.setUniform("viewProj", context().getMainCameraViewProj());

      // Then upload all the properties of the mesh
      shader.uploadIndices(mesh.indices);
      shader.uploadAttr("position", mesh.positions);
      shader.uploadAttr("normal", mesh.normals);
      // shader.uploadAttr("texCoord", mesh.texCoords);

      // Pass material specific prooperties
      mat.prerender();

      // Set render mode
      if (renderMode == Mode::LINE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      } else if (renderMode == Mode::POINT) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
      }

      // Draw the triangles
      shader.drawIndexed(GL_TRIANGLES, 0, mesh.numFaces());

      // Set back to mode fill
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
  }
}