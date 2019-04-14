#ifndef SCRIPT_H
#define SCRIPT_H

#include "utility/math.h"
#include "viewer/context.h"
#include <string>

namespace nash {
  template <typename T> class Script {
  public:
    const std::string name;

    Script(const std::string &name) : name(name) {
      // Do nothing
    }

    void setContext(Context &ctx) { context = &ctx; }

    void bind(T &tar) { target = &tar; }

    virtual void onAttach(const T &tar) {
      // Do nothing
    }

    virtual void start() {
      // Do nothing
    }

    virtual void update() {
      // Do nothing
    }

    virtual void preRender() {
      // Do nothing
    }

    virtual void postRender() {
      // Do nothing
    }

  protected:
    Context *context;
    T *target;
  };
}

#endif
