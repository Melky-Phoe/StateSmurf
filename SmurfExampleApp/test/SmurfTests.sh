#!/bin/bash
cd "$(dirname "$0")"
evaluator_path="../../_build/SmurfEvaluator/smurf_evaluator"
python3 ../../SmurfTestingScripts/smurf_compare_scenarios.py --scenario smurf/scenarios.json --executable ../_build/smurf_example_app --evaluator $evaluator_path