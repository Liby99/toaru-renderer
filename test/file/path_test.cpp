#include <iostream>
#include <nash/nash.h>

using namespace nash;

int main(int argc, char *argv[]) {
  Path::init(argv[0]);
  std::cout << "current working directory: " << Path::getCurrentWorkingDirectory() << std::endl;
  std::cout << "argv[0]: " << Path::getArgv0() << std::endl;
  std::cout << "program directory: " << Path::getProgramDirectory() << std::endl;
  std::cout << "program file name: " << Path::getProgramFileName() << std::endl;
  std::cout << "absolute path to './res/model/teapot.obj': "
            << Path::getAbsolutePathTo("./res/model/teapot.obj") << std::endl;
  std::cout << "absolute path to '/usr/local/bin/bash': "
            << Path::getAbsolutePathTo("/usr/local/bin/bash") << std::endl;
  std::cout << "file name of './res/model/teapot.obj': "
            << Path::getFileName("./res/model/teapot.obj") << std::endl;
}
