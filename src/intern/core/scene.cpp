#include "core/scene.h"

using namespace toaru;

Scene::Scene() {}

void Scene::update(Context &context) {
  Matrix4f mat = Matrix4f::Identity();
  root.update(context, mat);
}

void Scene::render(Context &context) {
  root.render(context);
}