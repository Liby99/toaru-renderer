#include "utility/path.h"
#include <stdexcept>

using namespace nash;

std::string Path::argv0 = std::string("");

void Path::init(char *a0) { argv0 = std::string(a0); }

const std::string &Path::getArgv0() { return argv0; }

std::string Path::getCurrentWorkingDirectory() {
#if defined(__APPLE__) || defined(__unix__)
  char buffer[BUF_SIZE];
  return std::string(getcwd(buffer, BUF_SIZE));
#else
  char buffer[BUF_SIZE];
  return std::string(_getcwd(buffer, BUF_SIZE));
#endif
}

std::string Path::getProgramDirectory() {
#if defined(__APPLE__) || defined(__unix__)
  if (getArgv0().size() > 0 && getArgv0()[0] == '/') {
    return getDirectory(getArgv0());
  } else {
    return getCurrentWorkingDirectory() + "/" + getDirectory(getArgv0());
  }
#elif defined(_WIN32)
  char buffer[256];
  GetModuleFileName(NULL, buffer, 256);
  return getDirectory(std::string(buffer));
#else
  throw std::runtime_error("Not implemented for platform");
#endif
}

std::string Path::getProgramFileName() {
#if defined(__APPLE__) || defined(__unix__)
  return getFileName(getArgv0());
#elif defined(_WIN32)
  char buffer[256];
  GetModuleFileName(NULL, buffer, 256);
  return getFileName(std::string(buffer));
#else
  throw std::runtime_error("Not implemented for platform");
#endif
}

std::string Path::getAbsolutePathTo(const std::string &relativePath, bool fromCWD) {
#if defined(__APPLE__) || defined(__unix__)
  if (relativePath[0] == '/') {
    return relativePath;
  } else {
    return (fromCWD ? getCurrentWorkingDirectory() : getProgramDirectory()) + "/" + relativePath;
  }
#elif defined(_WIN32)
  if (relativePath.length() > 3 && relativePath[1] == ':' && relativePath[2] == '/') {
    return relativePath;
  } else {
    return (fromCWD ? getCurrentWorkingDirectory() : getProgramDirectory()) + "/" + relativePath;
  }
#else
  throw std::runtime_error("Not implemented for platform");
#endif
}

std::string Path::getFileName(const std::string &path) {
  std::size_t found = path.find_last_of("/\\");
  return path.substr(found + 1);
}

std::string Path::getDirectory(const std::string &path) {
  std::size_t found = path.find_last_of("/\\");
  return path.substr(0, found + 1);
}
