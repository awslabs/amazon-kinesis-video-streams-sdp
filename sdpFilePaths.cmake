# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# SDP library source files.
set( SDP_SOURCES
     "source/sdp_deserializer.c"
     "source/sdp_serializer.c" )

# SDP library public include directories.
set( SDP_INCLUDE_PUBLIC_DIRS
     "source/include" )

# SDP library public include header files.
set( SDP_INCLUDE_PUBLIC_FILES
     "source/include/sdp_config_defaults.h"
     "source/include/sdp_data_types.h"
     "source/include/sdp_deserializer.h"
     "source/include/sdp_serializer.h" )
