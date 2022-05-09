# Smurf Example App
C++ application showing possible usage of StateSmurf framework

### Requirements
- cmake [>= 3.20]
- C++20
- [cmlib](https://github.com/cmakelib/cmakelib)
- StateSmurf

### Build
```
mkdir -p _build && cd _build
cmake ../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=<state_smurf_install_path>
make -j 8
```
### Run
```
./smurfExampleApp --speed <int>
```
#### Arguments
- **--speed**: target speed. Each increment take 1 second => is duration of run.

### Test
Before testing, path to SmurfEvaluator must be set. This path is relative to this script. It is set to `StateSmurf/_build/SmurfEvaluator/smurf_evaluator` by default
```
bash test/SmurfTests.sh
```
This script is example of what can be put into CI tool

### State Diagram
State diagram on which SmurfExampleApp is working

<img src="StateDiagram.svg" width="350">