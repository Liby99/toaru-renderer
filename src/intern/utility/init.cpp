#include "utility/init.h"

using namespace nash;

void Nash::init(int argc, char *argv[]) {
  nanogui::init();
  Path::init(argv[0]);
}

void Nash::shutdown() { nanogui::shutdown(); }
