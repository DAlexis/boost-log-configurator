# Boost log configurator `blconfig`

Small library and header that provides boost log configuration acceptable for me.

If you want to begin using canonical C++ monster `Boost.log`, this repository may help you.
Project builds with `CMake` so you can run it under windows too.

Project structure:
 - `libblconfig` contains one file static library with header that configures boost log for you
 - `sample` contains code demonstrating logging usage

You can use `blconfig` as a subproject of another project using standard cmake's `add_subdirectory()`
interface.
