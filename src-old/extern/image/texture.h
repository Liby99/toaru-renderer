#ifndef TEXTURE_H
#define TEXTURE_H

#include <nanogui/nanogui.h>
#include <vector>

namespace nash {
  class Texture {
  public:
    GLuint textureId;
    virtual void init();
    virtual void bind(GLuint pos) const;

    static void initAll();

  protected:
    Texture();

  private:
    static std::vector<Texture *> textures;
  };
}

#endif
