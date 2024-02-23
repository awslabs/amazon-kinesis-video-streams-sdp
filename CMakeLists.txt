cmake_minimum_required(VERSION 3.6.3)
project(kvssdp C)

include(sdpFilePaths.cmake)

add_library(kvssdp ${SDP_SOURCES})

target_include_directories(kvssdp PUBLIC
                           ${SDP_INCLUDE_PUBLIC_DIRS})

# install header files
install(
    FILES ${SDP_INCLUDE_PUBLIC_FILES}
    DESTINATION include/kvssdp)

# install SDP library
install(
    TARGETS kvssdp
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
