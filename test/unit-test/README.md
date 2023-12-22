# Unit Tests for amazon-kinesis-video-streams-sdp library

This directory is made for the purpose of Unit testing and tries to provide the
tools for developing unit tests. To that end, this directory submodules the
[CMock](https://github.com/ThrowTheSwitch/CMock) framework (which submodules
[Unity](https://github.com/throwtheswitch/unity/)).

## Getting Started

### Prerequisites

You can run this on any GNU Make compatible systems. But in case of DOS based
systems some tweaking is required with the makefile. To compile and run this
project successfully, you must have the following:

1. Make (You can check whether you have this by typing `make --version`)
   - Not found? Try `apt-get install make`.
1. Ruby (You can check whether you have this by typing `ruby --version`)
   - Not found? Try `apt-get install ruby`.
1. CMake version > 3.13.0 (You can check whether you have this by typing
   `cmake --version`)
   - Not found? Try `apt-get install cmake`
   - Try the `cmake --version` command. If still the version number is >=
     3.13.0, skip to (4.) or else, continue.
   - You will need to get the latest CMake version using curl or wget (or
     similar command).
     - Uninstall the current version of CMake using
       `sudo apt remove --purge --auto-remove cmake`.
     - Download the [CMAKE version 3.13.0](https://cmake.org/files/v3.13/).
     - Extract the cmake download using `tar -xzvf cmake-3.13.0.tar.gz`.
     - Go to the extracted folder (`cd cmake-3.13.0`) and run `./bootstrap`.
     - Run `make -j$(nproc)' and then run `sudo make install`.
     - Check the version using `cmake --version` command.
1. lcov version 1.14 ( check with --version option )
   - 'sudo apt-get install lcov'

### To run the Unit tests:

Go to the root directory of the amazon-kinesis-video-streams-sdp repo and run the following script:

```
#!/bin/bash
# This script should be run from the root directory of the amazon-kinesis-video-streams-sdp repo.

if [[ ! -d source ]]; then
    echo "Please run this script from the root directory of the amazon-kinesis-video-streams-sdp repo."
    exit 1
fi

UNIT_TEST_DIR="test/unit-test"
BUILD_DIR="${UNIT_TEST_DIR}/build"

# Create the build directory using CMake:
rm -rf ${BUILD_DIR}/
cmake -S ${UNIT_TEST_DIR} -B ${BUILD_DIR}/ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_CLONE_SUBMODULES=ON -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG -DLIBRARY_LOG_LEVEL=LOG_DEBUG'

# Create the executables:
make -C ${BUILD_DIR}/ all

pushd ${BUILD_DIR}/
# Run the tests for all units
ctest -E system --output-on-failure
popd

# Calculate the coverage
make -C ${BUILD_DIR}/ coverage
lcov --rc lcov_branch_coverage=1 -r ${BUILD_DIR}/coverage.info -o ${BUILD_DIR}/coverage.info '*test*' '*CMakeCCompilerId*' '*mocks*'
lcov --rc lcov_branch_coverage=1 --list ${BUILD_DIR}/coverage.info

# Generate HTML coverage report
genhtml ${BUILD_DIR}/coverage.info -o ${BUILD_DIR}/htmlout
```

You should see an output similar to this:

```
test_SdpSerializer_Finalize_NullBuffer   PASS
test_SdpSerializer_Finalize_NullLength   PASS

=================== SUMMARY =====================

Tests Passed  : 108
Tests Failed  : 0
Tests Ignored : 0
```

And also:

```
Reading tracefile test/unit-test/build/coverage.info
                          |Lines       |Functions  |Branches    
Filename                  |Rate     Num|Rate    Num|Rate     Num
================================================================
[/home/ousc/git/amazon-kinesis-video-streams-sdp-fork/source/]
sdp_deserializer.c        | 100%    179| 100%     8| 100%    124
sdp_serializer.c          | 100%    202| 100%    11| 100%    150
================================================================
                    Total:| 100%    381| 100%    19| 100%    274
```
