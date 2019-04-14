#ifndef ASSIMP_OBJECT_H
#define ASSIMP_OBJECT_H

#include "object/assimp/assimp_mesh.h"
#include "object/assimp/assimp_node.h"
#include "object/object.h"
#include "utility/path.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace nash {
  class AssimpObject : public Object {
  public:
    AssimpObject(const std::string &filename);
    ~AssimpObject();
    AssimpNode &getRootNode();

    virtual void setShader(Shader &shader);

    const std::vector<AssimpMesh *> &getMeshes() const;

  private:
    AssimpNode *root;
    std::vector<AssimpMesh *> meshes;
    void processScene(const aiScene *scene);
    AssimpNode *parseAssimpNode(const aiNode *assimpNode);
  };
}

#endif
