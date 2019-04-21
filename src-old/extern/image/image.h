#ifndef IMAGE_H
#define IMAGE_H

#include "utility/math.h"
#include "utility/path.h"
#include <string>

namespace nash {
  class Image {
  public:
    int width, height, numChannels;
    unsigned char *data;

    Image(int width, int height);
    Image(const std::string &filename);
    ~Image();

    void setColor(int i, int j, Vector4u color);
    void setChannel(int i, int j, int channel, unsigned char c);
    Vector4u getColor(int i, int j) const;
  };
}

#endif
