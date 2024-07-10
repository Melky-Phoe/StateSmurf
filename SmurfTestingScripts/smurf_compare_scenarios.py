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
        elif command:
            print(f"\033[93m{command}\033[0m")
            return_code = os.system(command)
            if return_code > 0 and return_code != 15:
                print(f"\033[31m'{command}' ended with exit code: {return_code}\033[0m")
                commands_ok.value = 0


def setup():
    print(f"\033[96mSetting up environment for scenario file {args.scenario}\033[0m\n")
    run_commands(scenario_json["setup"])
    if not commands_ok.value:
        print("\033[31mOperation unsuccessful, shutting down testing script\033[0m")
        return False
    print("\n\033[96mSetup finished\033[0m")
    return True


def tidy_up():
    if len(scenario_json["between_runs"]):
        print("\n\033[96mTidying up in beteween scenarios .....\033[0m\n")
        run_commands(scenario_json["between_runs"])
        if not commands_ok.value:
            print("\033[31mTidy-up operation unsuccessful\033[0m")
            return False
    return True


def run_scenarios():
    tests_passed = True
    for scenario in scenario_json["scenarios"]:
        print(f"\n\033[92mRunning test: {scenario['name']} .....\033[0m\n")
        process = subprocess.Popen(create_command_string(scenario), shell=True, cwd=workDir, preexec_fn=os.setsid)
        if "actions" in scenario:
            multiprocessing.Process(target=run_commands, args=(scenario["actions"],)).start()
        try:
            timeout = scenario.get("timeout_s", default_timeout)
            process.wait(timeout=timeout)
        except subprocess.TimeoutExpired:
            print("\033[31mTest timed out, aborting .....\033[0m")
            os.killpg(os.getpgid(process.pid), signal.SIGTERM)
            time.sleep(kill_timeout)
            os.killpg(os.getpgid(process.pid), signal.SIGKILL)
            return False
        except KeyboardInterrupt:
            print("Terminating process")
            os.killpg(os.getpgid(process.pid), signal.SIGKILL)
            raise KeyboardInterrupt
        if commands_ok.value == 0:
            print("\033[31mAn error occurred in a test action\033[0m")
            return False
        print("\n\033[92m..... Done\033[0m")

        if not args.create_etalons:
            if not compare_output(scenario["name"]):
                tests_passed = False
        if not tidy_up():
            return False
    return tests_passed


def check_executable(path_to_executable) -> bool:
    if not os.path.isfile(path_to_executable):
        print(f"\033[31mERROR: {path_to_executable} binary doesn't exist\033[0m")
        return False

    if not os.access(path_to_executable, os.X_OK):
        print(f"\033[31mERROR: {path_to_executable} is not executable\033[0m")
        return False
    return True


def create_command_string(scenario: dict) -> str:
    command = executable_path + " " + " ".join(scenario.get("arguments", [])) + " "

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
        print(f"\033[31mWARNING: test didn't pass: {filename}\033[0m")
        return False
    return True


def cleanup():
    print("\033[96mCleaning up environment .....\033[0m")
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
    if "name" not in scenario and not isinstance(scenario["name"], str):
        raise Exception("Scenario name must be a string")
    if scenario["name"] in used_names:
        raise Exception(f"Scenario '{scenario['name']}' name is not unique")
    expected_keys = ["name", "timeout_s", "arguments", "actions"]
    for key in scenario:
        if key not in expected_keys:
            raise Exception(f"Unexpected key '{key}' found in scenario '{scenario['name']}'")
    if "timeout_s" in scenario and (not isinstance(scenario["timeout_s"], (int, float)) or scenario["timeout_s"] < 0):
            raise Exception(f"Scenario '{scenario['name']}' timeout must be a positive number")
    if "arguments" in scenario:
        if not isinstance(scenario["arguments"], list):
            raise Exception(f"Scenario '{scenario['name']}' arguments must be a list strings")
        for argument in scenario["arguments"]:
            if not isinstance(argument, str):
                raise Exception(f"Scenario '{scenario['name']}' arguments can only contain a list strings")
    if "actions" in scenario and not validate_list_of_cmd_strings(scenario["actions"]):
        raise Exception(f"Scenario '{scenario['name']}' actions can only contain nested lists of strings")


def validate_json():
    expected_keys = ["setup", "between_runs", "scenarios", "cleanup"]
    for key in scenario_json:
        if key not in expected_keys:
            raise Exception(f"Unexpected key '{key}' found in scenario file")
    for key_name in expected_keys:
        if key_name not in scenario_json:
            raise Exception(f"'{key_name}' key not found in scenario file")
    expected_keys.remove("scenarios")
    if not all(validate_list_of_cmd_strings(scenario_json[key_name]) for key_name in expected_keys):
        raise Exception(f"'{key_name}' key can only contain nested lists of strings")
    used_names = []
    for scenario in scenario_json["scenarios"]:
        validate_scenario(scenario, used_names)
        used_names.append(scenario["name"])


if __name__ == "__main__":
    commands_ok = multiprocessing.Value('i', 1)
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
        print(f"\033[31mERROR: File given by argument --scenario is not a valid file: {args.scenario}\033[0m")
        exit(1)

    try:
        with open(args.scenario, "r") as scenario_file:
            scenario_json = json.loads(scenario_file.read())
            validate_json()
    except json.decoder.JSONDecodeError as e:
        print("\033[31mERROR: raised exception while parsing json file\033[0m")
        print(e)
        exit(1)
    except Exception as e:
        print("\033[31mERROR: raised exception while validating json file\033[0m")
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
    if not setup():
        exit(1)
    try:
        if not run_scenarios():
            if args.create_etalons:
                print("\033[31mERROR: An unexpected error occured during tests\033[0m")
                exit_code = 1
            else:
                print(f"\n\033[31mWARNING: Some test have different transition logs, check '{evaluator_output_dir}' for output\033[0m")
                exit_code = 2
        if args.create_etalons and exit_code == 0:
            print(f"\n\033[96mEtalons were created in: {etalons_dir}\033[0m\n")
    finally:
        cleanup()
        exit(exit_code)
