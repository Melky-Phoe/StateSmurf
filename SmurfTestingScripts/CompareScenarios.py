import os.path
import subprocess
from pathlib import Path
import sys
import json
import argparse


def run_commands(key):
    for action in scenario_json[key]:
        return_code = os.system(action)
        print("'" + action + "' ended with exit code: ", return_code)
        if return_code > 0:
            return False
    return True


def set_up():
    if not run_commands("prestart"):
        print("Operation unsuccessful, shutting down testing script")
        tear_down()
        exit(1)

    if not args.create_etalons:
        init_evaluator()


def tidy_up():
    if not run_commands("betweenRuns"):
        print("Tidy-up operation unsuccessful, following test might be unsuccessful")
        exit(1)


def run_scenarios():
    for scenario in scenario_json["scenarios"]:
        print("Running test: ", scenario["name"], " .....")
        process = subprocess.Popen(create_command_string(scenario), shell=True, cwd=workDir)
        try:
            if "timeout" in scenario.keys():
                process.wait(timeout=scenario["timeout"])
            else:
                # setting default timeout to 5 minutes
                process.wait(timeout=5 * 60)
        except subprocess.TimeoutExpired:
            print("Timed out")
            process.kill()
        print("..... Done")
        tidy_up()


def check_executable(path_to_executable) -> bool:
    if not os.path.isfile(path_to_executable):
        print("ERROR: ", path_to_executable, " binary doesn't exist")
        return False

    if not os.access(path_to_executable, os.X_OK):
        print("ERROR: ", path_to_executable, "  is not executable")
        return False
    return True


def compare_outputs() -> bool:
    tests_passed = True
    for scenario in scenario_json["scenarios"]:
        etalon_file = os.path.join("etalons", scenario["name"] + ".log")
        compared_file = os.path.join("output", scenario["name"] + ".log")
        return_code = os.system(evaluator_bin_path + " --etalon " + etalon_file +
                                " --compare " + compared_file +
                                " > compare_out/" + scenario["name"])
        if return_code > 0:
            tests_passed = False
            print("WARNING: test didn't pass: ", scenario["name"])
    return tests_passed


def create_command_string(scenario: dict) -> str:
    command = executable_path + " "
    for argument in scenario["arguments"]:
        command += argument + " " + str(scenario["arguments"][argument]) + " "
    if args.create_etalons:
        command += "> etalons/" + scenario["name"] + ".log"
    else:
        command += "> output/" + scenario["name"] + ".log"
    return command


def tear_down():
    run_commands("tearDown")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--scenario", type=str, required=True, help="Path to scenario.json file")
    parser.add_argument("-e", "--executable", type=str, required=True, help="Path to executable")
    parser.add_argument("-E", "--evaluator", type=str, default="", help="Path to SmurfEvaluator binary")
    parser.add_argument("-C", "--create-etalons", dest="create_etalons", action="store_true",
                        help="Creates Etalon files and ends program")

    args = parser.parse_args()

    if not check_executable(args.executable):
        exit(1)
    if not args.create_etalons:
        if args.evaluator == "":
            print("Path to SmurfEvaluator executable needed, pass in --evaluator argument ")
            exit(2)
        if not check_executable(args.evaluator):
            exit(1)

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

    evaluator_bin_path = args.evaluator
    executable_path = args.executable
    workDir = args.file.rsplit('/', 1)[0]
    workDir = os.path.realpath(workDir)
    os.chdir(workDir)

    Path("./etalons/").mkdir(parents=True, exist_ok=True)
    Path("./output/").mkdir(parents=True, exist_ok=True)
    Path("./compare_out/").mkdir(parents=True, exist_ok=True)

    exit_code = 0
    set_up()
    run_scenarios()
    if not args.create_etalons:
        if not compare_outputs():
            exit_code = 2
            print("WARNING: Some test have different transition logs, check \'compare_out/\' for output")

    tear_down()

    exit(exit_code)
