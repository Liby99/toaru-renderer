#include <mpm/grid.h>
#include <iostream>
//#include <omp.h>

using namespace toaru;
using Grid = mpm::Grid;
using Cell = mpm::Cell;
using Particle = mpm::Particle;

Grid::Grid(const Vector3f &center, const Vector3f &size, const Vector3u &resolution)
  : center(center),
    size(size) {

  // Initialize the resolutionn
  xres = resolution(0), yres = resolution(1), zres = resolution(2);
  totalCellAmount = xres * yres * zres;

  // Other parameters
  dx = size.x() / xres, dy = size.y() / yres, dz = size.z() / zres;
  invdx = 1.0f / dx, invdy = 1.0f / dy, invdz = 1.0f / dz;
  minCorner = center - size / 2.0f, maxCorner = center + size / 2.0f;

  // Initialize the cells grid
  cells = new Cell[totalCellAmount];

  // Initialize intial lame parameter
  lambda0 = e * nu / ((1 + nu) * (1 - 2 * nu));
  mu0 = e / (2 * (1 + nu));
}

Grid::~Grid() {
  delete[] cells;
}

void Grid::addParticle(const Particle &particle) {
  particles.push_back(particle);
}

void Grid::init() {
  p2g();
  for (Particle &p : particles) {
    auto index = getCellIndex(p);

    // Get center of current cell
    Vector3f cellCenter = getCellCenter(index);

    // Get cell difference
    Vector3f cellDiff = p.position - cellCenter;
    Vector3f powCellDiff(
                         cellDiff.x() * cellDiff.x(),
                         cellDiff.y() * cellDiff.y(),
                         cellDiff.z() * cellDiff.z());

    Vector3f invh(invdx, invdy, invdz);
    Vector3f powInvh(invdx * invdx, invdy * invdy, invdz * invdz);

    Vector3f constant1(0.125f, 0.125f, 0.125f);
    Vector3f constant2(0.75f, 0.75f, 0.75f);

    Vector3f x = invh.cwiseProduct(cellDiff);
    Vector3f xx = powInvh.cwiseProduct(powCellDiff);

    // 2.1 Calculate weights (in column)
    Matrix3f weights;
    weights << constant1 - 0.5f * x + 0.5f * xx,
      -xx + constant2,
      constant1 + 0.5f * x + 0.5f * xx;

    p.W = weights;
    float density = 0.0f;
    // iterate over neighbouring 3x3 cells

    // 2.3 Update neighboring grid
    std::vector<Index> neighbors;
    populateCellNeighbors(index, neighbors);
    auto [x0, y0, z0] = index;
    for (int i = 0; i < neighbors.size(); i ++) {
      auto [x, y, z] = neighbors[i];
      float weight = p.W.col(x - x0 + 1).x() *
                     p.W.col(y - y0 + 1).y() *
                     p.W.col(z - z0 + 1).z();

      if (weight > 1) {
        std::cout << "wrong" << std::endl;
      }
      // P2G
      Cell &cell = getCell(neighbors[i]);
      density += cell.mass * weight;
    }

    // per-particle volume estimate has now been computed
    float volume = p.mass / density;
    p.initialVolume = volume;
  }
  resetCells();
}

void Grid::step() {

  // 1. Clear all cells
  resetCells();

  // 2. Particle to Grid (P2G)
  p2g();

  // 3. Update grid values (velocity)
  updateGrid();

  // 4. Grid to Particle (G2P)
  g2p();
}

void Grid::resetCells() {
  for (int i = 0; i < totalCellAmount; i++) {
    cells[i].reset();
  }
}

