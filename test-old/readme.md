# Tests

You can create new tests under this directory. Similar to `/app` folder, you can add `.cpp` files
anywhere under this (or sub-) directory(ies). Make sure every `.cpp` file you are adding contains
a `main` function.

## Resource Files

It is always the case that you want to include some resource files in your build directory. 
We've implemented good infrastructures to aid this process. For example, you can type

``` c++
AssimpObject obj("./model/teapot.obj");
```

in `C++` files when you want to load this file, after you add a `CMakeLists.txt` in the same 
folder and inside it, type

``` cmake
copy_model("teapot.obj")
```

This command will copy the `teapot.obj` directly into your build directory's `model/` 
sub-folder. If you want some nested folder structure, you can add `DESTINATION` option in 
`copy_*` functions:

``` cmake
copy_model("teapot.obj" DESTINATION "my_models/my_teapot")
```

Then in `C++` you will need to type

``` c++
AssimpObject("./model/my_models/my_teapot/teapot.obj");
```

There are lots of this functions available, for example,

``` cmake
copy_image("head.bmp")
copy_shader("sh_sphere")
```

Note that for the `copy_shader` command you don't need to add file extension and it will
automatically copy both vertex shader (`.vert.glsl`) and fragment shader (`.frag.glsl`)
to the destination directory.

## Nash Viewer Application Tests

Just like the `.cpp` files in `/app`, you can follow this kind of template to create tests:

``` c++
#include <nash/nash.h> // Include the header file from nash

using namespace nash; // Use the namespace to avoid repeatedly adding `nash::`

int main(int argc, char * argv[]) {
  Nash::init(argc, argv); // Remember to initialize the library,

  // Do Anything

  Nash::shutdown(); // ...and also shutdown
}
```
