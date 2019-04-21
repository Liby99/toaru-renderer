#ifndef TOGGLE_H
#define TOGGLE_H

#include "object/object.h"
#include "utility/script.h"
#include <functional>

namespace nash {
  class Toggle : public Script<Object> {
  public:
    typedef std::function<bool(Context &)> ToggleFunction;

    bool initial, useKey;
    char toggleKey;
    ToggleFunction toggleFunc;

    Toggle(const std::string &name);
    Toggle(const std::string &name, char key);
    Toggle(const std::string &name, bool initial);
    Toggle(const std::string &name, bool initial, char key);
    Toggle(const std::string &name, ToggleFunction func);
    Toggle(const std::string &name, bool initial, ToggleFunction func);

    virtual void start();
    virtual void update();

  private:
    bool show, activating;
    bool eval();
  };
}

#endif