//#pragma optimize("", off)
void Grid::p2g() {
#pragma omp parallel for
  for (int c = 0; c < particles.size(); c ++) {
    Particle &p = particles[c];
    //std::cout << omp_get_max_threads() <<std::endl;
    auto index = getCellIndex(p);

    // Get center of current cell
    Vector3f cellCenter = getCellCenter(index);

    // Get cell difference
    Vector3f cellDiff = p.position - cellCenter;
    Vector3f powCellDiff(
                         cellDiff.x() * cellDiff.x(),
                         cellDiff.y() * cellDiff.y(),
                         cellDiff.z() * cellDiff.z());

    Vector3f invh(invdx, invdy, invdz);
    Vector3f powInvh(invdx * invdx, invdy * invdy, invdz * invdz);

    Vector3f constant1(0.125f, 0.125f, 0.125f);
    Vector3f constant2(0.75f, 0.75f, 0.75f);

    Vector3f x = invh.cwiseProduct(cellDiff);
    Vector3f xx = powInvh.cwiseProduct(powCellDiff);

    // 2.1 Calculate weights (in column)
    Matrix3f weights;
    weights << constant1 - 0.5f * x + 0.5f * xx,
      -xx + constant2,
      constant1 + 0.5f * x + 0.5f * xx;

    p.W = weights;

    // 2.2 CPIC
    float exp = std::exp(xi * (1 - p.Jp));
    float lambda = lambda0 * exp;
    float mu = mu0 * exp;

    // volume: J = det(F)
    float J = p.F.determinant();

    // Polar decomposition
    Eigen::JacobiSVD<Matrix3f> D(p.F, Eigen::ComputeFullU | Eigen::ComputeFullV);

    // Q = V * U';
    // F = U * S * U';
    // origF = Q*F = V * U' * U * S * U' = V * S * U';
    Matrix3f Q = D.matrixV() * D.matrixU().transpose();
    Matrix3f S = Matrix3f::Zero();
    S.diagonal() << D.singularValues();
    Matrix3f F = D.matrixU() * S * D.matrixU().transpose();

    // Quadratic Dp, analogous to an inertia tensor
    Matrix3f Dp = Matrix3f::Zero();
    Dp.diagonal() << 0.25 * dx * dx, 0.25 * dy * dy, 0.25 * dz * dz;
    Matrix3f invDp = Dp.inverse();

    // Fixed corotated constitutive model
    // Stress tensor ??
    Matrix3f l = Matrix3f::Zero();
    l.diagonal() << lambda * (J - 1) * J, lambda * (J - 1) * J, lambda * (J - 1) * J;
    Matrix3f stress = 2.0 * mu * (p.F - Q) * p.F.transpose() + l;

    // Eqn 29 Ni(x)Qp(xi − xp) and Eqn 18 det(p.F) * σ = (∂Ψ/∂p.F)p.F^T ??
    //Matrix3f Qp = deltaTime * initialVolume * invDp * J * stress + p.mass * p.C;

    // APIC momentum
    auto apicP = p.mass * p.C;

    // Stress momentum;
    auto stressP = - invDp * deltaTime * p.initialVolume * stress;

    //Matrix3f stress = 2.0 * mu * (p.F - Q) + lambda * (J - 1) * J * p.F.transpose();

    // Eqn 29 Ni(x)Qp(xi − xp) and Eqn 18 det(p.F) * σ = (∂Ψ/∂p.F)p.F^T ??
    //Matrix3f Qp = deltaTime * initialVolume * invDp * J * stress + p.mass * p.C;

    // Stress momentum;
    //auto stressP = invDp * deltaTime * p.initialVolume * stress * p.F * p.F.transpose();

    // 2.3 Update neighboring grid
    std::vector<Index> neighbors;
    populateCellNeighbors(index, neighbors);
    auto [x0, y0, z0] = index;
    for (int i = 0; i < neighbors.size(); i ++) {
      auto [x, y, z] = neighbors[i];
      float weight = p.W.col(x - x0 + 1).x() *
                     p.W.col(y - y0 + 1).y() *
                     p.W.col(z - z0 + 1).z();

      if (weight > 1) {
        std::cout << "wrong" << std::endl;
      }
      Vector3f deltaPos = (getCellCenter(neighbors[i]) - p.position);
      Vector3f momentum = p.velocity * p.mass;
      // P2G
      Cell &cell = getCell(neighbors[i]);
      float old = cell.momentum.y();
      // Mass
      cell.mass += weight * p.mass;
      // Momentum ??
      cell.momentum += weight * momentum;

      // APIC momentum contribution
      cell.momentum += apicP * deltaPos * weight;

      // Stress momentum contribution
      cell.momentum += stressP * deltaPos * weight;
      if (isnan(cell.momentum.y())) {
        int a = 0;
      }
      if (abs(cell.momentum.y()) > 1000) {
        int a = 0;
      }
      //cell.momentum += weight * (momentum + Qp * deltaPos);
    }
  }
}

void Grid::updateGrid() {
  // 3. Calculate grid velocities
  #pragma omp parallel for
  for (int i = 0; i < totalCellAmount; i++) {
    // 3.1 Calculate grid velocity based on momentum found in the P2G stage
    Cell &cell = cells[i];

    // Handle when mass is zero
    if (cell.mass > 0) {
      // p = mv, v = p/m
      cell.velocity += cell.momentum / cell.mass;

      if (hasGravity) {
        cell.velocity += gravity * deltaTime;
      }

      // 3.2 enforce boundary conditions
      float y = cell.velocity.y();
      auto center = getCellCenter(getCellIndex(i));
      if (center.x() < boundary || center.x() > xres * dx - boundary ||
          center.z() < boundary || center.z() > zres * dz - boundary ||
          center.y() > yres * dy - boundary) {
        cell.velocity = Vector3f::Zero();
      }

      if (center.y() < boundary) {
        cell.velocity.y() = std::max(0.0f, y);
      }

      if (abs(cell.velocity.y()) > 10) {
        int a = 0;
      }

    }
    // TODO
  }
}

