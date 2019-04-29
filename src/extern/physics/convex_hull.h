#ifndef TOARU_CONVEX_HULL_H
#define TOARU_CONVEX_HULL_H

#include "utility/math.h"
#include <queue>
#include <unordered_set>

namespace toaru {

  class Edge {
  public:
    const int p;
    const int q;
    mutable bool processed;

    Edge(int p, int q) : p(p), q(q), processed(false) {}

    bool operator==(const Edge &other) const {
      return ((this->p == other.p) && (this->q == other.q)) ||
             ((this->p == other.q) && (this->q == other.p));
    }
  };

  struct EdgeHash {
    std::size_t operator()(const Edge &edge) const {
      return std::hash<int>()(edge.p) ^ std::hash<int>()(edge.q);
    }
  };

  struct EdgeEqual {
    bool operator()(const Edge &a, const Edge &b) const {
      return a == b;
    }
  };

  class ConvexHull {
  public:
    std::vector<Vector3f> points;
    std::vector<int> pointsOnHull;
    std::unordered_set<Edge, EdgeHash, EdgeEqual> edges;
    std::vector<std::tuple<int, int, int>> faces;

    Vector3f center;
    float volume;

    ConvexHull(std::vector<Vector3f> ps);

    std::tuple<int, int, int> findTriangleOnHull();

    Edge findEdgeOnHull();

    // Bottom most, Left Most, Back Most: -y, -x, +z
    int findFarmostPoint();

    // Bottom most, Left Most, Back Most: -y, -x, +z => smallest
    // Return +1 if p1 > p2
    // Return -1 if p1 < p2
    int compareTwoPoint(const Vector3f &p1, const Vector3f &p2);

    float squaredArea(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3);

    float signedVolume(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3,
                       const Vector3f &p4);

    int pivotOnEdge(Edge &edge);

    int pivotOnEdge(const Vector3f &q0, const Vector3f &q1);

    void giftWrap();
  };
}

#endif
