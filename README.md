# StateSmurf
DiagramSmurf is a framework for Integration testing of systems based on Finite State Machine.

## Components
#### DiagramSmurf
- Library for creating state diagrams and moving around them
#### TransitionSmurf
- API for controlling state transitions in state diagram and logging them. Implemented on DiagramSmurf
#### SmurfEvaluator
- Application for comparing StateTransitions of different runs
#### SmurfTestingScript
- Python scripts for automated testing and comparing

## Build
```
mkdir -p _build && cd _build
cmake .. -DCMLIB_DIR=<path_to_cmlib_dir> -DCMAKE_BUILD_TYPE=Release
make -j 8
```
## Tests
- For building tests use `-DBRINGAUTO_TESTS=ON`
- Run test with command `$ ctest`
## Install
To install, use CMake option `-DBRINGAUTO_INSTALL=ON`  
and set install prefix `-DCMAKE_INSTALL_PREFIX=<install_path>`
```
mkdir -p _build && cd _build
cmake .. -DBRINGAUTO_INSTALL=ON -DCMAKE_INSTALL_PREFIX=<install_path> -DCMLIB_DIR=<path_to_cmlib_dir> -DCMAKE_BUILD_TYPE=Release
make install
```
To generate package, use CMake option `-DBRINGAUTO_PACKAGE=ON` and run command `$ cpack`


After Install, smurf_evaluator binary will be generated in <install_prefix>/bin/,
interfaces of DiagramSmurf and TransitionSmurf in <install_prefix>/include/state_smurf/ and .so files in <install_prefix>/lib/
## Usage
Generate package with cpack command, install the package and include:
```CMake
FIND_PACKAGE(state_smurf)
TARGET_LINK_LIBRARIES(<target> PUBLIC diagram_smurf_lib transition_smurf_lib)
```
Example usage of StateSmurf framework is described in [SmurfExampleApp](https://github.com/Melky-Phoe/StateSmurf/tree/master/SmurfExampleApp)


#### Integration Steps
Code:
1. Initialize state diagram using DiagramSmurf.
    1. Create vertexes.
    2. Set starting vertexes
    3. Add edges between vertexes
2. Pass this diagram to StateTransition class
3. Use StateTransition::goToState to change states according to system behavior

Automated evaluating:
1. Create scenarios.json file containing environment setup and arguments for each tested scenario 
2. Use SmurfTestingScripts CompareScenarios.py with --create-etalons option to create etalons
3. Verify etalons. Check if all transitions were successful, runs were ended as expected etc.
   - Etalons should remain same for a working system
4. Add CompareScenarios.py to test set.