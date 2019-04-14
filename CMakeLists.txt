# Basic setups
cmake_minimum_required(VERSION 3.10)
project(nash)

# Options
option(NASH_BUILD_APP "Building application executables" ON)
option(NASH_BUILD_TEST "Building test executables" ON)
option(USE_OPEN_MP "Build use open mp" OFF)

# Global setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MACOSX_RPATH 1)
set(build_dir ${CMAKE_BINARY_DIR})
set(curr_dir ${CMAKE_CURRENT_LIST_DIR})
set(script_dir ${curr_dir}/script)
set(extern_dir ${curr_dir}/src/extern)
set(intern_dir ${curr_dir}/src/intern)
add_definitions(-DGL_SILENCE_DEPRECATION)

# Extra CMake Scripts
include("script/cmake/copy_res.cmake")
include("script/cmake/check_library.cmake")

# MinGW Support
if(MINGW)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas -Wno-unused-parameter -O1 -g \
    -Wno-int-in-bool-context -Wno-tautological-compare -Wno-maybe-uninitialized -Wno-return-type \
    -Wno-cast-function-type -Wno-implicit-fallthrough")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-unused-parameter -frtti -O1 -g \
    -Wno-int-in-bool-context -Wno-tautological-compare -Wno-class-memaccess \
    -Wno-maybe-uninitialized -Wno-return-type -Wno-cast-function-type -Wno-implicit-fallthrough")
endif()

# Include OpenMP
if(USE_OPEN_MP)
  find_package(OpenMP)
  if(OPENMP_FOUND)
    add_definitions(-DUSE_OPEN_MP)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()
endif()

# Include NanoGui
check_library("nanogui")
set(NANOGUI_BUILD_EXAMPLE OFF)
set(NANOGUI_BUILD_PYTHON OFF)
set(NANOGUI_BUILD_SHARED OFF)
add_subdirectory(lib/nanogui EXCLUDE_FROM_ALL)
include_directories(lib/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})
add_definitions(${NANOGUI_EXTRA_DEFS})

# Include Assimp
check_library("assimp")
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
set(ASSIMP_BUILD_OFF_IMPORTER TRUE)
set(ASSIMP_BUILD_PLY_IMPORTER TRUE)
# Not really sure why it is option here. But `set` doesn't work for the first time `cmake ..` runs.
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
option(ASSIMP_NO_EXPORT ON)
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_QT_VIEWER OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
add_subdirectory(lib/assimp EXCLUDE_FROM_ALL)
include_directories(lib/assimp/include)

# Include json
check_library("json")
add_subdirectory(lib/json EXCLUDE_FROM_ALL)
include_directories(lib/json/include)

# Include stb
# e.g. #include <stb_image.h>
check_library("stb")
include_directories(lib/stb/)

# Generate single header file using the script
set(generate_header ${script_dir}/generate_header.py)
execute_process(COMMAND python3 ${generate_header} nash ${extern_dir})
set(add_clang_format_ignore ${script_dir}/add_clang_format_ignore.py) # And prepend clang-format off
execute_process(COMMAND python3 ${add_clang_format_ignore} ${extern_dir}/nash/nash.h)

# Set Include Directory to Extern
include_directories(src/extern)

# Compile all the cpp files and link to `nash` library along with NanoGui
file(GLOB_RECURSE nash_sources ${intern_dir}/*.cpp ${extern_dir}/*.h)
set(nash ${nash_sources})
add_library(nash STATIC ${nash})
target_link_libraries(nash nanogui assimp nlohmann_json::nlohmann_json ${NANOGUI_EXTRA_LIBS})

# Compile all the executable files
if(NASH_BUILD_TEST)
  file(GLOB_RECURSE tests test/*.cpp)
  list(FILTER tests EXCLUDE REGEX "deprecated") # Ignore tests containing path `deprecated`
endif(NASH_BUILD_TEST)
if(NASH_BUILD_APP)
  file(GLOB_RECURSE apps app/*.cpp)
endif(NASH_BUILD_APP)
foreach(exec_source ${tests} ${apps})
  get_filename_component(source_dir ${exec_source} DIRECTORY)
  get_filename_component(source_name ${exec_source} NAME)
  string(REPLACE ".cpp" "" target_path ${exec_source})
  string(REPLACE "${CMAKE_CURRENT_LIST_DIR}/" "" target_path ${target_path})
  get_filename_component(target_dir ${target_path} DIRECTORY)
  get_filename_component(target_name ${target_path} NAME)
  add_executable(${target_name} ${exec_source})
  set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${target_dir})
  target_link_libraries(${target_name} nash)
  include("${target_dir}/CMakeLists.txt" OPTIONAL)
endforeach()
