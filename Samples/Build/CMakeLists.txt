cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(3dNovac-Samples)

set(EXECUTABLE_OUTPUT_PATH ../bin/)
set(LIBRARY_OUTPUT_PATH ../../lib/)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(-D_DEBUG)
endif()

include_directories(../../)
link_directories(../../lib/)


#------------------------------------------------------------------------------
#	Tuto1
#------------------------------------------------------------------------------
file(
  GLOB
  Tuto1-sources
  ../Tuto1/*.cpp
)

add_executable(Tuto1
  ${Tuto1-sources}
)

target_link_libraries(Tuto1
  Nc-Core
  Nc-Graphics
  GL
)


#------------------------------------------------------------------------------
#	Tuto_NcCore_Server
#------------------------------------------------------------------------------
file(
  GLOB
  Tuto_NcCore_Server-sources
  ../Tuto_NcCore_Server/*.cpp
)

add_executable(Tuto_NcCore_Server ${Tuto_NcCore_Server-sources})

target_link_libraries(Tuto_NcCore_Server Nc-Core)


#------------------------------------------------------------------------------
#	Tuto_NcCore_Client
#------------------------------------------------------------------------------
file(
  GLOB
  Tuto_NcCore_Client-sources
  ../Tuto_NcCore_Client/*.cpp
)

add_executable(Tuto_NcCore_Client ${Tuto_NcCore_Client-sources})

target_link_libraries(Tuto_NcCore_Client Nc-Core)


#------------------------------------------------------------------------------
#	Frustum Demo
#------------------------------------------------------------------------------
file(
  GLOB
  FrustumDemo-sources
  ../FrustumDemo/*.cpp
)

add_executable(FrustumDemo
  ${FrustumDemo-sources}
)

add_library(PluginFrustumDemo
  SHARED
  ${FrustumDemo-sources}
)

target_link_libraries(FrustumDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)

target_link_libraries(PluginFrustumDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)


#------------------------------------------------------------------------------
#	GUI Demo
#------------------------------------------------------------------------------
file(
  GLOB
  GUIDemo-sources
  ../GUIDemo/*.cpp
)

add_executable(GUIDemo
  ${GUIDemo-sources}
)

add_library(PluginGUIDemo
  SHARED
  ${GUIDemo-sources}
)

target_link_libraries(GUIDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)

target_link_libraries(PluginGUIDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)


#------------------------------------------------------------------------------
#	Light Demo
#------------------------------------------------------------------------------
file(
  GLOB
  LightDemo-sources
  ../LightDemo/*.cpp
)

add_executable(LightDemo
  ${LightDemo-sources}
)

add_library(PluginLightDemo
  SHARED
  ${LightDemo-sources}
)

target_link_libraries(LightDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)

target_link_libraries(PluginLightDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)


#------------------------------------------------------------------------------
#	bSplineDemo Demo
#------------------------------------------------------------------------------
file(
  GLOB
  bSplinesDemo-sources
  ../bSplinesDemo/*.cpp
)

add_executable(bSplinesDemo
  ${bSplinesDemo-sources}
)

add_library(PluginbSplinesDemo
  SHARED
  ${bSplinesDemo-sources}
)

target_link_libraries(bSplinesDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)

target_link_libraries(PluginbSplinesDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)


#------------------------------------------------------------------------------
#	FBEffects Demo
#------------------------------------------------------------------------------
file(
  GLOB
  FBEffectsDemo-sources
  ../FBEffectsDemo/*.cpp
)

add_executable(FBEffectsDemo
  ${FBEffectsDemo-sources}
)

add_library(PluginFBEffectsDemo
  SHARED
  ${FBEffectsDemo-sources}
)

target_link_libraries(FBEffectsDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)

target_link_libraries(PluginFBEffectsDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)


#------------------------------------------------------------------------------
#	Sokoban Demo
#------------------------------------------------------------------------------
file(
  GLOB
  SokobanDemo-sources
  ../SokobanDemo/*.cpp
)

add_executable(SokobanDemo
  ${SokobanDemo-sources}
)

add_library(PluginSokobanDemo
  SHARED
  ${SokobanDemo-sources}
)

target_link_libraries(SokobanDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  Nc-Contrib
  GL
)

target_link_libraries(PluginSokobanDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  Nc-Contrib
  GL
)
