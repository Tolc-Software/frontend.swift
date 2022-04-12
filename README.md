![Ubuntu](https://github.com/Tolc-Software/frontend.swift/workflows/Ubuntu/badge.svg) ![MacOS](https://github.com/Tolc-Software/frontend.swift/workflows/MacOS/badge.svg) ![Windows](https://github.com/Tolc-Software/frontend.swift/actions/workflows/windows.yml/badge.svg) ![Windows-debug-deploy](https://github.com/Tolc-Software/frontend.swift/actions/workflows/windows-debug-deploy.yml/badge.svg)

***NOTE: This is a work in progress. It will be added to Tolc in due time.***

# frontend.swift #

`frontend.swift` is a library that takes an AST defined by [`IR::IR`](https://github.com/Tolc-Software/IntermediateRepresentation) and writes bindings to `Objective-C` and `Swift`.

`frontend.swift` is a part of the [`Tolc`](https://github.com/Tolc-Software/tolc) project.

## Building ##

`frontend.swift` can be built on Linux, MacOS, and Windows. See the [CI pipelines for more platform specific information.](./.github/workflows/).

Here is a general overview:

Requirements:

* CMake
* python3
* conan
* clang

Configure the project:

```shell
$ cmake -S. -Bbuild -G Ninja -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DCMAKE_BUILD_TYPE="Debug" -DENABLE_TESTING=ON -DENABLE_PACKAGING=ON
```

Build `tolc`:

```shell
$ cmake --build build
```

Test with `ctest`:

```shell
$ cd build
$ ctest --output-on-failure
```

Install with `CPack`:

```shell
$ cpack -G TGZ --config build/CPackConfig.cmake
```
