# NASH: Numerical/Analytical Spherical Harmonics

This project aims to create a complete picture of all Spherical Harmonics Rendering Pipelines -
from pre-calculation of spherical harmonics coefficient for meshes, environment maps and area
lights, coefficient file formatting, storing and loading, to advanced rendering involving
spherical harmonics as well as analytical area light lighting. Eventually this will be a
research project exploring various numerical/analytical methods, their efficiency, computation
complexity and visual effect.

The software infrastructure based on [nanogui](https://github.com/wjakob/nanogui) framework. We are
also using [assimp](https://github.com/assimp/assimp) for 3D object file loading. Make sure you have
[cmake](https://cmake.org) (>= 3.10) for building this project.

## Getting Started

To build this project, you need to first clone this repository recursively.

```
$ git clone --recursive https://github.com/liby99/nash.git
```

Then you can simply type

```
$ make
```

This will generate a directory namely `build/` and the executables will be stored respectively.

### Run Tests or Applications

For example, you can do this to start running one of the app:

```
$ ./build/test/infra/viewer_with_cube
```

Then you should be seeing one cube showing inside a window.

Or a simpler app:

```
$ ./build/app/sh_0000
```

You will see the program evaluating `y_m^l(\theta, \phi)` for `m = 0`, `l = 0`, `\theta = 0`, and
`\phi = 0`.

### Building Documents

Meanwhile, if you want to compile the documents (paper) involved in this project, you can type

```
$ make doc
```

to generate the pdfs under `pdf/`.

## Contributing

### Build Process

When you are actively developing this project, you should be staying in `build/` and do this:

```
$ cmake ..
$ make
```

This will make sure every time you build it only compiles the file that is needed. Of course if you
are not adding new files you won't need to do the `cmake ..` step.

### Building Executables

For each `.cpp` file inside `app/` or `test/` folder, we will generate an executable. When acquiring
the header file, you just need to add this line at the beginning of the `.cpp` file:

``` cpp
#include <nash/nash.h>
```

Make sure you are putting the applications inside `app/` folder and testing or adhoc executables in
`test/`. When you are building, you can disable building app or test by adding flags (suppose you
are in `build/` directory.)

```
$ cmake .. -DNASH_BUILD_TEST=OFF
```

or

```
$ cmake .. -DNASH_BUILD_APP=OFF
```

### Documents and Latex Files

If you are adding new `.tex` file in `doc/` you will need to add that target to `Makefile`. For
example, if you want `make doc` to compile `doc/demo.tex`, you will need to add `demo` to the
`DOC_TARGET` in the `Makefile`.
