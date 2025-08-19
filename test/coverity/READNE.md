# Static code analysis for SDP Library
This directory is made for the purpose of statically testing the MISRA C:2012 compliance of SDP Library using
[Black Duck Coverity](https://www.blackduck.com/static-analysis-tools-sast/coverity.html) static analysis tool.
To that end, this directory provides a [configuration file](./misra.config) to use when
building a binary for the tool to analyze.

> **Note**
For generating the report as outlined below, we have used Coverity version 2025.6.0.

For details regarding the suppressed violations in the report (which can be generated using the instructions described below), please
see the [MISRA.md](../../MISRA.md) file.

## Getting Started
### Prerequisites
You can run this on a platform supported by Coverity. The list and other details can be found [here](https://documentation.blackduck.com/bundle/coverity-docs/page/deploy-install-guide/topics/supported_platforms_for_coverity_analysis.html).
To compile and run the Coverity target successfully, you must have the following:

1. CMake version > 3.13.0 (You can check whether you have this by typing `cmake --version`)
2. GCC compiler
    - You can see the downloading and installation instructions [here](https://gcc.gnu.org/install/).
3. Download the repo and include the submodules using the following commands.
    - `git clone --recurse-submodules https://github.com/awslabs/amazon-kinesis-video-streams-sdp`
    - `cd ./amazon-kinesis-video-streams-sdp`

### To build and run coverity:
Go to the root directory of the library and run the following commands:
~~~
cmake -S test/coverity/ -B build
cmake --build build --target coverity_scan
~~~

These commands will:
1. Configure the build system using the Coverity-specific CMake configuration
2. Build and execute the Coverity scan target, which handles all the necessary steps including:
   - Compiler configuration
   - Static analysis execution
   - Report generation in both HTML and JSON formats

You should now have the HTML formatted violations list in a directory named `build/coverity_report`.
With the current configuration and the provided project, you should not see any deviations.
