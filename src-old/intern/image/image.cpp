#include "image/image.h"
#include <stb_image.h>
#include <stdexcept>

using namespace nash;

Image::Image(int width, int height) : width(width), height(height), numChannels(4) {
  data = new unsigned char[width * height * numChannels];
}

Image::Image(const std::string &filename) : width(0), height(0), numChannels(0) {
  data = stbi_load(filename.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
  if (!data) {
    throw std::runtime_error("Cannot load file " + filename);
  }
}

Image::~Image() { stbi_image_free(data); }

void Image::setColor(int i, int j, Vector4u color) {
  int start = (j * width + i) * numChannels;
  for (int i = 0; i < numChannels; i++) {
    data[start + i] = color[i];
  }
}

void Image::setChannel(int i, int j, int channel, unsigned char c) {
  int start = (j * width + i) * numChannels;
  data[start + channel] = c;
}

Vector4u Image::getColor(int i, int j) const {
  int start = (j * width + i) * numChannels;
  return Vector4u(data[start], data[start + 1], data[start + 2], data[start + 3]);
}