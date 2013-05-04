
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(Nc-Core)

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

# we want to compile our libs dynamically
add_definitions(-DNC_DYNAMIC_LINK)


# Set the Nc-Core sources path only the API folder is not set
# we use a global and not a global recurse to avoid the svn directories and because some folder shouldn't be added to the compilation process
file(GLOB Nc-Core-sources
  ../*.h
  ./*.cpp			./*.h
  ./Utils/*.cpp                 ./Utils/*.h
  ./Utils/Debug/*.cpp           ./Utils/Debug/*.h
  ./Utils/Xml/*.cpp             ./Utils/Xml/*.h
  ./System/*.cpp                ./System/*.h
  ./System/API/*.cpp            ./System/API/*.h
  ./System/Input/*.cpp          ./System/Input/*.h
  ./Math/*.cpp                  ./Math/*.h
  ./Graph/*.cpp                 ./Graph/*.h
  ./Engine/*.cpp                ./Engine/*.h
  ./Network/*.cpp               ./Network/*.h
)

file(GLOB Nc-Core-sources-api-specific
  ./System/API/Unix/*
  ./System/API/Windows/*
)

list(REMOVE_ITEM Nc-Core-sources ${Nc-Core-sources-api-specific})

add_library(Nc-Core SHARED ${Nc-Core-sources})

if(UNIX)
  target_link_libraries(Nc-Core pthread)
else()
  target_link_libraries(Nc-Core ws2_32)
endif()

# set the export defines to export the symbole in the dll
# those defines are more likely more usefull for Windows, but it could be interesting to keep these define on the other systems
set_target_properties(Nc-Core PROPERTIES DEFINE_SYMBOL NC_EXPORT_CORE)

# copy dll and .pdb files for excutable dependencies if NC_COPY_DEPENDENCIES_PATH is set
if (WIN32)
  if(NC_COPY_DEPENDENCIES_PATH)
     add_custom_command(TARGET Nc-Core POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${NC_DIR}/lib/${CMAKE_CFG_INTDIR}/Nc-Core.dll ${NC_COPY_DEPENDENCIES_PATH}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

     if (MSVC_IDE)
       if (${CMAKE_CFG_INTDIR} MATCHES "Debug")
	 add_custom_command(TARGET Nc-Core POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${NC_DIR}/lib/${CMAKE_CFG_INTDIR}/Nc-Core.pdb ${NC_COPY_DEPENDENCIES_PATH}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
       endif()
     endif()
   endif()
endif()
