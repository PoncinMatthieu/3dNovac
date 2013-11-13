
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(Nc-Net)

# force the build type to release if the type is not already set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)." FORCE)
endif()

if(NOT DEFINED NC_NET_BUILD_MONGO)
  set(NC_NET_BUILD_MONGO true CACHE BOOL "If true, build the Mongodb abstract sources.")
endif()

if(NOT DEFINED NC_NET_BUILD_TLS_BOTAN)
  set(NC_NET_BUILD_TLS_BOTAN true CACHE BOOL "If true, build the Tls classes using Botan lib.")
endif()

if(NOT DEFINED NC_NET_BUILD_KYOTOTYCOON)
  set(NC_NET_BUILD_KYOTOTYCOON true CACHE BOOL "If true, build the KyotoTycoon abstract sources.")
endif()

# define _DEBUG
if(UNIX)
  if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-D_DEBUG)
  endif()
endif()

set(NC_ROOT ../../)

# set the include and link directories
include_directories(${NC_ROOT})

# we want to compile our libs dynamically
add_definitions(-DNC_DYNAMIC_LINK)


# Set the Nc-Net sources path only the API folder is not set
# we use a global and not a global recurse to avoid the svn directories and because some folder shouldn't be added to the compilation process
file(GLOB Nc-Net-sources ./*.cpp ./*.h)

if (NOT NC_NET_BUILD_MONGO)
  file(GLOB Nc-Net-Mongo ./Mongo.cpp ./Mongo.h)
  list(REMOVE_ITEM Nc-Net-sources ${Nc-Net-Mongo})
endif()

if (NOT NC_NET_BUILD_TLS_BOTAN)
  file(GLOB Nc-Net-TLS-Botan ./TlsConnection.cpp ./TlsConnection.h)
  list(REMOVE_ITEM Nc-Net-sources ${Nc-Net-TLS-Botan})
else()
  include_directories(/usr/local/include/botan-1.10/)
endif()

if (NOT NC_NET_BUILD_KYOTOTYCOON)
  file(GLOB Nc-Net-KyotoTycoon ./KyotoTycoon.cpp ./KyotoTycoon.h)
  list(REMOVE_ITEM Nc-Net-sources ${Nc-Net-KyotoTycoon})
endif()

add_library(Nc-Net SHARED ${Nc-Net-sources})
add_dependencies(Nc-Net Nc-Core)
target_link_libraries(Nc-Net Nc-Core)
if (NC_NET_BUILD_MONGO)
  target_link_libraries(Nc-Net mongoclient boost_thread-mt boost_filesystem boost_program_options boost_system)
endif()
if (NC_NET_BUILD_TLS_BOTAN)
  target_link_libraries(Nc-Net botan-1.10 ssl) # I don't know why, but I had to add the 'ssl' lib as well as botan.. ssl shouldn't be needed.
endif()
if (NC_NET_BUILD_KYOTOTYCOON)
  target_link_libraries(Nc-Net kyototycoon)
endif()

# set the export defines to export the symbole in the dll
# those defines are more likely more usefull for Windows, but it could be interesting to keep these define on the other systems
set_target_properties(Nc-Net PROPERTIES DEFINE_SYMBOL NC_EXPORT_NET)

# copy dll and .pdb files for excutable dependencies if the lib and bin output file are different
if (WIN32)
  if(NOT ${LIBRARY_OUTPUT_PATH} EQUAL ${EXECUTABLE_OUTPUT_PATH})
     add_custom_command(TARGET Nc-Net POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Nc-Net.dll ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

     if (MSVC_IDE)
       if (${CMAKE_CFG_INTDIR} MATCHES "Debug")
	 add_custom_command(TARGET Nc-Net POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Nc-Net.pdb ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
       endif()
     endif()
   endif()
endif()
