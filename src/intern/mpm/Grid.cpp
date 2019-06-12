#include "mpm/Grid.h"

using namespace toaru::mpm;

Grid::Grid(const Vector3f& center, const Vector3f& size, const Vector3u& resolution)
  : center(center), size(size) {

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

void Grid::addParticle(const Particle& particle) {
  particles.push_back(particle);
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

void Grid::p2g() {
  for (Particle& p : particles) {
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

    Vector3f constant1(1.125f, 1.125f, 1.125f);
    Vector3f constant2(0.75f, 0.75f, 0.75f);

    // 2.1 Calculate weights (in column)
    Matrix3f weights;
    weights << 0.5f * powInvh.cwiseProduct(powCellDiff) + 1.5f * invh.cwiseProduct(cellDiff) + constant1,
      -powInvh.cwiseProduct(powCellDiff) + constant2,
      0.5f * powInvh.cwiseProduct(powCellDiff) - 1.5f * invh.cwiseProduct(cellDiff) + constant1;

    p.W = weights;

    // 2.2 CPIC
    float exp = std::exp(e * (1 - p.Jp));
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
    Matrix3f Dp;
    Dp.diagonal() << 0.25 * dx * dx, 0.25 * dy * dy, 0.25 * dz * dz;
    Matrix3f invDp = Dp.inverse();

    // Fixed corotated constitutive model
    // Stress tensor ??
    Matrix3f stress = 2.0 * mu * (p.F - Q) + lambda * (J - 1) * J * p.F.transpose();

    // Eqn 29 Ni(x)Qp(xi − xp) and Eqn 18 det(p.F) * σ = (∂Ψ/∂p.F)p.F^T ??
    //Matrix3f Qp = deltaTime * initialVolume * invDp * J * stress + p.mass * p.C;

    // APIC momentum
    auto apicP = p.mass * p.C;

    // Stress momentum;
    auto stressP = 4.0 * invDp * deltaTime * initialVolume * stress * p.F.transpose();

    // 2.3 Update neighboring grid
    std::vector<Index> neighbors;
    populateCellNeighbors(index, neighbors);
    auto [x0, y0, z0] = index;
    for(int i = 0 ; i < neighbors.size(); i ++) {
      auto [x, y, z] = neighbors[i];
      float weight = p.W.col(x - x0 + 1).x() *
          p.W.col(y - y0 + 1).y() *
          p.W.col(z - z0 + 1).z();
      Vector3f deltaPos =  getCellCenter(neighbors[i]) - p.position;
      Vector3f momentum = p.velocity * p.mass;
      // P2G
      Cell& cell = getCell(neighbors[i]);
      // Mass
      cell.mass += weight * p.mass;
      // Momentum ??
      cell.momentum += momentum;

      // APIC momentum contribution
      cell.momentum += apicP * deltaPos * weight;

      // Stress momentum contribution
      cell.momentum += stress * deltaPos * weight;
      //cell.momentum += weight * (momentum + Qp * deltaPos);
    }
  }
}

void Grid::updateGrid() {
  // 3. Calculate grid velocities
  for (int i = 0; i < totalCellAmount; i++) {
    // 3.1 Calculate grid velocity based on momentum found in the P2G stage
    Cell & cell = cells[i];

    // Handle when mass is zero
    if(cell.mass > 0) {
      // p = mv, v = p/m
      cell.velocity += cell.momentum / cell.mass;

      if(hasGravity) {
        cell.velocity += gravity * deltaTime;
      }

      // 3.2 enforce boundary conditions
      auto center = getCellCenter(getCellIndex(i));
      if(center.x() < boundary || center.x() > xres * dx - boundary ||
		    center.z() < boundary || center.z() > zres * dz - boundary ||
		    center.y() > yres * dy - boundary) {
        cell.velocity = Vector3f::Zero();
      }

      if(center.y() < boundary) {
        cell.velocity.y() = std::max(0.0f, -cell.velocity.y());
      }

    }
    // TODO
  }
}

void Grid::g2p() {
  for (Particle& p : particles) {
    /*if (isAPIC) {

    }else if (isPolyPIC) {

    }*/

    // Reset particle affine momentum and velocity to zero
    p.velocity = Vector3f::Zero();
    p.C = Matrix3f::Zero();

    // APIC G2P affine velocity reconstruction
    // Quadratic Dp, analogous to an inertia tensor
    Matrix3f Dp;
    Dp.diagonal() << 0.25 * dx * dx, 0.25 * dy * dy, 0.25 * dz * dz;
    Matrix3f invDp = Dp.inverse();

    // Get neighboring grid
    auto index = getCellIndex(p);
    std::vector<Index> neighbors;
    populateCellNeighbors(index, neighbors);
    auto [x0, y0, z0] = index;
    for(int i = 0 ; i < neighbors.size(); i ++) {
      auto [x, y, z] = neighbors[i];
      auto & cell = getCell(neighbors[i]);
      float weight = p.W.col(x - x0 + 1).x() *
          p.W.col(y - y0 + 1).y() *
          p.W.col(z - z0 + 1).z();
      Vector3f deltaPos =  getCellCenter(neighbors[i]) - p.position;

      // Updating particle affine momentum
      p.C += cell.velocity * deltaPos.transpose() * weight;

      // Updating velocity
      p.velocity += cell.velocity * weight;
	  }

    // Advection
    p.position += p.velocity * deltaTime;

    // Updating particle affine momentum
    p.C *= invDp;

    // 4.1: update particle's deformation gradient using MLS-MPM's velocity gradient estimate
    // Velocity gradient = new Affine momentum
    p.F = (Matrix3f::Identity() + deltaTime * p.C) * p.F;

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

Cell& Grid::getCell(const Grid::Index& index) {
  auto [x, y, z] = index;
  return cells[x * yres * zres + y * zres + z];
}

Grid::Index Grid::getCellIndex(const Particle& p) const {
  const Vector3f diff = p.position - minCorner;
  return std::make_tuple(
    std::floor(diff.x() * invdx),
    std::floor(diff.y() * invdy),
    std::floor(diff.z() * invdz)
  );
}

void Grid::populateCellNeighbors(const Grid::Index& index, std::vector<Grid::Index>& neighbors) {
  auto [x, y, z] = index;
  for (int i = x - 1; i <= x + 1; i++) {
    for (int j = y - 1; j <= y + 1; j++) {
      for (int k = z - 1; k <= z + 1; k++) {
        if (i >= 0 && i < xres && j >= 0 && j < yres && k >= 0 && k < zres && (i != x || j != y || k != z)) {
          neighbors.push_back(std::make_tuple(i, j, k));
        }
      }
    }
  }
}

Vector3f Grid::getCellCenter(const Grid::Index& index) {
  const auto& [x, y, z] = index;
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
