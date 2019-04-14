#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "image/cube_map.h"
#include "object/object.h"

namespace nash {
  class SkyBox : public Object {
  public:
    SkyBox(const CubeMap &cubeMap);
    virtual void update();
    virtual void render();

  private:
    const CubeMap *cubeMap;
    MatrixXf positions;
    MatrixXu indices;
  };
}

#endif