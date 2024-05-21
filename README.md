# amazon-kinesis-video-streams-sdp

The goal of SDP library is provide SDP Serializer and Deserializer functionalities with low memory footprint.

## What is SDP?
[Session Description Protocol (SDP)](https://en.wikipedia.org/wiki/Session_Description_Protocol), described in [RFC8866](https://datatracker.ietf.org/doc/html/rfc8866), is a format for describing multimedia communication sessions for the purposes of announcement and invitation. An SDP description consists of a number of lines of text of the form:

`<type>=<value>`

where <type> is exactly one case-significant character and <value> is structured text whose format depends on <type>. Here is an example of a SDP message:

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

## SDP interfaces
### Serializer
```
SdpResult_t SdpSerializer_Init( SdpSerializerContext_t * pCtx,
                                char * pBuffer,
                                size_t bufferLength )


SdpResult_t SdpSerializer_AddBuffer( SdpSerializerContext_t * pCtx,
                                     uint8_t type,
                                     const char * pValue,
                                     size_t valueLength )

SdpResult_t SdpSerializer_AddOriginator( SdpSerializerContext_t * pCtx,
                                         uint8_t type,
                                         const SdpOriginator_t * pOriginator )

...

SdpResult_t SdpSerializer_Finalize( SdpSerializerContext_t * pCtx,
                                    const char ** pSdpMessage,
                                    size_t * pSdpMessageLength )
```
1. Call SdpSerializer_Init when they want to send SDP content with buffer.
1. Keep appending info by calling corresponding APIs.
   - To append string, call SdpSerializer_AddBuffer().
   - To append originator, call SdpSerializer_AddOriginator().
   - etc.
1. Call SdpSerializer_Finalize() to get the result after serialization.

### Deserializer
```
SdpResult_t SdpDeserializer_Init( SdpDeserializerContext_t * pCtx,
                                  const char * pSdpMessage,
                                  size_t sdpMessageLength )

SdpResult_t SdpDeserializer_GetNext( SdpDeserializerContext_t * pCtx,
                                     uint8_t * pType,
                                     const char ** pValue,
                                     size_t * pValueLength )

SdpResult_t SdpDeserializer_ParseOriginator( const char * pValue,
                                             size_t valueLength,
                                             SdpOriginator_t * pOriginator )

...
```
1. Call SdpDeserializer_Init when they receive SDP content.
1. Keep calling SdpDeserializer_GetNext() to parse SDP content line by line.
1. Call corresponding parse APIs to get string into structure.
   - If return type is SDP_TYPE_ORIGINATOR, users can cll SdpSerializer_AddOriginator().
   - etc.
1. Loop to step 2 till end.

## Building Unit Tests

### Checkout CMock Submodule
By default, the submodules in this repository are configured with `update=none` in [.gitmodules](./.gitmodules) to avoid increasing clone time and disk space usage of other repositories.

To build unit tests, the submodule dependency of CMock is required. Use the following command to clone the submodule:
```
git submodule update --checkout --init --recursive test/unit-test/CMock
```

### Platform Prerequisites
- For running unit tests:
    - C99 compiler like gcc.
    - CMake 3.13.0 or later.
    - Ruby 2.0.0 or later (It is required for the CMock test framework that we use).
- For running the coverage target, gcov and lcov are required.

### Steps to build Unit Tests
1. Go to the root directory of this repository. (Make sure that the CMock submodule is cloned as described in [Checkout CMock Submodule](#checkout-cmock-submodule))
1. Run the cmake command: 
    ```sh
    cmake -S test/unit-test -B build/ -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DBUILD_CLONE_SUBMODULES=ON -DCMAKE_C_FLAGS='--coverage -Wall -Wextra -Werror -DNDEBUG 
    ```
1. Run this command to build the library and unit tests:
    ```sh
    make -C build all
    ```
1. The generated test executables will be present in build/bin/tests folder.
1. Run below commands to execute all tests and view the test run summary.
    ```sh
    cd build && ctest -E system --output-on-failure
    ```

### Steps to generate code coverage report of Unit Test
1. Run Unit Tests in [Steps to build Unit Tests](#steps-to-build-unit-tests).
1. Generate coverage.info in build folder:
    ```
    make coverage
    ```
1. Get code coverage by lcov.
    ```
    lcov --rc lcov_branch_coverage=1 -r coverage.info -o coverage.info '*test*' '*CMakeCCompilerId*' '*mocks*'
    ```
1. Generage HTML report in folder `CodecovHTMLReport`.
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
