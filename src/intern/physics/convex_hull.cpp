#include "physics/convex_hull.h"
#include <iostream>
#pragma optimize("", off)
toaru::ConvexHull::ConvexHull(std::vector<Vector3f> ps)
  : center(Vector3f::Zero()),
    volume(0.0f) {
  points.insert(points.end(), ps.begin(), ps.end());

  // currently using giftWrap
  giftWrap();

  // calculate center
  for (const auto &point : points) {
    center += point;
  }
  center /= static_cast<float>(points.size());

  // calculate volume: sum of facets
  for (int i = 0; i < faces.size(); i++) {
    auto [t0, t1, t2] = faces[i];
    const Vector3f &p0 = points[t0];
    const Vector3f &p1 = points[t1];
    const Vector3f &p2 = points[t2];
    Vector3f n = -(p1 - p0).cross(p2 - p1);
    if ((center - p0).dot(n) > 0) {
      n = -n;
      // std::cout << "inverted" << std::endl;
    }
    float area = n.norm() / 2.0f;
    n = n.normalized();
    float curr = p0.dot(n) * area;
    volume += curr;
  }
  volume /= 3.0;
  //std::cout << volume << std::endl;
}

std::tuple<int, int, int> toaru::ConvexHull::findTriangleOnHull() {
  Edge edge = findEdgeOnHull();
  int r = pivotOnEdge(edge);
  return {edge.p, edge.q, r};
}

toaru::Edge toaru::ConvexHull::findEdgeOnHull() {
  int p = findFarmostPoint();
  Vector3f q = points[p];
  bool found = false;
  for (auto &r : points) {
    if (Math::isEqual(q.z(), r.z()) && Math::isEqual(q.y(), r.y()) && q.x() < r.x()) {
      q = r;
      found = true;
    }
  }
  if (!found) {
    q = q + Vector3f(1, 0, 0);
  }
  int r = pivotOnEdge(points[p], q);
  return Edge(p, r);
}

int toaru::ConvexHull::findFarmostPoint() {
  int minIndex = -1;
  Vector3f min = Vector3f(std::numeric_limits<float>::max(),
                          std::numeric_limits<float>::max(),
                          -std::numeric_limits<float>::lowest());
  for (int i = 0; i < points.size(); i++) {
    auto point = points[i];
    if (compareTwoPoint(point, min) < 0) {
      // if point < min
      min = point;
      minIndex = i;
    }
  }
  return minIndex;
}

int toaru::ConvexHull::compareTwoPoint(const Vector3f &p1, const Vector3f &p2) {
  if (p1.y() < p2.y()) {
    return -1;
  } else if (Math::isEqual(p1.y(), p2.y())) {
    // y is close, then compare x
    if (p1.x() < p2.x()) {
      return -1;
    } else if (Math::isEqual(p1.x(), p2.x())) {
      // x is close, then compare z
      if (p1.z() > p2.z()) {
        return -1;
      } else {
        // no equivalent
        return 1;
      }
    } else {
      return 1;
    }
  } else {
    return 1;
  }
}

float toaru::ConvexHull::squaredArea(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3) {
  return ((p2 - p1).cross((p3 - p1))).squaredNorm();
}

float toaru::ConvexHull::signedVolume(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3,
                                      const Vector3f &p4) {
  const auto e1 = p2 - p1;
  const auto e2 = p3 - p1;
  const auto e3 = p4 - p1;

  // Calculate volume
  constexpr float factor = 1.0 / 6.0;
  const float volume = factor * e3.dot(e1.cross(e2));
  return volume;
}

int toaru::ConvexHull::pivotOnEdge(Edge &edge) {
  return pivotOnEdge(points[edge.p], points[edge.q]);
}

int toaru::ConvexHull::pivotOnEdge(const Vector3f &q0, const Vector3f &q1) {
  int ret = 0;
  Vector3f pPoint = points[ret];
  float areaSq = squaredArea(q0, q1, pPoint);
  for (int i = 1; i < points.size(); i++) {
    const auto &curr = points[i];
    const float volume = signedVolume(q0, q1, pPoint, curr);
    if (volume < 0) {
      ret = i;
      pPoint = curr;
    } else if (volume == 0) {
      const float areaSq2 = squaredArea(q0, q1, curr);
      if (areaSq2 > areaSq) {
        ret = i;
        pPoint = curr;
        areaSq = areaSq2;
      }
    }
  }

  return ret;
}

void toaru::ConvexHull::giftWrap() {
  // TODO: clean up
  
  auto t = findTriangleOnHull();
  const auto &[t0, t1, t2] = t;
  edges.insert({Edge(t1, t0), Edge(t2, t1), Edge(t0, t2)});
  faces.push_back(t);

  std::queue<Edge> queue;
  queue.emplace(t0, t2);
  queue.emplace(t2, t1);
  queue.emplace(t1, t0);
  while (!queue.empty()) {

    // Get first edge in the queue
    Edge edge = queue.front();
    queue.pop();

    // Find the edge
    auto res = edges.find(edge);
    if (!res->processed) {
      int r = pivotOnEdge(edge);
      auto t = std::make_tuple(edge.p, edge.q, r);
      const auto &[t0, t1, t2] = t;
      faces.push_back(t);
      auto e1 = edges.emplace(t0, t2);
      auto e2 = edges.emplace(t2, t1);
      auto e3 = edges.emplace(t1, t0);

      if (e1.second) {
        queue.push(*e1.first);
      } else {
        e1.first->processed = true;
      }
      if (e2.second) {
        queue.push(*e2.first);
      } else {
        e2.first->processed = true;
      }
      if (e3.second) {
        queue.push(*e3.first);
      } else {
        e3.first->processed = true;
      }
      res->processed = true;
    }
  }
}

#pragma optimize("", on)
