# Note

Notice how we are using `CMakeLists.txt` in the current directory. Inside this
file we only have one line:

``` cmake
file(
  COPY "${CMAKE_CURRENT_SOURCE_DIR}/res/model/teapot.obj"
  DESTINATION "${temp_dir}/res/"
)
```

This says we are going to copy the root directory (`CMAKE_CURRENT_SOURCE_DIR`)'s
`/res/model/teapot.obj` to the target locations' `res/` folder. Then inside our
`.cpp` file we can write loading path to be

``` c++
AssimpObject teapot("./res/teapot.obj");
```

Then you can start the program from anywhere you want (since we are using absolute path when loading
the file.)
