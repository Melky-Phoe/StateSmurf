# Smurf Testing Script
Python script used for automated StateSmurf testing.

# CompareScenarios
Script runs test-scenarios based on scenario.json file and compares each run's State transitions using [SmurfEvaluator](https://github.com/Melky-Phoe/StateSmurf/tree/master/SmurfEvaluator).  
Test is successful when SmurfEvaluator is successful. Script doesn't consider exit code of application run.


## Usage
Scenario.json file contains commands that will be run before, after and in between each application run.
Application runs contains name of test, timeout in seconds, and program arguments.

All paths used in scenario.json are relative to this file.

**Created directories:** 
* etalons: created when -c option is used, files with raw .log files
* output: raw .log output of test-runs
* aggregated_output: aggregated output of test-runs
* evaluator_output: SmurfEvaluator compare output containing differences between etalon and output

## Run
```
python3 CompareScenarios.py --scenario <path> --executable <path> --evaluator <path> [--output <path> --create-etalons]
```  
first run must  have option --create-etalons! Etalons aren't created automatically when not found,
because they demand human-approve.
### Arguments:
- **-s | --scenario**: Path to scenario.json file containing run scenarios.
- **-e | --executable**: Path to executable of tested application.
- **--evaluator**: Path SmurfEvaluator executable.
- **-c | --create-etalons**: Switch to create etalons.
- **-o | --output**: Path to directory, where all output directories are created

#### Exit codes:
- 0 = Success, everything worked
- 1 = Error
- 2 = Some files are different from etalon.
### Scenarios

#### keys:
- setup : set of commands, that are run once on the beginning
- between_runs : set of commands, that are run in between each test scenario
- cleanup : set of commands, that are run once in the end of tests
- scenarios : set of testing scenarios containing:
  - name : test name
  - timeout : time in seconds after which the run is terminated (SIGTERM). 10 seconds after SIGTERM, SIGKILL is sent 
  - arguments : set of program arguments. Switch without value has to be an empty string!
  
#### Example
```json
{
  "setup" : [
    "docker-compose --file=./docker-compose.yml up -d", 
    "echo message > msg.txt"
  ],
  "between_runs" : [ "docker-compose --file=./docker-compose.yml restart" ],
  "scenarios" : [
    {
      "name" : "test1",
      "timeout" : 70,
      "arguments" : {
        "--foo" : "dir/file",
        "--bar" : ""
      }
    },
    {
      "name" : "test2",
      "timeout" : 65,
      "arguments" : {
        "--foo" : "dir/file2"
      }
    }
  ],
  "cleanup" : [
    "docker-compose --file=./etna/docker-compose.yml down"
  ]
}
```
