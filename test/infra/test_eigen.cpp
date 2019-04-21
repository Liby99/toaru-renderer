#include <toaru/toaru.h>

using namespace toaru;

int main(int argc, char *argv[]) {

  Matrix3f R;
  R << 1, 2, 3, 4, 5, 6, 7, 8, 9;

  Matrix<float, 6, 1> strain;
  Vector3f offDiagonal;
  offDiagonal << R(1, 2), R(0, 2), R(0, 1);
  offDiagonal *= 2.0;
  strain << R.diagonal(), offDiagonal;

  std::cout << strain << std::endl;

  getc(stdin);
}
