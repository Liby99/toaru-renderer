#ifndef ASSIMP_NODE_H
#define ASSIMP_NODE_H

#include "object/assimp/assimp_mesh.h"
#include "object/object.h"
#include <assimp/scene.h>
#include <map>

namespace nash {
  class AssimpNode : public Object {
  public:
    AssimpNode(const std::string &name);
    const std::string &getName();

    void addMesh(AssimpMesh &mesh);
    bool hasMesh(const std::string &name);
    AssimpMesh &getMesh(const std::string &name);

    void addChildNode(AssimpNode &node);
    bool hasChildNode(const std::string &name);
    AssimpNode &getChildNode(const std::string &name);

  private:
    std::string name;
    std::map<std::string, AssimpMesh *> meshes;
    std::map<std::string, AssimpNode *> nodes;
  };
}

#endif
