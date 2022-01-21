# DiagramSmurf
Library for creating State diagrams and changing states

## Requirements

Following dependencies must be installed on the host system

- cmake [>= 3.20]
- C++20

## Build

```
mkdir -p _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j 8
```

### Tests
- For building tests use cmake option -DBRINGAUTO_TESTS=ON 
- Start tests: `$ ./test/diagram_smurf_tests`

# Installation
- For installation provide -DBRINGAUTO_INSTALL switch to cmake
- use `$ make install` to create directory lib

### Usage
- copy lib directory to your project
- in CMakeLists.txt use:
```
  ADD_LIBRARY(StateGraphLib SHARED IMPORTED)
  SET_PROPERTY(TARGET StateGraphLib PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/SGLib/libstateGraphLib.so")
  TARGET_INCLUDE_DIRECTORIES(<target> PUBLIC CMAKE_CURRENT_SOURCE_DIR}/lib/SGLib/include")
  TARGET_LINK_LIBRARIES(<target> PUBLIC StateGraphLib)
  ```

### Example
- example use of creating state graph and changing states is in example/main.cpp