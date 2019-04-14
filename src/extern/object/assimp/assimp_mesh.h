#ifndef ASSIMP_MESH_H
#define ASSIMP_MESH_H

#include "object/mesh.h"
#include <assimp/scene.h>

namespace nash {
  class AssimpMesh : public Mesh {
  public:
    AssimpMesh(const aiMesh *mesh);
    const std::string &getName();

  private:
    std::string name;
    void loadName(const aiMesh *mesh);
    void processMesh(const aiMesh *mesh);
  };
}

#endif
