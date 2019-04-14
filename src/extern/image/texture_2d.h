#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include "image/image.h"
#include "image/texture.h"

namespace nash {
  class Texture2D : public Texture {
  public:
    Texture2D(const Image &image);
    virtual void init();
    virtual void bind(GLuint pos) const;

  protected:
    const Image *image;
  };
}

#endif
