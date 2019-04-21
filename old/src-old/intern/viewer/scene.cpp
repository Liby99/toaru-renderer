#include "viewer/scene.h"
#include "object/object.h"
#include "viewer/camera.h"

using namespace nash;

Scene::Scene() : camera(new Camera()), root(new Object()) {
  // Do nothing
}

Scene::~Scene() {
  delete camera;
  delete root;
}

void Scene::setSize(int width, int height) { camera->setSize(width, height); }

Camera &Scene::getCamera() { return *camera; }

Object &Scene::getRoot() { return *root; }

void Scene::addObject(Object &object) { object.setParent(*root); }

void Scene::start(Context &context) {
  context.setScene(*this);
  camera->start(context);
  root->startWrapper(context);
}

void Scene::update(Context &context) {
  context.setScene(*this);
  Matrix4f base = Matrix4f::Identity();
  camera->update(context);
  root->updateWrapper(context, base);
}

void Scene::render(Context &context) {
  context.setScene(*this);
  Matrix4f viewPersp = camera->getViewPerspective();
  root->renderWrapper(context, viewPersp);
}
