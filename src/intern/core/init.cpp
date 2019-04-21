#include "core/init.h"

using namespace toaru;

void toaru::init(int argc, char *argv[]) {
  nanogui::init();
  Path::init(argv[0]);
}

void toaru::exit() {
  nanogui::shutdown();
}