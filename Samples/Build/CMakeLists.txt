cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(3dNovac-Samples)

set(EXECUTABLE_OUTPUT_PATH ../bin/)
set(LIBRARY_OUTPUT_PATH ../../lib/)
#set(CMAKE_FILE_DIRECTORY ../files/)
#set(CMAKE_BINARY_DIR ../files/)



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

include_directories(../../)
include_directories(../../extlibs/)
link_directories(../../lib/)


#------------------------------------------------------------------------------
#	Tuto1
#------------------------------------------------------------------------------
file(
  GLOB
  Tuto1-sources
  ../Tuto1/*.cpp
  ../Tuto1/*.h
)

add_executable(Tuto1	${Tuto1-sources})

if(UNIX)
	target_link_libraries(Tuto1 	Nc-Core Nc-Graphics GL)
else()
	target_link_libraries(Tuto1		debug 		../../lib/Debug/Nc-Core
									debug 		../../lib/Debug/Nc-Graphics
									optimized	../../lib/Release/Nc-Core
									optimized	../../lib/Release/Nc-Graphics
									general 	../../extlibs/freetype
									general		opengl32)
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

if(UNIX)
	target_link_libraries(Tuto_NcCore_Server	Nc-Core)
else()
	target_link_libraries(Tuto_NcCore_Server	debug		../../lib/Debug/Nc-Core
												optimized	../../lib/Release/Nc-Core)
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

add_executable(Tuto_NcCore_Client ${Tuto_NcCore_Client-sources})

if(UNIX)
	target_link_libraries(Tuto_NcCore_Client	Nc-Core)
else()
	target_link_libraries(Tuto_NcCore_Client	debug		../../lib/Debug/Nc-Core
												optimized	../../lib/Release/Nc-Core)
endif()

#------------------------------------------------------------------------------
#	Sample Viewer
#------------------------------------------------------------------------------
file(
  GLOB
  SampleViewer-sources
  ../SampleViewer/*.cpp
  ../SampleViewer/*.h
)

add_executable(SampleViewer	${SampleViewer-sources})
add_library(PluginSampleViewer	SHARED ${SampleViewer-sources})

if(UNIX)
	target_link_libraries(SampleViewer			Nc-Core Nc-Graphics Nc-GUI GL)
	target_link_libraries(PluginSampleViewer		Nc-Core Nc-Graphics Nc-GUI GL)
else()
	target_link_libraries(SampleViewer			debug 		../../lib/Debug/Nc-Core
											debug		../../lib/Debug/Nc-Graphics
											debug		../../lib/Debug/Nc-GUI
											optimized	../../lib/Release/Nc-Core
											optimized	../../lib/Release/Nc-Graphics
											optimized	../../lib/Release/Nc-GUI
											general		opengl32)

	target_link_libraries(PluginSampleViewer		debug 		../../lib/Debug/Nc-Core
											debug		../../lib/Debug/Nc-Graphics
											debug		../../lib/Debug/Nc-GUI
											optimized	../../lib/Release/Nc-Core
											optimized	../../lib/Release/Nc-Graphics
											optimized	../../lib/Release/Nc-GUI
											general		opengl32)
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

add_executable(FrustumDemo		${FrustumDemo-sources})
add_library(PluginFrustumDemo	SHARED ${FrustumDemo-sources})

if(UNIX)
	target_link_libraries(FrustumDemo			Nc-Core Nc-Graphics Nc-GUI GL)
	target_link_libraries(PluginFrustumDemo		Nc-Core Nc-Graphics Nc-GUI GL)
else()
	target_link_libraries(FrustumDemo			debug		../../lib/Debug/Nc-Core
												debug		../../lib/Debug/Nc-Graphics
												debug		../../lib/Debug/Nc-GUI
												optimized	../../lib/Release/Nc-Core
												optimized	../../lib/Release/Nc-Graphics
												optimized	../../lib/Release/Nc-GUI
												general		opengl32)

	target_link_libraries(PluginFrustumDemo		debug		../../lib/Debug/Nc-Core
												debug		../../lib/Debug/Nc-Graphics
												debug		../../lib/Debug/Nc-GUI
												optimized	../../lib/Release/Nc-Core
												optimized	../../lib/Release/Nc-Graphics
												optimized	../../lib/Release/Nc-GUI
												general		opengl32)
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

add_executable(GUIDemo		${GUIDemo-sources})
add_library(PluginGUIDemo	SHARED ${GUIDemo-sources})

if(UNIX)
	target_link_libraries(GUIDemo			Nc-Core Nc-Graphics Nc-GUI GL)
	target_link_libraries(PluginGUIDemo		Nc-Core Nc-Graphics Nc-GUI GL)
else()
	target_link_libraries(GUIDemo			debug 		../../lib/Debug/Nc-Core
											debug		../../lib/Debug/Nc-Graphics
											debug		../../lib/Debug/Nc-GUI
											optimized	../../lib/Release/Nc-Core
											optimized	../../lib/Release/Nc-Graphics
											optimized	../../lib/Release/Nc-GUI
											general		opengl32)

	target_link_libraries(PluginGUIDemo		debug 		../../lib/Debug/Nc-Core
											debug		../../lib/Debug/Nc-Graphics
											debug		../../lib/Debug/Nc-GUI
											optimized	../../lib/Release/Nc-Core
											optimized	../../lib/Release/Nc-Graphics
											optimized	../../lib/Release/Nc-GUI
											general		opengl32)
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

add_executable(LightDemo		${LightDemo-sources})
add_library(PluginLightDemo		SHARED ${LightDemo-sources})

if(UNIX)
	target_link_libraries(LightDemo			Nc-Core Nc-Graphics Nc-GUI GL)
	target_link_libraries(PluginLightDemo	Nc-Core Nc-Graphics Nc-GUI GL)
else()
	target_link_libraries(LightDemo			debug 		../../lib/Debug/Nc-Core
											debug		../../lib/Debug/Nc-Graphics
											debug		../../lib/Debug/Nc-GUI
											optimized	../../lib/Release/Nc-Core
											optimized	../../lib/Release/Nc-Graphics
											optimized	../../lib/Release/Nc-GUI
											general		opengl32)

	target_link_libraries(PluginLightDemo	debug 		../../lib/Debug/Nc-Core
											debug		../../lib/Debug/Nc-Graphics
											debug		../../lib/Debug/Nc-GUI
											optimized	../../lib/Release/Nc-Core
											optimized	../../lib/Release/Nc-Graphics
											optimized	../../lib/Release/Nc-GUI
											general		opengl32)
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

add_executable(bSplinesDemo		${bSplinesDemo-sources})
add_library(PluginbSplinesDemo	SHARED ${bSplinesDemo-sources})


if(UNIX)
	target_link_libraries(bSplinesDemo			Nc-Core Nc-Graphics Nc-GUI GL)
	target_link_libraries(PluginbSplinesDemo	Nc-Core Nc-Graphics Nc-GUI GL)
else()
	target_link_libraries(bSplinesDemo			debug 		../../lib/Debug/Nc-Core
												debug		../../lib/Debug/Nc-Graphics
												debug		../../lib/Debug/Nc-GUI
												optimized	../../lib/Release/Nc-Core
												optimized	../../lib/Release/Nc-Graphics
												optimized	../../lib/Release/Nc-GUI
												general		opengl32)

	target_link_libraries(PluginbSplinesDemo	debug 		../../lib/Debug/Nc-Core
												debug		../../lib/Debug/Nc-Graphics
												debug		../../lib/Debug/Nc-GUI
												optimized	../../lib/Release/Nc-Core
												optimized	../../lib/Release/Nc-Graphics
												optimized	../../lib/Release/Nc-GUI
												general		opengl32)
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

add_executable(FBEffectsDemo		${FBEffectsDemo-sources})
add_library(PluginFBEffectsDemo		SHARED ${FBEffectsDemo-sources})

if(UNIX)
	target_link_libraries(FBEffectsDemo			Nc-Core Nc-Graphics Nc-GUI GL)
	target_link_libraries(PluginFBEffectsDemo	Nc-Core Nc-Graphics Nc-GUI GL)
else()
	target_link_libraries(FBEffectsDemo			debug 		../../lib/Debug/Nc-Core
												debug		../../lib/Debug/Nc-Graphics
												debug		../../lib/Debug/Nc-GUI
												optimized	../../lib/Release/Nc-Core
												optimized	../../lib/Release/Nc-Graphics
												optimized	../../lib/Release/Nc-GUI
												general		opengl32)

	target_link_libraries(PluginFBEffectsDemo	debug 		../../lib/Debug/Nc-Core
												debug		../../lib/Debug/Nc-Graphics
												debug		../../lib/Debug/Nc-GUI
												optimized	../../lib/Release/Nc-Core
												optimized	../../lib/Release/Nc-Graphics
												optimized	../../lib/Release/Nc-GUI
												general		opengl32)
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

add_executable(SokobanDemo		${SokobanDemo-sources})
add_library(PluginSokobanDemo	SHARED ${SokobanDemo-sources})

if(UNIX)
	target_link_libraries(SokobanDemo			Nc-Core Nc-Graphics Nc-GUI Nc-Contrib GL)
	target_link_libraries(PluginSokobanDemo		Nc-Core Nc-Graphics Nc-GUI Nc-Contrib GL)
else()
	target_link_libraries(SokobanDemo			debug 		../../lib/Debug/Nc-Core
												debug		../../lib/Debug/Nc-Graphics
												debug		../../lib/Debug/Nc-GUI
												debug		../../lib/Debug/Nc-Contrib
												optimized	../../lib/Release/Nc-Core
												optimized	../../lib/Release/Nc-Graphics
												optimized	../../lib/Release/Nc-GUI
												optimized	../../lib/Release/Nc-Contrib
												general		opengl32)

	target_link_libraries(PluginSokobanDemo		debug 		../../lib/Debug/Nc-Core
												debug		../../lib/Debug/Nc-Graphics
												debug		../../lib/Debug/Nc-GUI
												debug		../../lib/Debug/Nc-Contrib
												optimized	../../lib/Release/Nc-Core
												optimized	../../lib/Release/Nc-Graphics
												optimized	../../lib/Release/Nc-GUI
												optimized	../../lib/Release/Nc-Contrib
												general		opengl32)
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

add_executable(SimpleParticleEffect		${SimpleParticleEffect-sources})
add_library(PluginSimpleParticleEffect		SHARED ${SimpleParticleEffect-sources})

if(UNIX)
	target_link_libraries(SimpleParticleEffect		Nc-Core Nc-Graphics Nc-GUI GL)
	target_link_libraries(PluginSimpleParticleEffect	Nc-Core Nc-Graphics Nc-GUI GL)
else()
	target_link_libraries(SimpleParticleEffect		debug 		../../lib/Debug/Nc-Core
								debug		../../lib/Debug/Nc-Graphics
								debug		../../lib/Debug/Nc-GUI
								optimized	../../lib/Release/Nc-Core
								optimized	../../lib/Release/Nc-Graphics
								optimized	../../lib/Release/Nc-GUI
								general		opengl32)

	target_link_libraries(PluginSimpleParticleEffect	debug 		../../lib/Debug/Nc-Core
								debug		../../lib/Debug/Nc-Graphics
								debug		../../lib/Debug/Nc-GUI
								optimized	../../lib/Release/Nc-Core
								optimized	../../lib/Release/Nc-Graphics
								optimized	../../lib/Release/Nc-GUI
								general		opengl32)
endif()


