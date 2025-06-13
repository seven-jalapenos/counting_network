cmake_minimum_required(VERSION 3.15)
project(bitonic_network)

set(CMAKE_CXX_STANDARD 20)


# Set source and test directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_SOURCE_DIR}/test")

# Include directories
include_directories(${SRC_DIR})

# Source files
set(SRC_FILES
        ${SRC_DIR}/node.cpp
)

# Test files
set(TEST_FILES
        ${TEST_DIR}/bitonic_network_test.cpp
)

# Define the executable
add_executable(bitonic_network_test
        ${SRC_FILES}
        ${TEST_FILES}
)

# Optional: set output directory for binaries
set_target_properties(bitonic_network_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TEST_DIR}/bin
)
