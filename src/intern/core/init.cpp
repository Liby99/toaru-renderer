#include "core/init.h"

using namespace toaru;

void toaru::init() {
  nanogui::init();
}

void toaru::exit() {
  nanogui::shutdown();
}