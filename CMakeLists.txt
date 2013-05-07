
#
# Usage:
# - mkdir build && cd build
# - cmake -G"Unix Makefiles" ..
# - make
#

cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

# project name
project(3dNovac)

set(NC_LICENSE "LGPL")
message("*** 3dNovac use ${NC_LICENSE} license ***")

set(NC_VERSION_MAJOR 0)
set(NC_VERSION_MINOR 1)
set(NC_VERSION "${NC_VERSION_MAJOR}.${NC_VERSION_MINOR}" )
add_definitions(-DNC_VERSION=\"${NC_VERSION}\")
message("*** Using Nc version ${NC_VERSION} ***")

if(NOT DEFINED NC_BUILD_PLUGINS)
    set(NC_BUILD_PLUGINS true CACHE BOOL "If true, add the plugins to the project.")
endif()
if(NOT DEFINED NC_BUILD_DEMOS)
    set(NC_BUILD_DEMOS true CACHE BOOL "If true, add the demos to the project.")
endif()
if(NOT DEFINED NC_BUILD_EXAMPLES)
    set(NC_BUILD_EXAMPLES true CACHE BOOL "If true, add the examples to the project.")
endif()

# executable and libraries output path
set(LIBRARY_OUTPUT_PATH  ${3dNovac_SOURCE_DIR}/lib CACHE PATH "Single directory for all libraries.")
set(EXECUTABLE_OUTPUT_PATH ${3dNovac_SOURCE_DIR}/bin CACHE PATH "Single directory for all executables.")
mark_as_advanced(EXECUTABLE_OUTPUT_PATH)
mark_as_advanced(LIBRARY_OUTPUT_PATH)

# add 3dNovac modules
# the second argument is usefull for an out of source build.
add_subdirectory(./Nc/Core/ ./Nc/Core/)
add_subdirectory(./Nc/Graphics/ ./Nc/Graphics/)
add_subdirectory(./Nc/GUI/ ./Nc/GUI/)
add_subdirectory(./Nc/Contrib/ ./Nc/Contrib/)
add_subdirectory(./Nc/Audio/ ./Nc/Audio/)

# Following projects won't be compiled with the ALL target or when you execute "make", to compile then, you must specify the target in your IDE or by "make 'TargetName'".
# Those projects are optional.
# plugins
if(NC_BUILD_PLUGINS)
  add_subdirectory(./Plugin/FileFormatCollada/ ./Plugin/FileFormatCollada/)
endif()

# examples
if(NC_BUILD_EXAMPLES)
  add_subdirectory(./Examples/ ./Examples/)
endif()

# demos
if(NC_BUILD_DEMOS)
  add_subdirectory(./Demos/ ./Demos/)
endif()
