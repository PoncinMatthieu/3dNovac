cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(3dNovac-Examples)

# setup the launcher module for vs2010
if(MSVC_IDE)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
	include(CreateLaunchers)
endif()

# set Nc root directory path
set(NC_DIR ../..)

# set path to copy dependencies
set(NC_COPY_DEPENDENCIES_PATH ../bin/${CMAKE_CFG_INTDIR})

# executable and libraries output path
set(EXECUTABLE_OUTPUT_PATH ../bin/)
set(LIBRARY_OUTPUT_PATH ${NC_DIR}/lib/)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# add the _DEBUG define
if(UNIX)
  if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-D_DEBUG)
  endif()
endif()

# we want to compile our libs dynamically
add_definitions(-DNC_DYNAMIC_LINK)

# add 3dNovac to the project
SUBDIRS(${NC_DIR}/Build)

# set include and link directories
include_directories(${NC_DIR})
include_directories(${NC_DIR}/extlibs/)
link_directories(${NC_DIR}/lib/${CMAKE_CFG_INTDIR})
link_directories(${NC_DIR}/extlibs/)

#------------------------------------------------------------------------------
#	Tuto1
#------------------------------------------------------------------------------
file(
  GLOB
  Tuto1-sources
  ../Tuto1/*.cpp
  ../Tuto1/*.h
)

add_executable(Tuto1		${Tuto1-sources})
add_dependencies(Tuto1 		Nc-Core Nc-Graphics)
target_link_libraries(Tuto1	Nc-Core Nc-Graphics)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Tuto1 ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	Tuto_NcCore_Server
#------------------------------------------------------------------------------
file(
  GLOB
  Tuto_NcCore_Server-sources
  ../Tuto_NcCore_Server/*.cpp
  ../Tuto_NcCore_Server/*.h
)

add_executable(Tuto_NcCore_Server 			${Tuto_NcCore_Server-sources})
add_dependencies(Tuto_NcCore_Server			Nc-Core)
target_link_libraries(Tuto_NcCore_Server	Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Tuto_NcCore_Server ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	Tuto_NcCore_Client
#------------------------------------------------------------------------------
file(GLOB Tuto_NcCore_Client-sources ../Tuto_NcCore_Client.cpp)
add_executable(Tuto_NcCore_Client		${Tuto_NcCore_Client-sources})
add_dependencies(Tuto_NcCore_Client		Nc-Core)
target_link_libraries(Tuto_NcCore_Client	Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Tuto_NcCore_Client ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	BinaryTree.cpp
#------------------------------------------------------------------------------
file(GLOB BinaryTree-sources        ../BinaryTree.cpp)
add_executable(BinaryTree           ${BinaryTree-sources})
add_dependencies(BinaryTree         Nc-Core)
target_link_libraries(BinaryTree    Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(BinaryTree ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	GraphNode.cpp
#------------------------------------------------------------------------------
file(GLOB GraphNode-sources        ../GraphNode.cpp)
add_executable(GraphNode           ${GraphNode-sources})
add_dependencies(GraphNode         Nc-Core)
target_link_libraries(GraphNode    Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(GraphNode ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


