# amazon-kinesis-video-streams-sdp

The goal of the SDP library is provide SDP Serializer and Deserializer
functionalities.

## What is SDP?
[Session Description Protocol (SDP)](https://en.wikipedia.org/wiki/Session_Description_Protocol),
described in [RFC8866](https://datatracker.ietf.org/doc/html/rfc8866), is a
format for describing multimedia communication sessions for the purposes of
announcement and invitation. An SDP description consists of a number of lines of
text of the form:

`<type>=<value>`

where `<type>` is exactly one case-significant character and `<value>` is structured
text whose format depends on `<type>`. Here is an example of a SDP message:

```
v=0
o=jdoe 2890844526 2890842807 IN IP4 10.47.16.5
s=SDP Seminar
i=A Seminar on the session description protocol
u=http://www.example.com/seminars/sdp.pdf
e=j.doe@example.com (Jane Doe)
c=IN IP4 224.2.17.12/127
t=2873397496 2873404696
a=recvonly
m=audio 49170 RTP/AVP 0
m=video 51372 RTP/AVP 99
a=rtpmap:99 h263-1998/90000
```

## Building Unit Tests

### Platform Prerequisites
- For running unit tests
    - C99 compiler like gcc
    - CMake 3.13.0 or later
    - Ruby 2.0.0 or later is additionally required for the CMock test framework
      (that we use).
- For running the coverage target, gcov and lcov are additionally required.

### Checkout CMock Submodule
By default, the submodules in this repository are configured with `update=none`
in [.gitmodules](./.gitmodules) to avoid increasing clone time and disk space
usage of other repositories.

To build unit tests, the submodule dependency of CMock is required. Use the
following command to clone the submodule:

```
git submodule update --checkout --init --recursive test/unit-test/CMock
```

### Steps to build Unit Tests
1. Go to the root directory of this repository. (Make sure that the CMock
   submodule is cloned as described in [Checkout CMock Submodule](#checkout-cmock-submodule)).
1. Run the following command to generate Makefiles:

    ```
    cmake -S test/unit-test -B build/ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_CLONE_SUBMODULES=ON -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG
    ```
1. Run the following command to build the library and unit tests:

    ```
    make -C build all
    ```
1. Run the following command to execute all tests and view results:

    ```
    cd build && ctest -E system --output-on-failure
    ```

### Steps to generate code coverage report of Unit Test
1. Run Unit Tests in [Steps to build Unit Tests](#steps-to-build-unit-tests).
1. Generate coverage.info in build folder:

    ```
    make coverage
    ```
1. Get code coverage by lcov:

    ```
    lcov --rc lcov_branch_coverage=1 -r coverage.info -o coverage.info '*test*' '*CMakeCCompilerId*' '*mocks*'
    ```
1. Generage HTML report in folder `CodecovHTMLReport`:

    ```
    genhtml --rc lcov_branch_coverage=1 --ignore-errors source coverage.info --legend --output-directory=CodecovHTMLReport
    ```

### Script to run Unit Test and generate code coverage report

```
cmake -S test/unit-test -B build/ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_CLONE_SUBMODULES=ON -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG -DLIBRARY_LOG_LEVEL=LOG_DEBUG'
make -C build all
cd build
ctest -E system --output-on-failure
make coverage
lcov --rc lcov_branch_coverage=1 -r coverage.info -o coverage.info '*test*' '*CMakeCCompilerId*' '*mocks*'
genhtml --rc lcov_branch_coverage=1 --ignore-errors source coverage.info --legend --output-directory=CodecovHTMLReport
```
