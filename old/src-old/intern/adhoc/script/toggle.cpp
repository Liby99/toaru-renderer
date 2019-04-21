#include "adhoc/script/toggle.h"

using namespace nash;

Toggle::Toggle(const std::string &name) : Toggle(name, true) {}

Toggle::Toggle(const std::string &name, char key) : Toggle(name, true, key) {}

Toggle::Toggle(const std::string &name, bool initial) : Toggle(name, initial, ' ') {}

Toggle::Toggle(const std::string &name, bool initial, char key)
    : Script(name), initial(initial), useKey(true), toggleKey(key) {}

Toggle::Toggle(const std::string &name, ToggleFunction func) : Toggle(name, true, func) {}

Toggle::Toggle(const std::string &name, bool initial, ToggleFunction func)
    : Script(name), initial(initial), useKey(false), toggleFunc(func) {}

void Toggle::start() {
  show = initial;
  target->setHidden(!show);
}

void Toggle::update() {
  bool result = eval();
  if (!activating && result) {
    activating = true;
    show = !show;
    target->setHidden(!show);
  } else if (activating && !result) {
    if (!eval()) {
      activating = false;
    }
  }
}

bool Toggle::eval() {
  if (useKey) {
    return context->getKey(toggleKey);
  } else {
    return toggleFunc(*context);
  }
}
