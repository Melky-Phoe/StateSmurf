# SmurfEvaluator
Application that compares TransitionSmurf logs in 2 .log files.

First file is etalon - log file, we assume is correct. Etalon must contain exactly one run of application.

Second file will be compared to Etalon, 
this file can contain multiple runs of application and each run will be compared separately

## Requirements

Following dependencies must be installed on the host system

- cmake [>= 3.20]
- C++20
- [cmlib](https://github.com/cmakelib/cmakelib)
- [cxxopts](https://github.com/jarro2783/cxxopts)

## Build

```
mkdir -p _build && cd _build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j 8
```

## Run
`./smurfEvaluator -e <path_to_etalon> -c <path_to_compared>`
### arguments
- **-e | --etalon**: path to etalon file.
- **-c | --compare**: path to file that will be compared.


### Tests
- For building tests use cmake option -DBRINGAUTO_TESTS=ON 
- Start tests: `$ ./test/diagram_smurf_tests`
