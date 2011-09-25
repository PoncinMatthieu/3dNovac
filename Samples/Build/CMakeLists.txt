cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(3dNovac-Samples)

set(EXECUTABLE_OUTPUT_PATH ../bin/)
set(LIBRARY_OUTPUT_PATH ../../lib/)
set(CMAKE_FILE_DIRECTORY ../files/)
set(CMAKE_BINARY_DIR ../files/)



if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(-D_DEBUG)
endif()

include_directories(../../)
include_directories(../../extlibs/)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	link_directories(../../lib/Debug/)
else()
	link_directories(../../lib/Release/)
endif()


#------------------------------------------------------------------------------
#	Tuto1
#------------------------------------------------------------------------------
file(
  GLOB
  Tuto1-sources
  ../Tuto1/*.cpp
  ../Tuto1/*.h
)

add_executable(Tuto1
  ${Tuto1-sources}
)

if(UNIX)

	target_link_libraries(
		Tuto1
	  Nc-Core
	  Nc-Graphics
	  GL
	)
	
else()

	target_link_libraries(
		Tuto1
	  Nc-Core
	  Nc-Graphics
	  ../extlibs/freetype
	  opengl32
	)
	
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

add_executable(Tuto_NcCore_Server ${Tuto_NcCore_Server-sources})

target_link_libraries(Tuto_NcCore_Server 
	Nc-Core
)


#------------------------------------------------------------------------------
#	Tuto_NcCore_Client
#------------------------------------------------------------------------------
file(
  GLOB
  Tuto_NcCore_Client-sources
  ../Tuto_NcCore_Client/*.cpp
  ../Tuto_NcCore_Client/*.h
)

add_executable(Tuto_NcCore_Client ${Tuto_NcCore_Client-sources})

target_link_libraries(Tuto_NcCore_Client 
	Nc-Core
)


#------------------------------------------------------------------------------
#	Frustum Demo
#------------------------------------------------------------------------------
file(
  GLOB
  FrustumDemo-sources
  ../FrustumDemo/*.cpp
  ../FrustumDemo/*.h
)

add_executable(FrustumDemo
  ${FrustumDemo-sources}
)

add_library(PluginFrustumDemo
  SHARED
  ${FrustumDemo-sources}
)

IF(UNIX)

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

ELSE()

target_link_libraries(FrustumDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  opengl32
)

target_link_libraries(PluginFrustumDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  opengl32
)


ENDIF()


#------------------------------------------------------------------------------
#	GUI Demo
#------------------------------------------------------------------------------
file(
  GLOB
  GUIDemo-sources
  ../GUIDemo/*.cpp
  ../GUIDemo/*.h
)

add_executable(GUIDemo
  ${GUIDemo-sources}
)

add_library(PluginGUIDemo
  SHARED
  ${GUIDemo-sources}
)

IF(UNIX)

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

ELSE()

target_link_libraries(GUIDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  opengl32
)

target_link_libraries(PluginGUIDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  opengl32
)

ENDIF()


#------------------------------------------------------------------------------
#	Light Demo
#------------------------------------------------------------------------------
file(
  GLOB
  LightDemo-sources
  ../LightDemo/*.cpp
  ../LightDemo/*.h
)

add_executable(LightDemo
  ${LightDemo-sources}
)

add_library(PluginLightDemo
  SHARED
  ${LightDemo-sources}
)

IF(UNIX)

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

ELSE()

target_link_libraries(LightDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  opengl32
)

target_link_libraries(PluginLightDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  opengl32
)

ENDIF()


#------------------------------------------------------------------------------
#	bSplineDemo Demo
#------------------------------------------------------------------------------
file(
  GLOB
  bSplinesDemo-sources
  ../bSplinesDemo/*.cpp
  ../bSplinesDemo/*.h
)

add_executable(bSplinesDemo
  ${bSplinesDemo-sources}
)

add_library(PluginbSplinesDemo
  SHARED
  ${bSplinesDemo-sources}
)


if(UNIX)
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

ELSE()

target_link_libraries(bSplinesDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  ../extlibs/freetype
  opengl32
)

target_link_libraries(PluginbSplinesDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  ../extlibs/freetype
  opengl32
)

ENDIF()


#------------------------------------------------------------------------------
#	FBEffects Demo
#------------------------------------------------------------------------------
file(
  GLOB
  FBEffectsDemo-sources
  ../FBEffectsDemo/*.cpp
  ../FBEffectsDemo/*.h
)

add_executable(FBEffectsDemo
  ${FBEffectsDemo-sources}
)

add_library(PluginFBEffectsDemo
  SHARED
  ${FBEffectsDemo-sources}
)

IF(UNIX)

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

ELSE()

target_link_libraries(FBEffectsDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  ../extlibs/freetype
  opengl32
)

target_link_libraries(PluginFBEffectsDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  ../extlibs/freetype
  opengl32
)

ENDIF()

#------------------------------------------------------------------------------
#	Sokoban Demo
#------------------------------------------------------------------------------
file(
  GLOB
  SokobanDemo-sources
  ../SokobanDemo/*.cpp
  ../SokobanDemo/*.h
)

add_executable(SokobanDemo
  ${SokobanDemo-sources}
)

add_library(PluginSokobanDemo
  SHARED
  ${SokobanDemo-sources}
)

IF(UNIX)

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

ELSE()

target_link_libraries(SokobanDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  Nc-Contrib
  opengl32
)

target_link_libraries(PluginSokobanDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  Nc-Contrib
  opengl32
)

ENDIF()

# we want to compile our libs dynamically
add_definitions(-DDYNAMIC_LINK)
