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

## Usage
- TransitionSmurf library uses bringauto Logger library, that has to be inicialized before using.

Clone directory to your project and add as subdirectory in CMakeLists.txt:
  ```
  ADD_SUBDIRECTORY("<Path_to_TransitionSmurf_directory>")
  
  TARGET_LINK_LIBRARIES(<target> PUBLIC TransitionSmurfLib bringauto_logger::bringauto_logger)
  ```



### Example
- Example can be found in `./example/main.cpp` or in `StateSmurf/SmurfExampleApp/`