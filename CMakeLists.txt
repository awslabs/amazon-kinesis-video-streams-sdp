cmake_minimum_required(VERSION 3.6.3)
project(kvsSdp C)

set(TYPE_OF_LIB SHARED)
if (BUILD_STATIC)
  set(TYPE_OF_LIB STATIC)
endif()

file(GLOB KVS_SDP_SOURCE_FILES_BASE "source/*.c")

include_directories(source/include)

add_library(kvsSdp ${TYPE_OF_LIB} ${KVS_SDP_SOURCE_FILES_BASE})

install(
    DIRECTORY source/include
	DESTINATION .)

install(
    TARGETS kvsSdp
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
