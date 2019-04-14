#include "image/cube_map.h"

using namespace nash;

const CubeMap::Face CubeMap::FACES[6] = {CubeMap::Face::Top,   CubeMap::Face::Down,
                                         CubeMap::Face::Left,  CubeMap::Face::Right,
                                         CubeMap::Face::Front, CubeMap::Face::Back};

CubeMap::CubeMap(const Image &top, const Image &down, const Image &left, const Image &right,
                 const Image &front, const Image &back)
    : top(&top), down(&down), left(&left), right(&right), front(&front), back(&back), Texture() {}

void CubeMap::init() {
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, right->width, right->height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, right->data);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, left->width, left->height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, left->data);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, top->width, top->height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, top->data);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, down->width, down->height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, down->data);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, front->width, front->height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, front->data);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, back->width, back->height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, back->data);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::bind(GLuint pos) const {
  Texture::bind(pos);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
}

Vector4u CubeMap::getColor(Vector3f dir) const {
  Vector3f d = dir.normalized();
  float sqrt2 = std::sqrt(2.0);
  float ax = std::abs(d.x()), ay = std::abs(d.y()), az = std::abs(d.z());
  float u, v, ma;
  const Image *img;
  if (az >= ax && az >= ay) {
    img = d.z() > 0 ? front : back;
    ma = 0.5f / az;
    u = d.z() > 0 ? d.x() : -d.x();
    v = -d.y();
  } else if (ay >= ax) {
    img = d.y() > 0 ? top : down;
    ma = 0.5f / ay;
    u = d.x();
    v = d.y() > 0 ? d.z() : d.x();
  } else {
    img = d.x() > 0 ? right : left;
    ma = 0.5f / ax;
    u = d.x() > 0 ? -d.z() : d.z();
    v = -d.y();
  }
  int i = (u * ma + 0.5) * img->width, j = (v * ma + 0.5) * img->height;
  return img->getColor(i, j);
}

const Image &CubeMap::getImage(Face face) const {
  switch (face) {
    case CubeMap::Face::Top: return *top;
    case CubeMap::Face::Down: return *down;
    case CubeMap::Face::Left: return *left;
    case CubeMap::Face::Right: return *right;
    case CubeMap::Face::Front: return *front;
    case CubeMap::Face::Back: return *back;
  }
}

Vector3f CubeMap::getCentralDirection(Face face) const {
  switch (face) {
    case CubeMap::Face::Top: return Vector3f(0, 1, 0);
    case CubeMap::Face::Down: return Vector3f(0, -1, 0);
    case CubeMap::Face::Right: return Vector3f(1, 0, 0);
    case CubeMap::Face::Left: return Vector3f(-1, 0, 0);
    case CubeMap::Face::Front: return Vector3f(0, 0, 1);
    case CubeMap::Face::Back: return Vector3f(0, 0, -1);
  }
}

Vector3f CubeMap::getDirectionTo(Face face, unsigned int i, unsigned int j) const {
  const Image &img = getImage(face);
  float fi = (float)i / img.width - 0.5;
  float fj = -(float)j / img.height + 0.5;
  switch (face) {
    case CubeMap::Face::Top: return Vector3f(fi, 0.5, -fj).normalized();
    case CubeMap::Face::Down: return Vector3f(fi, -0.5, fj).normalized();
    case CubeMap::Face::Right: return Vector3f(0.5, fj, -fi).normalized();
    case CubeMap::Face::Left: return Vector3f(-0.5, fj, fi).normalized();
    case CubeMap::Face::Front: return Vector3f(fi, fj, 0.5).normalized();
    case CubeMap::Face::Back: return Vector3f(-fi, fj, -0.5).normalized();
  }
}