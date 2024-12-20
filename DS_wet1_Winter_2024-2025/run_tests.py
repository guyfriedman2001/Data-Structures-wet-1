

import os
import argparse
import subprocess


COMPILATION_FLAGS ="-std=c++11 -DNDEBUG -Wall"
TIMEOUT = 10


def run_test(exe_file, test_id, tests_dir):
    input_file = os.path.join(tests_dir, f"test{test_id}.in")
    expected_output_file = os.path.join(tests_dir, f"test{test_id}.out")
    result_file = os.path.join(tests_dir, f"test{test_id}.res")

    if not os.path.isfile(input_file):
        print(f"Input file for test {test_id} not found: {input_file}")
        return

    if not os.path.isfile(expected_output_file):
        print(f"Expected output file for test {test_id} not found: {expected_output_file}")
        return

    # Execute the compiled binary with a timeout
    command = f"{exe_file}"
    try:
        with open(input_file, "r") as stdin, open(result_file, "w") as stdout:
            subprocess.run(command, stdin=stdin, stdout=stdout, timeout=TIMEOUT, shell=True, check=True)
    except subprocess.TimeoutExpired:
        print(f"Test {test_id} Failed: Execution timed out after {TIMEOUT} seconds.")
        return
    except subprocess.CalledProcessError as e:
        print(f"Test {test_id} Failed: Command execution error. {e}")
        return

    # Check result against expected output
    if not os.path.isfile(result_file):
        print(f"Test {test_id} failed: result file not created")
        return

    with open(result_file, "r") as res, open(expected_output_file, "r") as expected:
        if res.read().strip() == expected.read().strip():
            print(f"Test {test_id} Passed")
            return True
        else:
            print(f"Test {test_id} Failed: Output does not match expected.")


def main():
    parser = argparse.ArgumentParser(description="Generate Tests.")
    parser.add_argument("--tests_dir", type=str, default="./tests/", help="Path to the dir with the tests to run (default: './tests/').")
    parser.add_argument("--code_dir", type=str, default="./", help="Path to the dir with the code to compile and test (default: './').")
    parser.add_argument("--compiler_path", type=str, default="g++", help="Path to the g++ compiler (default: 'g++').")
    parser.add_argument("--clean", action="store_true", help="Remove all .res files from the tests dir.")
    parser.add_argument("--abort_on_fail", action="store_true", help="Abort on first test that fails.")
    parser.add_argument(
        "-t", "--tests", 
        type=int, 
        nargs="*",
        help="List of test IDs to run (default: run all tests).", 
        default=None
    )
    args = parser.parse_args()

    source_files = os.path.join(args.code_dir, "*.cpp")
    exe_file = os.path.join(args.code_dir, "main.out")
    compilation_command = "g++ {} -o {} {}".format(COMPILATION_FLAGS, exe_file, source_files)
    if os.system(compilation_command) != 0:
        print(f"Compilation failed. Command executed: {compilation_command}")
        return -1
    if not os.path.isfile(exe_file):
        print(f"Compilation succeeded, but the executable {exe_file} was not created.")
        return -1

    if args.clean:
        os.system(f"rm {os.path.join(args.tests_dir, '*.res')}")
        return 0

    # Get the list of tests to run
    if args.tests is None:
        tests = [int(f.split('test')[1].split('.in')[0]) for f in os.listdir(args.tests_dir) if f.endswith(".in")]
        tests = sorted(set(tests))
    else:
        tests = args.tests

    for test_id in tests:
        if not run_test(exe_file, test_id, args.tests_dir) and args.abort_on_fail:
            return 0
    
    return 0

if __name__ == "__main__":
    main()
