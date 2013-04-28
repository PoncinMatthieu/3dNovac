
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(Nc-Contrib)

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

# Set the Nc-Contrib sources path
file(GLOB_RECURSE Nc-Contrib-sources
  ./*.cpp
  ./*.h
)

add_library(Nc-Contrib SHARED ${Nc-Contrib-sources})

if(UNIX)
  target_link_libraries(Nc-Contrib Nc-Core Nc-Graphics)
else()
  target_link_libraries(Nc-Contrib Nc-Core Nc-Graphics freetype opengl32)
endif()

add_dependencies(Nc-Contrib	Nc-Core Nc-Graphics)

# set the export defines to export the symbole in the dll
# those defines are more likely more usefull for Windows, but it could be interesting to keep these define on the other systems
set_target_properties(Nc-Contrib PROPERTIES DEFINE_SYMBOL EXPORT_CONTRIB)

# copy dll and .pdb files for excutable dependencies if NC_COPY_DEPENDENCIES_PATH is set
if (WIN32)
  if(NC_COPY_DEPENDENCIES_PATH)
     add_custom_command(TARGET Nc-Contrib POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${NC_DIR}/lib/${CMAKE_CFG_INTDIR}/Nc-Contrib.dll ${NC_COPY_DEPENDENCIES_PATH}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

     if (MSVC_IDE)
       if (${CMAKE_CFG_INTDIR} MATCHES "Debug")
	 add_custom_command(TARGET Nc-Contrib POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${NC_DIR}/lib/${CMAKE_CFG_INTDIR}/Nc-Contrib.pdb ${NC_COPY_DEPENDENCIES_PATH}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
       endif()
     endif()
   endif()
endif()
