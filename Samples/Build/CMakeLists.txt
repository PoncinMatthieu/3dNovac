cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(3dNovac-Samples)

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
file(
  GLOB
  Tuto_NcCore_Client-sources
  ../Tuto_NcCore_Client/*.cpp
  ../Tuto_NcCore_Client/*.h
)

add_executable(Tuto_NcCore_Client			${Tuto_NcCore_Client-sources})
add_dependencies(Tuto_NcCore_Client			Nc-Core)
target_link_libraries(Tuto_NcCore_Client	Nc-Core)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(Tuto_NcCore_Client ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


#------------------------------------------------------------------------------
#	Frustum Demo
#------------------------------------------------------------------------------
file(
  GLOB
  FrustumDemo-sources
  ../FrustumDemo/*.cpp
  ../FrustumDemo/*.h
)

add_executable(FrustumDemo					${FrustumDemo-sources})
add_library(PluginFrustumDemo				SHARED ${FrustumDemo-sources})
add_dependencies(FrustumDemo				Nc-Core Nc-Graphics Nc-GUI PluginFileFormatCollada)
add_dependencies(PluginFrustumDemo			Nc-Core Nc-Graphics Nc-GUI PluginFileFormatCollada)
target_link_libraries(FrustumDemo			Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(PluginFrustumDemo		Nc-Core Nc-Graphics Nc-GUI)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(FrustumDemo ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


#------------------------------------------------------------------------------
#	GUI Demo
#------------------------------------------------------------------------------
file(
  GLOB
  GUIDemo-sources
  ../GUIDemo/*.cpp
  ../GUIDemo/*.h
)

add_executable(GUIDemo					${GUIDemo-sources})
add_library(PluginGUIDemo				SHARED ${GUIDemo-sources})
add_dependencies(GUIDemo 				Nc-Core Nc-Graphics Nc-GUI)
add_dependencies(PluginGUIDemo 			Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(GUIDemo			Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(PluginGUIDemo		Nc-Core Nc-Graphics Nc-GUI)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(GUIDemo ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


#------------------------------------------------------------------------------
#	Light Demo
#------------------------------------------------------------------------------
file(
  GLOB
  LightDemo-sources
  ../LightDemo/*.cpp
  ../LightDemo/*.h
)

add_executable(LightDemo				${LightDemo-sources})
add_library(PluginLightDemo				SHARED ${LightDemo-sources})
add_dependencies(LightDemo 				Nc-Core Nc-Graphics Nc-GUI PluginFileFormatCollada)
add_dependencies(PluginLightDemo 		Nc-Core Nc-Graphics Nc-GUI PluginFileFormatCollada)
target_link_libraries(LightDemo			Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(PluginLightDemo	Nc-Core Nc-Graphics Nc-GUI)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(LightDemo ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


#------------------------------------------------------------------------------
#	bSplineDemo Demo
#------------------------------------------------------------------------------
file(
  GLOB
  bSplinesDemo-sources
  ../bSplinesDemo/*.cpp
  ../bSplinesDemo/*.h
)

add_executable(bSplinesDemo					${bSplinesDemo-sources})
add_library(PluginbSplinesDemo				SHARED ${bSplinesDemo-sources})
add_dependencies(bSplinesDemo 				Nc-Core Nc-Graphics Nc-GUI)
add_dependencies(PluginbSplinesDemo 		Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(bSplinesDemo			Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(PluginbSplinesDemo	Nc-Core Nc-Graphics Nc-GUI)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(bSplineDemo ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


#------------------------------------------------------------------------------
#	FBEffects Demo
#------------------------------------------------------------------------------
file(
  GLOB
  FBEffectsDemo-sources
  ../FBEffectsDemo/*.cpp
  ../FBEffectsDemo/*.h
)

add_executable(FBEffectsDemo				${FBEffectsDemo-sources})
add_library(PluginFBEffectsDemo				SHARED ${FBEffectsDemo-sources})
add_dependencies(FBEffectsDemo 				Nc-Core Nc-Graphics Nc-GUI PluginFileFormatCollada)
add_dependencies(PluginFBEffectsDemo 		Nc-Core Nc-Graphics Nc-GUI PluginFileFormatCollada)
target_link_libraries(FBEffectsDemo			Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(PluginFBEffectsDemo	Nc-Core Nc-Graphics Nc-GUI)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(FBEffectsDemo ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	Sokoban Demo
#------------------------------------------------------------------------------
file(
  GLOB
  SokobanDemo-sources
  ../SokobanDemo/*.cpp
  ../SokobanDemo/*.h
)

add_executable(SokobanDemo					${SokobanDemo-sources})
add_library(PluginSokobanDemo				SHARED ${SokobanDemo-sources})
add_dependencies(SokobanDemo 				Nc-Core Nc-Graphics Nc-GUI Nc-Contrib PluginFileFormatCollada)
add_dependencies(PluginSokobanDemo 			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib PluginFileFormatCollada)
target_link_libraries(SokobanDemo			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)
target_link_libraries(PluginSokobanDemo		Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(SokobanDemo ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()

#------------------------------------------------------------------------------
#	Simple Particle Effect
#------------------------------------------------------------------------------
file(
  GLOB
  SimpleParticleEffect-sources
  ../SimpleParticleEffect/*.cpp
  ../SimpleParticleEffect/*.h
)

add_executable(SimpleParticleEffect					${SimpleParticleEffect-sources})
add_library(PluginSimpleParticleEffect				SHARED ${SimpleParticleEffect-sources})
add_dependencies(SimpleParticleEffect 				Nc-Core Nc-Graphics Nc-GUI)
add_dependencies(PluginSimpleParticleEffect 		Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(SimpleParticleEffect			Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(PluginSimpleParticleEffect	Nc-Core Nc-Graphics Nc-GUI)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(SimpleParticleEffect ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


#------------------------------------------------------------------------------
#	Sample Viewer
#------------------------------------------------------------------------------
file(
  GLOB
  SampleViewer-sources
  ../SampleViewer/*.cpp
  ../SampleViewer/*.h

  ../bSplinesDemo/GameEngine.cpp
  ../bSplinesDemo/bSplines.cpp
  ../bSplinesDemo/*.h
)

add_executable(SampleViewer					${SampleViewer-sources})
add_library(PluginSampleViewer				SHARED ${SampleViewer-sources})
add_dependencies(SampleViewer 				Nc-Core Nc-Graphics Nc-GUI)
add_dependencies(PluginSampleViewer 		Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(SampleViewer			Nc-Core Nc-Graphics Nc-GUI)
target_link_libraries(PluginSampleViewer	Nc-Core Nc-Graphics Nc-GUI)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(SampleViewer ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()
