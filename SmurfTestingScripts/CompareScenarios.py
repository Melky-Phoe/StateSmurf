import os.path
import time
import signal
import subprocess
from pathlib import Path
import sys
import json
import argparse

# Time in second to timeout if timeout argument is not set
default_timeout = 5*60
# Time after SIGINT to send SIGKILL
kill_timeout = 10


def run_commands(key):
    for action in scenario_json[key]:
        if action != "":
            return_code = os.system(action)
            print("'" + action + "' ended with exit code: ", return_code)
            if return_code > 0:
                return False
    return True


def setup():
    if not run_commands("setup"):
        print("Operation unsuccessful, shutting down testing script")
        cleanup()
        exit(1)


def tidy_up():
    if not run_commands("between_runs"):
        print("Tidy-up operation unsuccessful, following test might be unsuccessful")
        exit(1)


def run_scenarios():
    tests_passed = True
    for scenario in scenario_json["scenarios"]:
        print("Running test: ", scenario["name"], " .....")
        process = subprocess.Popen(create_command_string(scenario), shell=True, cwd=workDir, preexec_fn=os.setsid)
        try:
            if "timeout" in scenario.keys():
                process.wait(timeout=scenario["timeout"])
            else:
                # setting default timeout to 5 minutes
                process.wait(timeout=default_timeout)
        except subprocess.TimeoutExpired:
            print("Timed out")
            os.killpg(os.getpgid(process.pid), signal.SIGTERM)
            time.sleep(kill_timeout)
            os.killpg(os.getpgid(process.pid), signal.SIGKILL)
        print("..... Done")

        if not aggregate_circuits_and_compare(scenario["name"]):
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
    for argument in scenario["arguments"]:
        command += argument + " " + str(scenario["arguments"][argument]) + " "
    if args.create_etalons:
        target = os.path.join(etalons_dir, scenario["name"] + ".log")
        command += "> " + str(target)
    else:
        target = os.path.join(output_dir, scenario["name"] + ".log")
        command += "> " + str(target)
    return command


def aggregate_circuits_and_compare(filename: str) -> bool:
    if args.create_etalons:
        file = os.path.join(etalons_dir, filename + ".log")
        target_file = os.path.join(aggregated_etalons_dir, filename)
        return_code = os.system(evaluator_bin_path + " --aggregate " + file + " --target " + target_file)
        if return_code > 0:
            print("WARNING: Couldn't aggregate etalon file: ", filename)
            return False
    else:
        etalon_file = os.path.join(aggregated_etalons_dir, filename)
        compare_file = os.path.join(output_dir, filename + ".log")
        target_file = os.path.join(aggregated_output_dir, filename)
        return_code = os.system(evaluator_bin_path + " --etalon " + etalon_file + " --compare " + compare_file +
                                " --target " + target_file)
        if return_code > 0:
            print("WARNING: test didn't pass: ", filename)
            return False
    return True


def cleanup():
    run_commands("cleanup")


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
    except json.decoder.JSONDecodeError as e:
        print("ERROR: raised exception while parsing json file")
        print(e)
        exit(1)

    workDir = args.scenario.rsplit('/', 1)[0]
    workDir = os.path.realpath(workDir)
    if args.output_dir == "":
        out_dir = workDir
    else:
        out_dir = os.path.realpath(args.output_dir)

    etalons_dir = os.path.join(out_dir, "etalons")
    aggregated_etalons_dir = os.path.join(out_dir, "aggregated_etalons")
    output_dir = os.path.join(out_dir, "output")
    aggregated_output_dir = os.path.join(out_dir, "aggregated_output")
    evaluator_output_dir = os.path.join(out_dir, "evaluator_output")

    Path(etalons_dir).mkdir(parents=True, exist_ok=True)
    Path(aggregated_etalons_dir).mkdir(parents=True, exist_ok=True)
    Path(output_dir).mkdir(parents=True, exist_ok=True)
    Path(aggregated_output_dir).mkdir(parents=True, exist_ok=True)
    Path(evaluator_output_dir).mkdir(parents=True, exist_ok=True)

    os.chdir(workDir)

    exit_code = 0
    setup()
    if not run_scenarios():
        exit_code = 2
        if args.create_etalons:
            print("WARNING: Creating etalons failed")
        else:
            print("WARNING: Some test have different transition logs, check \'" + evaluator_output_dir + "\' for output")
    if args.create_etalons:
        print("Raw transition etalons were created in:", etalons_dir)
        print("aggregated etalons were created in ", aggregated_etalons_dir)
    cleanup()

    exit(exit_code)
