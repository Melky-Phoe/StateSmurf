import os.path
import subprocess
from pathlib import Path
import sys
import json
import argparse


def set_up():
    if not args.create_etalons:
        init_evaluator()

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
            if scenario.has_key["timeout"]:
                process.wait(timeout=scenario["timeout"])
            else:
                # setting default timeout to 5 minutes
                process.wait(timeout=5 * 60)
        except subprocess.TimeoutExpired:
            print('Timed out - killing', process.pid)
            process.kill()
        print("Done")
        tidy_up()


def init_evaluator():
    if not os.path.isfile(evaluator_path):
        Path(evaluator_path.rsplit(('/', 1)[0]).split('/')).mkdir(parents=True, exist_ok=True)
        exit_code = os.system("cmake .. -DCMAKE_BUILD_TYPE=Debug && make -j 8")
        print("'", action, "' ended with exit code: ", exit_code)
        if not exit_code == 0:
            print("Operation unsuccessful, shutting down testing script")
            tear_down()
    if not os.access(evaluator_path, os.X_OK):
        print("ERROR: SmurfEvaluator is not executable")
        tear_down()


def compare_outputs() -> bool:
    tests_passed = True
    for scenario in scenario_json["scenarios"]:
        etalon_file = os.path.join("etalons", scenario["name"] + ".log")
        compared_file = os.path.join("output", scenario["name"] + ".log")
        exit_code = os.system(evaluator_path + " --etalon " + etalon_file +
                              " --compare " + compared_file +
                              " > compare_out/" + scenario["name"])
        if exit_code > 0:
            tests_passed = False
            print("WARNING: test didn't pass: ", scenario["name"])
    return tests_passed


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
    Path("./compare_out/").mkdir(parents=True, exist_ok=True)

    evaluator_path = os.path.dirname(os.path.realpath(__file__)).rsplit('/', 1)[0]
    evaluator_path = os.path.join(evaluator_path, "SmurfEvaluator", "_build", "smurfEvaluator")

    exit_code = 0

    set_up()
    run_scenarios()
    if not args.create_etalons:
        if not compare_outputs():
            exit_code = 2
            print("WARNING: Some test have different transition logs, check \'compare_out/\' for output")

    tear_down()

    exit(exit_code)
