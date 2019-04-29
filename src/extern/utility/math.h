#ifndef TOARU_MATH_H
#define TOARU_MATH_H

#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;

namespace toaru {
  using Eigen::Matrix;

  using std::make_shared;
  using std::make_unique;
  using std::shared_ptr;
  using std::unique_ptr;

  typedef Eigen::Matrix<uint32_t, 4, 1> Vector4u;
  typedef Eigen::Matrix<uint32_t, 3, 1> Vector3u;
  typedef Eigen::Matrix<uint32_t, 2, 1> Vector2u;
  typedef Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> MatrixXu;

  class Math {
  public:
    static const double PI;

    static Vector2f normalCartToPolar(const Vector3f &pos);
    static Vector3f polarToCart(const Vector2f &pol);
    static Vector3f polarToCart(float theta, float phi);

    static float random(float low, float up);
    static float random(float up);

    static bool isEqual(float f1, float f2);
  };
}

#endif
