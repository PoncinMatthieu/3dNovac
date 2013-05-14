
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(Nc-Audio)

# force the build type to release if the type is not already set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
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
include_directories(${NC_ROOT}/extlibs/)
include_directories(${NC_ROOT}/extlibs/SFML/include/)
link_directories(${NC_ROOT}/lib/${CMAKE_CFG_INTDIR})
if(UNIX)
else()
  link_directories(${NC_ROOT}/extlibs/)
  link_directories(${NC_ROOT}/extlibs/SFML/lib/)
endif()

# we want to compile our libs dynamically
add_definitions(-DNC_DYNAMIC_LINK)

# Set the Nc-Audio sources path
file(GLOB_RECURSE Nc-Audio-sources
  ./*.cpp
  ./*.h
)

add_library(Nc-Audio SHARED ${Nc-Audio-sources})

if(UNIX)
  target_link_libraries(Nc-Audio Nc-Core sfml-audio sfml-system)
else()
  target_link_libraries(Nc-Audio general Nc-Core
				debug sfml-audio-d
				optimized sfml-audio)
endif()

add_dependencies(Nc-Audio	Nc-Core)

# set the export defines to export the symbole in the dll
# those defines are more likely more usefull for Windows, but it could be interesting to keep these define on the other systems
set_target_properties(Nc-Audio PROPERTIES DEFINE_SYMBOL NC_EXPORT_AUDIO)

# copy dll and .pdb files for excutable dependencies if the lib and bin output file are different
if (WIN32)
  if(NOT ${LIBRARY_OUTPUT_PATH} EQUAL ${EXECUTABLE_OUTPUT_PATH})
     add_custom_command(TARGET Nc-Audio POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Nc-Audio.dll ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

     if (MSVC_IDE)
       if (${CMAKE_CFG_INTDIR} MATCHES "Debug")
	 add_custom_command(TARGET Nc-Audio POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Nc-Audio.pdb ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
       endif()
     endif()
   endif()
endif()