void Grid::g2p() {
  int count = 0;
#pragma omp parallel for
  for (int c = 0; c < particles.size(); c ++) {
    Particle &p = particles[c];
    count += 1;
    /*if (isAPIC) {

    }else if (isPolyPIC) {

    }*/

    // Reset particle affine momentum and velocity to zero
    p.velocity = Vector3f::Zero();
    p.C = Matrix3f::Zero();

    // APIC G2P affine velocity reconstruction
    // Quadratic Dp, analogous to an inertia tensor
    Matrix3f Dp = Matrix3f::Zero();
    Dp.diagonal() << 0.25 * dx, 0.25 * dy, 0.25 * dz;
    Matrix3f invDp = Dp.inverse();
    Vector3f invh(invdx, invdy, invdz);

    // Get neighboring grid
    auto index = getCellIndex(p);

    std::vector<Index> neighbors;
    populateCellNeighbors(index, neighbors);
    auto [x0, y0, z0] = index;
    for (int i = 0; i < neighbors.size(); i ++) {
      auto [x, y, z] = neighbors[i];
      auto &cell = getCell(neighbors[i]);
      float weight = p.W.col(x - x0 + 1).x() *
                     p.W.col(y - y0 + 1).y() *
                     p.W.col(z - z0 + 1).z();
      Vector3f deltaPos = (getCellCenter(neighbors[i]) - p.position);
      // Updating particle affine momentum
      p.C += cell.velocity * deltaPos.transpose() * weight;

      if (abs(p.C(1, 1)) > 1) {
        int a = 0;
      }
      // Updating velocity
      if (isnan(cell.velocity.x())) {
        int a = 0;
      }
      p.velocity += cell.velocity * weight;
    }

    // Advection
    p.position += p.velocity * deltaTime;

    if (isnan(p.position.x())) {
      int a = 0;
    }
    //std::cout << p.position << std::endl;
    p.position.x() = std::max(std::min(p.position.x(), xres * dx), 0.0f);
    p.position.y() = std::max(std::min(p.position.y(), yres * dy), 0.0f);
    p.position.z() = std::max(std::min(p.position.z(), zres * dz), 0.0f);

    // Updating particle affine momentum
    p.C *= invDp;

    // 4.1: update particle's deformation gradient using MLS-MPM's velocity gradient estimate
    // Velocity gradient = new Affine momentum
    p.F = (Matrix3f::Identity() + deltaTime * p.C) * p.F;

    // SVD decomposition
    Eigen::JacobiSVD<Matrix3f> D(p.F, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Matrix3f S = Matrix3f::Zero();
    S.diagonal() << std::clamp(D.singularValues()(0), 1.0f - theta_c, 1.0f + theta_s),
      std::clamp(D.singularValues()(1), 1.0f - theta_c, 1.0f + theta_s),
      std::clamp(D.singularValues()(2), 1.0f - theta_c, 1.0f + theta_s);

    float oldJ = p.F.determinant();
    p.F = D.matrixU() * S * D.matrixV().transpose();

    p.Jp = std::clamp(p.Jp * oldJ / p.F.determinant(), 0.6f, 20.0f);

    if (count == 500) {
      int a = 0;
    }
    // 4.1: update particle's deformation gradient using MLS-MPM's velocity gradient estimate
    // Reference: MLS-MPM paper, Eq. 17

    //// 4.3: calculate our new particle velocities
    //for (int i = 0; i < totalCellAmount; i++) {
    //  // 4.3.1:
    //  // get this cell's weighted contribution to our particle's new velocity
    //}

    //// 4.4: advect particle positions by their velocity
  }
}
//#pragma optimize("", on)
Cell &Grid::getCell(const Grid::Index &index) {
  auto [x, y, z] = index;
  return cells[x * yres * zres + y * zres + z];
}

Grid::Index Grid::getCellIndex(const Particle &p) const {
  const Vector3f diff = p.position - minCorner;
  return std::make_tuple(
                         std::floor(diff.x() * invdx),
                         std::floor(diff.y() * invdy),
                         std::floor(diff.z() * invdz)
                        );
}

void Grid::populateCellNeighbors(const Grid::Index &index, std::vector<Grid::Index> &neighbors) {
  auto [x, y, z] = index;
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      for (int k = z - 1; k <= z + 1; k++) {
        if (i >= 0 && i < xres && j >= 0 && j < yres && k >= 0 && k < zres) {
          neighbors.push_back(std::make_tuple(i, j, k)); //  && (i != x || j != y || k != z)
        }
      }
    }
  }
}

Vector3f Grid::getCellCenter(const Grid::Index &index) {
  const auto &[x, y, z] = index;
  return Vector3f(
                  minCorner.x() + x * dx + 0.5 * dx,
                  minCorner.y() + y * dy + 0.5 * dy,
                  minCorner.z() + z * dz + 0.5 * dz);
}

Grid::Index Grid::getCellIndex(const int index) const {
  int x = index / (yres * zres);
  int g = (index % (yres * zres));
  int y = g / zres;
  int z = (g % zres);
  return std::make_tuple(x, y, z);
}
