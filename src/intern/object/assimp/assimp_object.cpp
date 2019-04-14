#include "object/assimp/assimp_object.h"

using namespace nash;

AssimpObject::AssimpObject(const std::string &filename) : Object() {

  // Setup importer and load
  Assimp::Importer importer;
  const aiScene *assimpScene = importer.ReadFile(filename, aiProcess_Triangulate);

  // If the import fail, throw error
  if (assimpScene == nullptr) {
    throw std::runtime_error("Cannot load file " + filename);
  }

  // Process the scene
  processScene(assimpScene);

  // Note the importer will be auto-recycled here. All the assimp related memory
  // will be freed
}

AssimpObject::~AssimpObject() { delete root; }

AssimpNode &AssimpObject::getRootNode() { return *root; }

void AssimpObject::setShader(Shader &shader) {
  Object::setShader(shader);
  for (int i = 0; i < meshes.size(); i++) {
    meshes[i]->setShader(shader);
  }
}

void AssimpObject::processScene(const aiScene *scene) {

  // First generate `AssimpMesh`es
  meshes = std::vector<AssimpMesh *>(scene->mNumMeshes);
  for (int i = 0; i < scene->mNumMeshes; i++) {
    meshes[i] = new AssimpMesh(scene->mMeshes[i]);
  }

  // Then recursively parse the root node, and add the root node as a child
  root = parseAssimpNode(scene->mRootNode);
  root->setParent(*this);
}

AssimpNode *AssimpObject::parseAssimpNode(const aiNode *assimpNode) {

  // First generate an empty assimp node
  AssimpNode *node = new AssimpNode(std::string(assimpNode->mName.C_Str()));

  // Then parse the local transform in the node
  aiMatrix4x4 trsf = assimpNode->mTransformation;
  Vector3f col1 = Vector3f(trsf.a1, trsf.b1, trsf.c1);
  Vector3f col2 = Vector3f(trsf.a2, trsf.b2, trsf.c2);
  Vector3f col3 = Vector3f(trsf.a3, trsf.b3, trsf.c3);
  node->transform.position << trsf.a4, trsf.b4, trsf.c4;
  node->transform.scale << col1.norm(), col2.norm(), col3.norm();
  Vector3f nc1 = col1.normalized(), nc2 = col2.normalized(), nc3 = col3.normalized();
  Matrix3f rot;
  rot << nc1.x(), nc2.x(), nc3.x(), nc1.y(), nc2.y(), nc3.y(), nc1.z(), nc2.z(), nc3.z();
  node->transform.rotation = rot;

  // Then put the meshes into the children of this node
  for (int i = 0; i < assimpNode->mNumMeshes; i++) {
    node->addMesh(*(meshes[assimpNode->mMeshes[i]]));
  }

  // Finally recursively generate child nodes
  for (int i = 0; i < assimpNode->mNumChildren; i++) {
    AssimpNode *child = parseAssimpNode(assimpNode->mChildren[i]);
    node->addChildNode(*child);
  }

  // Return generated node
  return node;
}

const std::vector<AssimpMesh *> &AssimpObject::getMeshes() const { return meshes; }
