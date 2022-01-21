# TransitionSmurf
Library for moving on DiagramSmurf and logging transitions

## Requirements

Following dependencies must be installed on the host system

- cmake [>= 3.20]
- C++20
- [cmlib](https://github.com/cmakelib/cmakelib)
- DiagramSmurf library
    * must be in same directory as TransitionSmurf 

## Build

```
mkdir -p _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j 8
```

## Tests
- For building tests use -DBRINGAUTO_TESTS=ON
- Start tests: `$ ./test/transition_smurf_tests`

# Installation
- For installation provide -DBRINGAUTO_INSTALL switch to cmake
- use `$ make install` to create directory lib

## Usage
- TransitionSmurf library uses bringauto Logger library, that has to be inicialized before using.
- Example can be found in `./example/main.cpp` or in `StateSmurf/SmurfExampleApp/`
- Link library to your project:

  ####1. Add as subdirectory in CMakeLists.txt:
  ```
  ADD_SUBDIRECTORY("<Path_to_TransitionSmurf_directory>" "TransitionSmurf/")
  
  TARGET_LINK_LIBRARIES(<target> PUBLIC TransitionSmurfLib bringauto_logger::bringauto_logger)
  ```
  ####2. Library install:
     - copy lib directory to your project
     - in CMakeLists.txt add .so library:
  ```
  ADD_LIBRARY(StateTransitionLib SHARED IMPORTED)
  SET_PROPERTY(TARGET StateTransitionLib PROPERTY IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/lib/STransLib/libstateTransitionLib.so")
  TARGET_INCLUDE_DIRECTORIES(<target> PUBLIC CMAKE_CURRENT_SOURCE_DIR}/lib/STransLib/include")
  TARGET_LINK_LIBRARIES(<target> PUBLIC StateTransitionLib)
  ```


### Example
- example use of using state transition app is in main.cpp