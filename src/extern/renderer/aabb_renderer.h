#ifndef TOARU_AABB_RENDERER_H
#define TOARU_AABB_RENDERER_H

#include "core/renderer.h"
#include "material/lambertian.h"
#include "physics/physics_system.h"

namespace toaru {
  class AABBRenderer : public Renderer {
  public:
    unique_ptr<Lambertian> material;
    MatrixXu indices;
    int depth; // Depth < 0 means no depth limit

    AABBRenderer();
    virtual void init();
    virtual void render();

  protected:
    void renderAABBTree(const AABBTree &tree);
    void renderAABBTreeNode(const AABBTreeNode &node, int depth = 0);
    void renderAABB(const AABB &aabb);

  private:
    void initializeIndices();
  };
}

#endif