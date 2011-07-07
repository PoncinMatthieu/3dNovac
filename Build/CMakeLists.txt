cmake_minimum_required(VERSION 2.8)

project(3dNovac)

set(LIBRARY_OUTPUT_PATH ../lib/)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(-D_DEBUG)
endif()

include_directories(
  ../
  /usr/include/freetype2/
  /usr/local/include/colladadom/
  /usr/local/include/colladadom/1.4/
)

file(
  GLOB
  Nc-Core-sources
  ../Nc/*.h
  ../Nc/Core/*.cpp
  ../Nc/Core/*.h
  ../Nc/Core/Utils/*.cpp
  ../Nc/Core/Utils/*.h
  ../Nc/Core/Utils/Debug/*.cpp
  ../Nc/Core/Utils/Debug/*.h
  ../Nc/Core/Utils/Xml/*.cpp
  ../Nc/Core/Utils/Xml/*.h
  ../Nc/Core/System/*.cpp
  ../Nc/Core/System/*.h
  ../Nc/Core/System/API/*.cpp
  ../Nc/Core/System/API/*.h
  ../Nc/Core/System/Input/*.cpp
  ../Nc/Core/System/Input/*.h
  ../Nc/Core/Math/*.cpp
  ../Nc/Core/Math/*.h
  ../Nc/Core/Graph/*.cpp
  ../Nc/Core/Graph/*.h
  ../Nc/Core/Engine/*.cpp
  ../Nc/Core/Engine/*.h
)

file(
  GLOB
  Nc-Graphics-sources
  ../Nc/Graphics/*.cpp
  ../Nc/Graphics/Camera/*.cpp
  ../Nc/Graphics/Core/*.cpp
  ../Nc/Graphics/Core/GL/*.cpp
  ../Nc/Graphics/Effect/*.cpp
  ../Nc/Graphics/Material/*.cpp
  ../Nc/Graphics/Object/*.cpp
  ../Nc/Graphics/Scene/*.cpp
  ../Nc/Graphics/SpacePartitioning/*.cpp
  ../Nc/Graphics/Window/*.cpp
  ../Nc/Graphics/Window/Window/*.cpp
  ../Nc/Graphics/Window/Context/*.cpp
  ../Nc/Graphics/Window/Input/*.cpp
)

file(
  GLOB
  Linux_CPP
  ../Nc/Graphics/Window/Window/X*.cpp
  ../Nc/Graphics/Window/Context/GLX*.cpp
  ../Nc/Graphics/Window/Input/XW*.cpp
)

file(
  GLOB
  Win32_CPP
  ../Nc/Graphics/Window/Window/WW*.cpp
  ../Nc/Graphics/Window/Window/WC*.cpp
  ../Nc/Graphics/Window/Context/WGL*.cpp
  ../Nc/Graphics/Window/Input/WW*.cpp
)

if(WIN32)
  list(REMOVE_ITEM Nc-Graphics-sources ${Linux_CPP})
else()
  list(REMOVE_ITEM Nc-Graphics-sources ${Win32_CPP})
endif()

file(
  GLOB
  Nc-GUI-sources
  ../Nc/GUI/*.cpp
)

file(
  GLOB
  Nc-Contrib-sources
  ../Nc/Contrib/*.cpp
)

file(
  GLOB
  Nc-Audio-sources
  ../Nc/Audio/*.cpp
)

file(
  GLOB
  PluginFileFormatCollada-sources
  ../Plugin/FileFormatCollada/*.cpp
)

add_library(
  Nc-Core
  SHARED
  ${Nc-Core-sources}
)

add_library(
  Nc-Graphics
  SHARED
  ${Nc-Graphics-sources}
)

add_library(
  Nc-GUI
  SHARED
  ${Nc-GUI-sources}
)

add_library(
  Nc-Contrib
  SHARED
  ${Nc-Contrib-sources}
)

add_library(
  Nc-Audio
  SHARED
  ${Nc-Audio-sources}
)

add_library(
  PluginFileFormatCollada
  SHARED
  ${PluginFileFormatCollada-sources}
)

target_link_libraries(
  Nc-Graphics
  freetype
  png
  X11
  Xrandr
)
