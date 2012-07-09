cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(3dNovac-Demos)

# setup the launcher module for vs2010
if(MSVC_IDE)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-modules")
	include(CreateLaunchers)
endif()

# set Nc root directory path
set(NC_DIR ../..)
set(NC_DEMOS_DIR ..)

# set path to copy dependencies
set(NC_COPY_DEPENDENCIES_PATH ${NC_DEMOS_DIR}/bin/${CMAKE_CFG_INTDIR})

# executable and libraries output path
set(EXECUTABLE_OUTPUT_PATH ${NC_DEMOS_DIR}/bin/)
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
#	Frustum Demo
#------------------------------------------------------------------------------
file(
  GLOB
  FrustumDemo-sources
  ../FrustumDemo/*.cpp
  ../FrustumDemo/*.h
)

file(GLOB FrustumDemoMain-sources ../FrustumDemo/main.cpp)
list(REMOVE_ITEM FrustumDemo-sources ${FrustumDemoMain-sources})

add_executable(FrustumDemo					${FrustumDemo-sources} ${FrustumDemoMain-sources})
add_dependencies(FrustumDemo				Nc-Core Nc-Graphics Nc-GUI Nc-Contrib PluginFileFormatCollada)
target_link_libraries(FrustumDemo			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

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

file(GLOB GUIDemoMain-sources ../GUIDemo/main.cpp)
list(REMOVE_ITEM GUIDemo-sources ${GUIDemoMain-sources})

add_executable(GUIDemo					${GUIDemo-sources} ${GUIDemoMain-sources})
add_dependencies(GUIDemo 				Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)
target_link_libraries(GUIDemo			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

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

file(GLOB LightDemoMain-sources ../LightDemo/main.cpp)
list(REMOVE_ITEM LightDemo-sources ${LightDemoMain-sources})

add_executable(LightDemo				${LightDemo-sources} ${LightDemoMain-sources})
add_dependencies(LightDemo 				Nc-Core Nc-Graphics Nc-GUI Nc-Contrib PluginFileFormatCollada)
target_link_libraries(LightDemo			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

# create the launcher for vs 2010
if(MSVC_IDE)
	create_target_launcher(LightDemo ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


#------------------------------------------------------------------------------
#	bSplinesDemo Demo
#------------------------------------------------------------------------------
file(
  GLOB
  bSplinesDemo-sources
  ../bSplinesDemo/*.cpp
  ../bSplinesDemo/*.h
)

file(GLOB bSplinesDemoMain-sources ../bSplinesDemo/main.cpp)
list(REMOVE_ITEM bSplinesDemo-sources ${bSplinesDemoMain-sources})

add_executable(bSplinesDemo					${bSplinesDemo-sources} ${bSplinesDemoMain-sources})
add_dependencies(bSplinesDemo 				Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)
target_link_libraries(bSplinesDemo			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

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

file(GLOB FBEffectsDemoMain-sources ../FBEffectsDemo/main.cpp)
list(REMOVE_ITEM FBEffectsDemo-sources ${FBEffectsDemoMain-sources})

add_executable(FBEffectsDemo				${FBEffectsDemo-sources} ${FBEffectsDemoMain-sources})
add_dependencies(FBEffectsDemo 				Nc-Core Nc-Graphics Nc-GUI Nc-Contrib PluginFileFormatCollada)
target_link_libraries(FBEffectsDemo			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

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

file(GLOB SokobanDemoMain-sources ../SokobanDemo/main.cpp)
list(REMOVE_ITEM SokobanDemo-sources ${SokobanDemoMain-sources})

add_executable(SokobanDemo					${SokobanDemo-sources} ${SokobanDemoMain-sources})
add_dependencies(SokobanDemo 				Nc-Core Nc-Graphics Nc-GUI Nc-Contrib PluginFileFormatCollada)
target_link_libraries(SokobanDemo			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

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

file(GLOB SimpleParticleEffectMain-sources ../SimpleParticleEffect/main.cpp)
list(REMOVE_ITEM SimpleParticleEffect-sources ${SimpleParticleEffectMain-sources})

add_executable(SimpleParticleEffect			${SimpleParticleEffect-sources} ${SimpleParticleEffectMain-sources})
add_dependencies(SimpleParticleEffect			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)
target_link_libraries(SimpleParticleEffect		Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

# create the launcher for vs 2010
if(MSVC_IDE)
  create_target_launcher(SimpleParticleEffect ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()


#------------------------------------------------------------------------------
#	Sample Viewer
#------------------------------------------------------------------------------
file(
  GLOB
  DemoViewer-sources
  ../DemoViewer/*.cpp
  ../DemoViewer/*.h
)

list(APPEND DemoViewer-sources ${FrustumDemo-sources})
list(APPEND DemoViewer-sources ${GUIDemo-sources})
list(APPEND DemoViewer-sources ${LightDemo-sources})
list(APPEND DemoViewer-sources ${bSplinesDemo-sources})
list(APPEND DemoViewer-sources ${FBEffectsDemo-sources})
list(APPEND DemoViewer-sources ${SokobanDemo-sources})
list(APPEND DemoViewer-sources ${SimpleParticleEffect-sources})

add_executable(DemoViewer		${DemoViewer-sources})
add_dependencies(DemoViewer		Nc-Core Nc-Graphics Nc-GUI Nc-Contrib PluginFileFormatCollada)
target_link_libraries(DemoViewer	Nc-Core Nc-Graphics Nc-GUI Nc-Contrib)

# create the launcher for vs 2010
if(MSVC_IDE)
  create_target_launcher(DemoViewer ARGS "${COMMAND_LINE_ARGS}" RUNTIME_LIBRARY_DIRS ${RUNTIME_LIBRARY_DIRS} WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin/" ENVIRONMENT ${ENVIRONMENT_VARS})
endif()
