cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(3dNovac-Samples)

set(EXECUTABLE_OUTPUT_PATH ../bin/)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(-D_DEBUG)
endif()

include_directories(../../)
link_directories(../../lib/)

file(
  GLOB
  Tuto1-sources
  ../Tuto1/*.cpp
)

file(
  GLOB
  FrustumDemo-sources
  ../FrustumDemo/*.cpp
)

file(
  GLOB
  GUIDemo-sources
  ../GUIDemo/*.cpp
)

file(
  GLOB
  LightDemo-sources
  ../LightDemo/*.cpp
)

file(
  GLOB
  bSplinesDemo-sources
  ../bSplinesDemo/*.cpp
)

add_executable(
  Tuto1
  ${Tuto1-sources}
)

add_executable(
  GUIDemo
  ${GUIDemo-sources}
)

add_executable(LightDemo
  ${LightDemo-sources}
)

add_executable(FrustumDemo
  ${FrustumDemo-sources}
)

add_executable(bSplinesDemo
  ${bSplinesDemo-sources}
)

target_link_libraries(
  Tuto1
  Nc-Core
  Nc-Graphics
  GL
)

target_link_libraries(
  GUIDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)

target_link_libraries(LightDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)

target_link_libraries(FrustumDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)

target_link_libraries(bSplinesDemo
  Nc-Core
  Nc-Graphics
  Nc-GUI
  GL
)
