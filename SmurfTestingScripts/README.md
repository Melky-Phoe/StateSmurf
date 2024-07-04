# Smurf Testing Script
Python script used for automated StateSmurf testing.

# CompareScenarios
This script runs test-scenarios based on a scenario json file and compares each run's State transitions using [SmurfEvaluator](https://github.com/Melky-Phoe/StateSmurf/tree/master/SmurfEvaluator).  
Test is successful when SmurfEvaluator is successful. Script doesn't consider exit code of application run.


## Usage
The scenario json file contains commands that will run before, in between each application run, and after all runs are completed.
Application runs contain the name of a test, timeout in seconds, program arguments and commands to be run in parallel.

All paths used in the scenario json are relative to that file.

**Created directories:** 
* etalons: created when -c option is used; files with raw .log files
* output: raw .log output of test-runs
* aggregated_output: aggregated output of test-runs
* evaluator_output: SmurfEvaluator output containing the comparison between an etalon and an output

## Run
```
python3 CompareScenarios.py --scenario <path> --executable <path> --evaluator <path> [--output <path> --create-etalons]
```  
First run must be run with the --create-etalons flag! Etalons aren't created automatically when not found,
because they require human approval.
### Arguments:
- **-s | --scenario**: Path to scenario json file containing run scenarios.
- **-e | --executable**: Path to executable of tested application.
- **--evaluator**: Path to SmurfEvaluator executable.
- **-c | --create-etalons**: Switch to create etalons.
- **-o | --output**: Path to directory, where all output directories are created

#### Exit codes:
- 0 = Success, everything worked
- 1 = Error
- 2 = Some files are different from etalon.
### Scenarios

#### keys:
- setup : list of commands, that are run once on the beginning
- between_runs : list of commands, that are run in between each test scenario
- cleanup : list of commands, that are run once at the end of all tests
- scenarios : set of testing scenarios containing:
  - name : test name; must be unique
  - timeout (optional) : time in seconds (can be decimal) after which the run is terminated (SIGTERM). 10 seconds after SIGTERM, SIGKILL is sent 
  - arguments (optional) : list of program arguments for the tested executable
  - actions (optional) : list of commands, that are run in parallel with the tested executable

note: all keys that accept a list of commands can have nested lists; nesting will run the commands in a new thread
  
#### Example
```json
{
  "setup" : [
    "docker-compose --file=./docker-compose.yml up -d", 
    "echo message > msg.txt"
  ],
  "between_runs" : [
    "docker-compose --file=./docker-compose.yml restart"
  ],
  "scenarios" : [
    {
      "name" : "test1",
      "timeout" : 70,
      "arguments" : [
        "--foo", "dir/file", "--bar"
      ],
      "actions": [
        "sleep 2",
        "python3 test1.py"
      ]
    },
    {
      "name" : "test2",
      "timeout" : 65,
      "arguments" : [
        "--foo", "dir/file2"
      ],
      "actions": [
        "sleep 2",
        "python3 test2.py"
      ]
    }
  ],
  "cleanup" : [
    "docker-compose --file=./etna/docker-compose.yml down"
  ]
}
```
