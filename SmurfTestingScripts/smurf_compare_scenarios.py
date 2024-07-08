import os.path
import time
import signal
import subprocess
from pathlib import Path
import json
import argparse
import multiprocessing

# Time in second to timeout if timeout argument is not set
default_timeout = 5*60
# Time after SIGINT to send SIGKILL
kill_timeout = 10


def run_commands(command_list):
    for command in command_list:
        if isinstance(command, list):
            multiprocessing.Process(target=run_commands, args=(command,)).start()
        elif command != "":
            return_code = os.system(command)
            print("'" + command + "' ended with exit code: ", return_code)
            if return_code > 0 and return_code != 15:
                return False
    return True


def setup():
    if not run_commands(scenario_json["setup"]):
        print("Operation unsuccessful, shutting down testing script")
        cleanup()
        exit(1)


def tidy_up():
    if not run_commands(scenario_json["between_runs"]):
        print("Tidy-up operation unsuccessful, following test might be unsuccessful")
        exit(1)


def run_scenarios():
    tests_passed = True
    for scenario in scenario_json["scenarios"]:
        print("Running test: ", scenario["name"], " .....")
        process = subprocess.Popen(create_command_string(scenario), shell=True, cwd=workDir, preexec_fn=os.setsid)
        if "actions" in scenario.keys():
            multiprocessing.Process(target=run_commands, args=(scenario["actions"],)).start()
        try:
            if "timeout_s" in scenario.keys():
                process.wait(timeout=scenario["timeout_s"])
            else:
                # setting default timeout to 5 minutes
                process.wait(timeout=default_timeout)
        except subprocess.TimeoutExpired:
            print("Timed out")
            os.killpg(os.getpgid(process.pid), signal.SIGTERM)
            time.sleep(kill_timeout)
            os.killpg(os.getpgid(process.pid), signal.SIGKILL)
        print("..... Done")

        if not args.create_etalons:
            if not compare_output(scenario["name"]):
                tests_passed = False
        tidy_up()
    return tests_passed


def check_executable(path_to_executable) -> bool:
    if not os.path.isfile(path_to_executable):
        print("ERROR: ", path_to_executable, " binary doesn't exist")
        return False

    if not os.access(path_to_executable, os.X_OK):
        print("ERROR: ", path_to_executable, "  is not executable")
        return False
    return True


def create_command_string(scenario: dict) -> str:
    command = executable_path + " "
    if "arguments" in scenario.keys():
        for argument in scenario["arguments"]:
            command += argument + " "

    if args.create_etalons:
        target = os.path.join(etalons_dir, scenario["name"] + ".log")
        command += "2>&1 > " + str(target)
    else:
        target = os.path.join(output_dir, scenario["name"] + ".log")
        command += "2>&1 > " + str(target)
    return command


def compare_output(filename: str) -> bool:
    etalon_file = os.path.join(etalons_dir, filename + ".log")
    compare_file = os.path.join(output_dir, filename + ".log")
    aggregate_dir = os.path.join(aggregated_output_dir, filename)
    output_file = os.path.join(evaluator_output_dir, filename)
    return_code = os.system(evaluator_bin_path + " --etalon " + etalon_file + " --compare " + compare_file +
                            " --save-aggregated " + aggregate_dir + " > " + output_file)
    if return_code > 0:
        print("WARNING: test didn't pass: ", filename)
        return False
    return True


def cleanup():
    run_commands(scenario_json["cleanup"])


def validate_list_of_cmd_strings(cmd_list) -> bool:
    for cmd in cmd_list:
        if isinstance(cmd, list):
            if not validate_list_of_cmd_strings(cmd):
                return False
        elif not isinstance(cmd, str):
            return False
    return True


