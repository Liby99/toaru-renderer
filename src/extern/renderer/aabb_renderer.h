#ifndef TOARU_AABB_RENDERER_H
#define TOARU_AABB_RENDERER_H

#include "core/renderer.h"
#include "physics/physics_system.h"
#include "material/lambertian.h"

namespace toaru {
  class AABBRenderer : public Renderer {
  public:
    unique_ptr<Lambertian> material;
    MatrixXu indices;

    AABBRenderer();
    virtual void init();
    virtual void render();

  protected:
    void renderAABBTree(const AABBTree &tree);
    void renderAABBTreeNode(const AABBTreeNode &node);
    void renderAABB(const AABB &aabb);

  private:
    void initializeIndices();
  };
}

#endif