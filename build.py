#!/usr/bin/env python3
import argparse
import subprocess
import shutil
import os
import sys

BUILD_DIR = ".build"

def clean():
    if os.path.exists(BUILD_DIR):
        shutil.rmtree(BUILD_DIR)
        print("Cleaned build directory.")
    else:
        print("Build directory does not exist.")

def build(debug=False):
    build_type = "Debug" if debug else "Release"
    try:
        subprocess.check_call(["cmake", "-S", ".", "-B", BUILD_DIR, f"-DCMAKE_BUILD_TYPE={build_type}"])
        subprocess.check_call(["cmake", "--build", BUILD_DIR])
        print(f"Build completed ({build_type} mode).")
    except subprocess.CalledProcessError as e:
        print("Build failed.", file=sys.stderr)
        sys.exit(e.returncode)

def run():
    exe_name = "expr_exe.exe" if os.name == 'nt' else "expr_exe"
    exe_path = os.path.join(BUILD_DIR, "bin", exe_name)
    if os.path.exists(exe_path):
        try:
            subprocess.check_call([exe_path])
        except subprocess.CalledProcessError as e:
            print("Executable returned an error.", e, file=sys.stderr)
            sys.exit(e.returncode)
    else:
        print("Executable not found. Please build the project first.")

def main():
    parser = argparse.ArgumentParser(description="Manage the ExpressionTree project: clean, build, or run.")
    parser.add_argument("action", choices=["clean", "build", "build-debug", "run"], help="Action to perform.")
    args = parser.parse_args()

    if args.action == "clean":
        clean()
    elif args.action == "build":
        build(debug=False)
    elif args.action == "build-debug":
        build(debug=True)
    elif args.action == "run":
        build(debug=True)  # Ensure we are using debug build before running
        run()

if __name__ == "__main__":
    main()
