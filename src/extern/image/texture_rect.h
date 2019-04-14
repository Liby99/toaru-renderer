#ifndef TEXTURE_RECT_H
#define TEXTURE_RECT_H

#include "image/image.h"
#include "image/texture.h"

namespace nash {
  class TextureRect : public Texture {
  public:
    TextureRect(const Image &image);
    virtual void init();
    virtual void bind(GLuint pos) const;

  protected:
    const Image *image;
  };
}

#endif