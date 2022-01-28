import os.path
import subprocess
from pathlib import Path
import sys
import json
import argparse


def set_up():
    for action in scenario_json["prestart"]:
        exit_code = os.system(action)
        print("'", action, "' ended with exit code: ", exit_code)
        if not exit_code == 0:
            print("Operation unsuccessful, shutting down testing script")
            tear_down()


def tidy_up():
    for action in scenario_json["betweenRuns"]:
        exit_code = os.system(action)
        print("'", action, "' ended with exit code: ", exit_code)
        if not exit_code == 0:
            print("Tidy-up operation unsuccessful, following test might be unsuccessful")


def run_scenarios():
    for scenario in scenario_json["scenarios"]:
        print("Running test: ", scenario["name"])

        # os.system(create_command_string(scenario))
        process = subprocess.Popen(create_command_string(scenario), shell=True, cwd=workDir)
        try:
            print('Running in process', process.pid)
            process.wait(timeout=1 * 60)
        except subprocess.TimeoutExpired:
            print('Timed out - killing', process.pid)
            process.kill()
        print("Done")
        tidyup()


def compare_outputs():
    for scenario in scenario_json["scenarios"]:
        print("Running test: ", scenario["name"])


def create_command_string(scenario: dict) -> str:
    command = scenario["executable"] + " "
    for argument in scenario["arguments"]:
        command += argument + " " + str(scenario["arguments"][argument]) + " "
    if args.create_etalons:
        command += "> etalons/" + scenario["name"] + ".log"
    else:
        command += "> output/" + scenario["name"] + ".log"
    return command


def tear_down():
    for action in scenario_json["tearDown"]:
        print(action)
        exit_code = os.system(action)
        print("'", action, "' ended with exit code: ", exit_code)
    exit(1)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", type=str, required=True, help="Path to scenario.json file")
    parser.add_argument("-E", "--evaluator-path", type=str, required=False, help="Path to SmurfEvaluator executable")
    parser.add_argument("-C", "--create-etalons", dest="create_etalons", action="store_true",
                        help="Creates Etalon files and ends program")

    args = parser.parse_args()

    if not os.path.isfile(args.file):
        print("ERROR: File given by argument --file is not a valid file: " + args.file)
        exit(1)

    scenario_file = open(args.file, "r")

    try:
        scenario_json = json.loads(scenario_file.read())
    except json.decoder.JSONDecodeError as e:
        print("ERROR: raised exception while parsing json file")
        print(e)
        exit(1)

    workDir = args.file.rsplit('/', 1)[0]
    os.chdir(workDir)
    Path("./etalons/").mkdir(parents=True, exist_ok=True)
    Path("./output/").mkdir(parents=True, exist_ok=True)
    set_up()
    run_scenarios()
    if not args.create_etalons:
        compare_outputs()

    tear_down()
