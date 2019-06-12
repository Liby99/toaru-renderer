#ifndef MPM_SYSTEM_H
#define MPM_SYSTEM_H

#include "mesh/point_cloud.h"
#include "grid.h"

namespace toaru::mpm {
  class System : public PointCloud {
  public:
    Grid &grid;

    System(Grid &grid);

    virtual void init();
    virtual void update();

  protected:
    void fill();
  };
}

#endif