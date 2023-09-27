#!/usr/bin/bash

# Constants for text formatting
BOLD=$(tput bold)
NORMAL=$(tput sgr0)
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
YELLOW=$(tput setaf 3)

# Usage function to display script usage
usage() {
  echo "Usage: $0 [OPTIONS]"
  echo "OPTIONS:"
  echo "  -h, --help           Display this help message."
  echo "  -t, --type TYPE      Specify the build type (DEBUG or RELEASE). Default is DEBUG."
  echo "  -j, --jobs NUM       Specify the number of parallel jobs for make. Default is 4."
  exit 1
}

# Default values
BUILD_TYPE="DEBUG"
NUM_JOBS=4

# Parse command line options
while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help)
      usage
      ;;
    -t|--type)
      shift
      if [[ $# -eq 0 ]]; then
        echo "${RED}Error: -t|--type option requires a build type (DEBUG or RELEASE).${NORMAL}"
        exit 1
      fi
      if [[ "$1" != "DEBUG" && "$1" != "RELEASE" ]]; then
        echo "${RED}Error: Invalid build type. Use DEBUG or RELEASE.${NORMAL}"
        exit 1
      fi
      BUILD_TYPE="$1"
      ;;
    -j|--jobs)
      shift
      if [[ $# -eq 0 ]]; then
        echo "${RED}Error: -j|--jobs option requires a number of jobs.${NORMAL}"
        exit 1
      fi
      if ! [[ "$1" =~ ^[0-9]+$ ]]; then
        echo "${RED}Error: Number of jobs must be a positive integer.${NORMAL}"
        exit 1
      fi
      NUM_JOBS="$1"
      ;;
    *)
      echo "${RED}Error: Unknown option $1.${NORMAL}"
      usage
      ;;
  esac
  shift
done

BUILD_DIR="build"

# Clean the existing build directory if it exists
if [ -d "$BUILD_DIR" ]; then
  echo "${YELLOW}Cleaning existing build directory...${NORMAL}"
  rm -r "$BUILD_DIR"
fi

# Create and move into the build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit

# Build the project
echo "${BOLD}Configuring and building with CMake...${NORMAL}"
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DPYTHON_EXECUTABLE="$(command -v python3)" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCUSTOMIZE_BUILD=ON -DWITH_PIC=ON
make -j"$NUM_JOBS"

# Generate stubs for the Python module
# stubgen -m pacman

# Check if the build was successful
if [ $? -eq 0 ]; then
  echo "${GREEN}${BOLD}Build successful.${NORMAL}"
else
  echo "${RED}${BOLD}Build failed. Check the build logs for errors.${NORMAL}"
fi
