#ifndef FILE_H
#define FILE_H

#include <string>

namespace nash {
  class File {
  public:
    virtual void save(const std::string &filepath) = 0;
    virtual void load(const std::string &filepath) = 0;
  };
}

#endif // FILE_H
