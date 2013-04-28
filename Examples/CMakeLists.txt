cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(Nc-Examples)

# setup the launcher module for vs2010
if(MSVC_IDE)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
	include(CreateLaunchers)
endif()

# set Nc root directory path
set(NC_DIR ../)

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
  Nc-Tuto1-sources
  ./Tuto1/*.cpp
  ./Tuto1/*.h
)

add_executable(Nc-Tuto1		${Nc-Tuto1-sources})
add_dependencies(Nc-Tuto1 	Nc-Core Nc-Graphics)
target_link_libraries(Nc-Tuto1	Nc-Core Nc-Graphics)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Nc-Tuto1 ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${NC_DIR}/bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	Tuto_NcCore_Server
#------------------------------------------------------------------------------
file(
  GLOB
  Nc-Tuto_NcCore_Server-sources
  ./Tuto_NcCore_Server/*.cpp
  ./Tuto_NcCore_Server/*.h
)

add_executable(Nc-Tuto_NcCore_Server 		${Nc-Tuto_NcCore_Server-sources})
add_dependencies(Nc-Tuto_NcCore_Server		Nc-Core)
target_link_libraries(Nc-Tuto_NcCore_Server	Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Nc-Tuto_NcCore_Server ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${NC_DIR}/bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	Tuto_NcCore_Client
#------------------------------------------------------------------------------
file(GLOB Nc-Tuto_NcCore_Client-sources ./Tuto_NcCore_Client.cpp)
add_executable(Nc-Tuto_NcCore_Client		${Nc-Tuto_NcCore_Client-sources})
add_dependencies(Nc-Tuto_NcCore_Client		Nc-Core)
target_link_libraries(Nc-Tuto_NcCore_Client	Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Nc-Tuto_NcCore_Client ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${NC_DIR}/bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	BinaryTree.cpp
#------------------------------------------------------------------------------
file(GLOB Nc-Example-BinaryTree-sources        ./BinaryTree.cpp)
add_executable(Nc-Example-BinaryTree           ${Nc-Example-BinaryTree-sources})
add_dependencies(Nc-Example-BinaryTree         Nc-Core)
target_link_libraries(Nc-Example-BinaryTree    Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Nc-Example-BinaryTree ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${NC_DIR}/bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	GraphNode.cpp
#------------------------------------------------------------------------------
file(GLOB Nc-Example-GraphNode-sources        ./GraphNode.cpp)
add_executable(Nc-Example-GraphNode           ${Nc-Example-GraphNode-sources})
add_dependencies(Nc-Example-GraphNode         Nc-Core)
target_link_libraries(Nc-Example-GraphNode    Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Nc-Example-GraphNode ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${NC_DIR}/bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


