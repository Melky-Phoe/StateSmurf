# SmurfEvaluator
Application serves for filtering and comparing TransitionSmurf application's logs.

SmurfEvaluator work on principle of Etalon -> file that contains one tested application's run, that is manually verified and assumed as correct.

All other runs of tested application are compared to this Etalon.

Logs are aggregated to circuits based on their state diagram. Circuit is set of vertexes, that can be repeated multiple times.  
This aggregation removes errors caused by delays in communication, high cpu usage, etc. 
Same circuits can have different numbers in different runs and only circuit's states are compared.

## Requirements

Following dependencies must be installed on the host system

- cmake [>= 3.20]
- C++20
- [cmlib](https://github.com/cmakelib/cmakelib)


## Build

```
mkdir -p _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j 8
```
## Usage
### Logs format

#### Creating Etalon:
First take .log file of tested application run. Verify if everything worked as expected.
Then use --aggregate option to create Aggregated-Etalon. Save this file for later comparison.
This file will be passed to Evaluator with --etalon option

#### Compared files
Other tested-application's runs .log files are passed with --compare option.
This file can contain multiple runs of tested application. File will be automatically aggregated to circuits and compared.
New aggregated file is saved to --target file and compare output is printed to stdout

## Run
#### Creating aggregated etalon: 
`./smurfEvaluator --aggregate <path> [--target <new_file_name>]`

#### Comparing runs:
`./smurfEvaluator --etalon <path_to_etalon> --compare <path_to_compared> [--target <new_file_name>]`
### arguments
- **-e | --etalon**: Path to EtalonAggregated file.
- **-c | --compare**: Path to .log file which we want to compare with etalon
- **-s | --save-aggregated**: If set, aggregated files will be saved


### Tests
- For building tests use cmake option -DBRINGAUTO_TESTS=ON 
- Start tests: `$ ./test/diagram_smurf_tests`

## Implementation
- Files are filtered using Boost library, only lines with [TransitionSmurf] are compared.
- Comparer compares every word of logs except timestamp
- State diagram used for finding circuits is built from [DiagramSmurf] logs on begin of .log file
- Circuits in diagrams are found by CircuitFinder using Johnson's algorithm for finding elementary circuits
- Algorithm only find unique elementary circuits, therefore in some application runs there are logged as StateTransition
instead of as "In circuit". This doesn't influence correct behavior.