def validate_scenario(scenario, used_names):
    if "name" not in scenario.keys() and not isinstance(scenario["name"], str):
        raise Exception("Scenario name must be a string")
    if scenario["name"] in used_names:
        raise Exception(f"Scenario '{scenario['name']}' name is not unique")
    expected_keys = ["name", "timeout_s", "arguments", "actions"]
    for key in scenario.keys():
        if key not in expected_keys:
            raise Exception(f"Unexpected key '{key}' found in scenario '{scenario['name']}'")
    if "timeout_s" in scenario.keys():
        if not isinstance(scenario["timeout_s"], (int, float)) or scenario["timeout_s"] < 0:
            raise Exception(f"Scenario '{scenario['name']}' timeout must be a positive number")
    if "arguments" in scenario.keys():
        if not isinstance(scenario["arguments"], list):
            raise Exception(f"Scenario '{scenario['name']}' arguments must be a list strings")
        for argument in scenario["arguments"]:
            if not isinstance(argument, str):
                raise Exception(f"Scenario '{scenario['name']}' arguments can only contain a list strings")
    if "actions" in scenario.keys() and not validate_list_of_cmd_strings(scenario["actions"]):
        raise Exception(f"Scenario '{scenario['name']}' actions can only contain nested lists of strings")


def validate_json():
    expected_keys = ["setup", "between_runs", "scenarios", "cleanup"]
    for key in scenario_json.keys():
        if key not in expected_keys:
            raise Exception(f"Unexpected key '{key}' found in scenario file")
    for key_name in expected_keys:
        if key_name not in scenario_json.keys():
            raise Exception(f"'{key_name}' key not found in scenario file")
    expected_keys.remove("scenarios")
    for key_name in expected_keys:
        if not validate_list_of_cmd_strings(scenario_json[key_name]):
            raise Exception(f"'{key_name}' key can only contain nested lists of strings")
    used_names = []
    for scenario in scenario_json["scenarios"]:
        validate_scenario(scenario, used_names)
        used_names.append(scenario["name"])


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", "--scenario", type=str, required=True, help="Path to scenario.json file")
    parser.add_argument("-e", "--executable", type=str, required=True, help="Path to executable")
    parser.add_argument("--evaluator", type=str, required=True, help="Path to SmurfEvaluator binary")
    parser.add_argument("-c", "--create-etalons", dest="create_etalons", action="store_true",
                        help="Creates Etalon files and ends program")
    parser.add_argument("-o", "--output-dir", type=str, dest="output_dir", default="", help="Path to output directory")

    args = parser.parse_args()

    evaluator_bin_path = os.path.realpath(args.evaluator)
    executable_path = os.path.realpath(args.executable)

    if not check_executable(executable_path):
        exit(1)

    if not check_executable(evaluator_bin_path):
        exit(1)

    if not os.path.isfile(args.scenario):
        print("ERROR: File given by argument --scenario is not a valid file: " + args.scenario)
        exit(1)

    scenario_file = open(args.scenario, "r")

    try:
        scenario_json = json.loads(scenario_file.read())
        validate_json()
    except json.decoder.JSONDecodeError as e:
        print("ERROR: raised exception while parsing json file")
        print(e)
        exit(1)
    except Exception as e:
        print("ERROR: raised exception while validating json file")
        print(e)
        exit(1)

    workDir = os.path.realpath(os.path.dirname(args.scenario))
    if args.output_dir == "":
        out_dir = workDir
    else:
        out_dir = os.path.realpath(args.output_dir)

    etalons_dir = os.path.join(out_dir, "etalons")
    output_dir = os.path.join(out_dir, "output")
    aggregated_output_dir = os.path.join(out_dir, "aggregated_output")
    evaluator_output_dir = os.path.join(out_dir, "evaluator_output")

    if args.create_etalons:
        Path(etalons_dir).mkdir(parents=True, exist_ok=True)
    else:
        Path(output_dir).mkdir(parents=True, exist_ok=True)
        Path(aggregated_output_dir).mkdir(parents=True, exist_ok=True)
        Path(evaluator_output_dir).mkdir(parents=True, exist_ok=True)

    os.chdir(workDir)

    exit_code = 0
    setup()
    if not run_scenarios():
        exit_code = 2
        print("WARNING: Some test have different transition logs, check \'" + evaluator_output_dir + "\' for output")
    if args.create_etalons:
        print("Etalons were created in:", etalons_dir)
    cleanup()

    exit(exit_code)
