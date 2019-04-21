#include "renderer/mesh_renderer.h"

using namespace toaru;

const std::string MeshRenderer::DEFAULT_MESH_NAME = "mesh";
const std::string MeshRenderer::DEFAULT_MATERIAL_NAME = "material";

MeshRenderer::MeshRenderer() : MeshRenderer(DEFAULT_MESH_NAME, DEFAULT_MATERIAL_NAME) {}

MeshRenderer::MeshRenderer(const std::string &meshName, const std::string &matName)
  : meshName(meshName), matName(matName), Renderer() {}

void MeshRenderer::render() {
  if (object().hasComponent(meshName)) {
    Mesh &mesh = object().getComponent<Mesh>(meshName);
    if (object().hasComponent(matName)) {
      Material &mat = object().getComponent<Material>(matName);
      // TODO
    }
  }
}