#include "object/assimp/assimp_mesh.h"

using namespace nash;

AssimpMesh::AssimpMesh(const aiMesh *mesh) : Mesh() {
  loadName(mesh);
  processMesh(mesh);
}

const std::string &AssimpMesh::getName() { return name; }

void AssimpMesh::loadName(const aiMesh *mesh) { name = std::string(mesh->mName.C_Str()); }

void AssimpMesh::processMesh(const aiMesh *mesh) {

  // First load the indices to matrix
  indices = MatrixXu(3, mesh->mNumFaces);
  for (int i = 0; i < mesh->mNumFaces; i++) {
    aiFace f = mesh->mFaces[i];
    indices.col(i) << f.mIndices[0], f.mIndices[1], f.mIndices[2];
  }

  // Then load the positions
  positions = MatrixXf(3, mesh->mNumVertices);
  normals = MatrixXf(3, mesh->mNumVertices);
  for (int i = 0; i < mesh->mNumVertices; i++) {
    aiVector3D v = mesh->mVertices[i];
    aiVector3D n = mesh->mNormals[i];
    positions.col(i) << v.x, v.y, v.z;
    normals.col(i) << n.x, n.y, n.z;
  }
}
