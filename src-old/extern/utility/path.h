#ifndef PATH_H
#define PATH_H

#include <string>

#if defined(__APPLE__) || defined(__unix__)
#include <unistd.h>
#elif defined(_WIN32)
#include <direct.h>
#include <windows.h>
#endif

namespace nash {
  class Path {
  public:
    static void init(char *a0);
    static const std::string &getArgv0();
    static std::string getCurrentWorkingDirectory();
    static std::string getProgramDirectory();
    static std::string getProgramFileName();
    static std::string getAbsolutePathTo(const std::string &relativePath, bool fromCWD = false);
    static std::string getFileName(const std::string &path);
    static std::string getDirectory(const std::string &path);

  private:
    static const int BUF_SIZE = 128;
    static std::string argv0;
  };
};

#endif
