#ifndef MPM_GRID_H
#define MPM_GRID_H

#include <tuple>
#include <vector>

#include "cell.h"
#include "particle.h"
#include "utility/math.h"

namespace toaru::mpm {
  class Grid {
  public:
    typedef std::tuple<int, int, int> Index;

    Vector3f center, size;
    Vector3f minCorner, maxCorner;
    unsigned int xres, yres, zres, totalCellAmount;
    float dx, dy, dz, invdx, invdy, invdz;

    // Physical constant
    float xi = 10; // hardening
    float e = 140000.0f; // initial young's modulus
    float theta_c = 2.5 * 1e-2; // Critical compression
    float theta_s = 7.5 * 1e-3; // Critical stretch
    // float rho = 400.0f; // initial density
    float nu = 0.2f; // Poisson's ratios
    float lambda0 = 0.0f;
    float mu0 = 0.0f;
    float initialVolume = 0.1f;

    // Time step
    float deltaTime = 0.001f;

    bool hasGravity = false;
    Vector3f gravity = Vector3f(0, -200, 0);

    float boundary = 0.1;

    std::vector<Particle> particles;
    Cell *cells;


    Grid(const Vector3f &center, const Vector3f &size, const Vector3u &resolution);
    ~Grid();

    void addParticle(const Particle &particle);

    void init();

    void step();

    Cell &getCell(const Grid::Index &index);

  private:
    void resetCells();
    void p2g();
    void updateGrid();
    void g2p();

    Index getCellIndex(const Particle &p) const;
    void populateCellNeighbors(const Index &index, std::vector<Index> &neighbors);

    Vector3f getCellCenter(const Grid::Index& index);

    Index getCellIndex(const int index) const;

  };
}

#endif