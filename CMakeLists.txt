cmake_minimum_required(VERSION 3.6.3)
project(kvsSdp C)

set(TYPE_OF_LIB SHARED)
if (BUILD_STATIC)
  set(TYPE_OF_LIB STATIC)
endif()

file(GLOB KVS_SDP_SOURCE_FILES_BASE "source/*.c")

include_directories(source/include)

install(
    DIRECTORY source/include
	DESTINATION .)

add_library(kvsSdp ${TYPE_OF_LIB} ${KVS_SDP_SOURCE_FILES_BASE})
