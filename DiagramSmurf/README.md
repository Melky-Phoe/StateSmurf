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

### Example
- Example use of creating state graph and changing states is in example/main.cpp
- To build Example use cmake option -DBRINGAUTO_SAMPLES=ON

## Usage
Initialization of state diagram:
1. Create class StateDiagram
2. Add vertexes.
3. Add starting vertexes
4. Add edges between vertexes

State diagram must have at least 1 starting vertex (function `setStartVertex(vertexName)`)  
DiagramSmurfs reserved names begin and end with double underscore (`__START__`) 
## Implementation
- Vertexes are shared_ptr saved in vector
- Transitions are defined by State Transition Matrix
