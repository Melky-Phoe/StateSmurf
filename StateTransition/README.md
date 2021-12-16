# State Transition App
Library for moving on StateGraph

## Requirements

Following dependencies must be installed on the host system

- cmake [>= 3.20]
- C++20
- [cmlib](https://github.com/cmakelib/cmakelib)
- StateGraph lib

## Build

```
mkdir -p _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j 8
```

### Tests
- For building tests use -DBRINGAUTO_TESTS=ON
- Start tests: `$ ./test/state_transition_tests`

# Installation
- For installation provide -DBRINGAUTO_INSTALL switch to cmake
- use `$ make install` to create directory lib

### Usage
- copy lib directory to your project
- in CMakeLists.txt use:
```
  ADD_LIBRARY(StateTransitionLib SHARED IMPORTED)
  SET_PROPERTY(TARGET StateTransitionLib PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/STransLib/libstateTransitionLib.so")
  TARGET_INCLUDE_DIRECTORIES(<target> PUBLIC CMAKE_CURRENT_SOURCE_DIR}/lib/STransLib/include")
  TARGET_LINK_LIBRARIES(<target> PUBLIC StateTransitionLib)
  ```
- StateTransition library uses bringauto Logger library, that has to be inicialized before using.

### Example
- example use of using state transition app is in main.cpp