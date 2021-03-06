
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(Nc-GUI)

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
include_directories(${NC_ROOT}/extlibs/freetype/)
include_directories(${NC_ROOT}/extlibs/GL3/)
include_directories(${NC_ROOT}/extlibs/libpng/)
include_directories(${NC_ROOT}/extlibs/zlib/)
link_directories(${NC_ROOT}/lib/${CMAKE_CFG_INTDIR})
if(UNIX)
else()
  link_directories(${NC_ROOT}/extlibs/)
endif()

# we want to compile our libs dynamically
add_definitions(-DNC_DYNAMIC_LINK)

# Set the Nc-GUI sources path
file(GLOB_RECURSE Nc-GUI-sources
  ./*.cpp
  ./*.h
)

add_library(Nc-GUI SHARED ${Nc-GUI-sources})

if(UNIX)
  target_link_libraries(Nc-GUI Nc-Core Nc-Graphics)
else()
  target_link_libraries(Nc-GUI Nc-Core Nc-Graphics freetype opengl32)
endif()

add_dependencies(Nc-GUI	Nc-Core Nc-Graphics)

# set the export defines to export the symbole in the dll
# those defines are more likely more usefull for Windows, but it could be interesting to keep these define on the other systems
set_target_properties(Nc-GUI PROPERTIES DEFINE_SYMBOL NC_EXPORT_GUI)

# copy dll and .pdb files for excutable dependencies if the lib and bin output file are different
if (WIN32)
  if(NOT ${LIBRARY_OUTPUT_PATH} EQUAL ${EXECUTABLE_OUTPUT_PATH})
     add_custom_command(TARGET Nc-GUI POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Nc-GUI.dll ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

     if (MSVC_IDE)
       if (${CMAKE_CFG_INTDIR} MATCHES "Debug")
	 add_custom_command(TARGET Nc-GUI POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${LIBRARY_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/Nc-GUI.pdb ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
       endif()
     endif()
   endif()
endif()
