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

## Install
Clone repository to your project and add following to CMakeLists.txt:
```
ADD_SUBDIRECTORY(StateSmurf)
TARGET_LINK_LIBRARIES(<target> PUBLIC DiagramSmurfLib TransitionSmurfLib)
```

## Usage
Example usage of StateSmurf framework is described in [SmurfExampleApp](https://github.com/Melky-Phoe/StateSmurf/tree/master/SmurfExampleApp)


#### Integration Steps
Code:
1. Initialize state diagram using DiagramSmurf.
    1. Create vertexes.
    2. Add starting vertexes
    3. Add edges between vertexes
2. Pass this diagram to StateTransition class
3. Use StateTransition::goToState to change states according to system behavior

Automated evaluating:
1. Create scenarios.json file containing environment setup and arguments for each tested scenario 
2. Use SmurfTestingScripts CompareScenarios.py with --create-etalons option to create etalons
3. Verify etalons. Check if all transitions were successful, runs were ended as expected etc.
   - Etalons should remain same for a working system
4. Add CompareScenarios.py to test set.