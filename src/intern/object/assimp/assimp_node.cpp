#include "object/assimp/assimp_node.h"

using namespace nash;

AssimpNode::AssimpNode(const std::string &name) : Object(), name(name) {}

const std::string &AssimpNode::getName() { return name; }

void AssimpNode::addMesh(AssimpMesh &mesh) {
  meshes[mesh.getName()] = &mesh;
  mesh.setParent(*this);
}

bool AssimpNode::hasMesh(const std::string &name) { return meshes.count(name) > 0; }

AssimpMesh &AssimpNode::getMesh(const std::string &name) { return *(meshes[name]); }

void AssimpNode::addChildNode(AssimpNode &node) {
  nodes[node.getName()] = &node;
  node.setParent(*this);
}

bool AssimpNode::hasChildNode(const std::string &name) { return nodes.count(name) > 0; }

AssimpNode &AssimpNode::getChildNode(const std::string &name) { return *(nodes[name]); }
