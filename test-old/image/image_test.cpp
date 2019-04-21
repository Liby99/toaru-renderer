#include <nash/nash.h>

using namespace nash;

void test(const std::string &filename) {
  Image image(Path::getAbsolutePathTo(filename));
  std::cout << "Loaded " << filename << std::endl;
  std::cout << " - width: " << image.width << std::endl;
  std::cout << " - height: " << image.height << std::endl;
  std::cout << " - num channels: " << image.numChannels << std::endl;
}

int main(int argc, char *argv[]) {
  Path::init(argv[0]);

  // Go to look at CMakeLists.txt for each copied images
  test("./image/head.bmp");
  test("./image/bridge3_posx.jpg");
  test("./image/gogo.png");
}
