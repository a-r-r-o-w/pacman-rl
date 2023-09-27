cmake_minimum_required(VERSION 3.0)

project(
  pacman-reinforcement-learning
  DESCRIPTION
    "Pacman Reinforcement Learning"
  LANGUAGES
    C
    CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(PYBIND_BINDING_FILE "pacman_rl")

if (CMAKE_BUILD_TYPE STREQUAL "")
  message(STATUS "No build type was specified using -DCMAKE_BUILD_TYPE=[DEBUG|RELEASE]. Defaulting to Release build")
  set(CMAKE_BUILD_TYPE "DEBUG")
endif()

# Debug build compile flags. Remove "-fsanitize=address,undefined" if you do not have
# [ASan](https://github.com/google/sanitizers/wiki/AddressSanitizer) installed
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -DDEBUG_MODE -D_GLIBCXX_DEBUG -fsanitize=address,undefined -fPIC")

# Release build compile flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -fPIC")

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")

# External dependencies
include_directories(./extern/pybind11/include)
include_directories(./extern/raylib/src)

add_subdirectory(./extern/pybind11)
add_subdirectory(./extern/raylib)

# Project dependencies
include_directories(./src/)
include_directories(./src/pacman)
include_directories(./src/render)

add_subdirectory(./src)
# add_subdirectory(./bindings)

# Generate stubs for the python bindings
# add_custom_command(
#   OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_BINDING_FILE}.pyi"
#   COMMAND stubgen --include-private --ignore-errors --verbose -m "${PYBIND_BINDING_FILE}" -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#   DEPENDS "${PYBIND_BINDING_FILE}"
#   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
#   COMMENT "Generating stubs for ${PYBIND_BINDING_FILE}"
# )

# add_custom_target(run ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${PYBIND_BINDING_FILE}.pyi")
