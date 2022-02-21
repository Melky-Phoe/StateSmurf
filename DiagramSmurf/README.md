# DiagramSmurf
Library for creating State diagrams and changing states.
Diagrams are defined by set of vertexes, starting vertexes and transitions.

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

## Usage

Clone directory to your project and add as subdirectory in CMakeLists.txt:
  ```
  ADD_SUBDIRECTORY("<Path_to_DiagramSmurf_directory>")
  
  TARGET_LINK_LIBRARIES(<target> PUBLIC DiagramSmurfLib)
  ```

### Example
- Example use of creating state graph and changing states is in example/main.cpp
- To build Example use cmake option -DBRINGAUTO_SAMPLES=ON

## Implementation
- Vertexes are shared_ptr saved in vector
- Transitions are defined by State Transition Matrix