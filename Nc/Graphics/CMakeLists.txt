
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(Nc-Graphics)

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
include_directories(${NC_ROOT}/extlibs/)
include_directories(${NC_ROOT}/extlibs/freetype/)
include_directories(${NC_ROOT}/extlibs/GL3/)
include_directories(${NC_ROOT}/extlibs/libpng/)
include_directories(${NC_ROOT}/extlibs/zlib/)
link_directories(${NC_ROOT}/lib/${CMAKE_CFG_INTDIR})
if(UNIX)
else()
  link_directories(../extlibs/)
endif()

# we want to compile our libs dynamically
add_definitions(-DNC_DYNAMIC_LINK)


# Set the Nc-Graphics sources path
file(GLOB_RECURSE Nc-Graphics-sources
  ./*.cpp
  ./*.h
)

# Identify the Nc-Graphics linux dependant sources path
file(GLOB Linux_CPP
  ./Window/X*.cpp          ./Window/X*.h
  ./Context/GLX*.cpp       ./Context/GLX*.h
  ./Input/XW*.cpp          ./Input/XW*.h
)

# Identify the Nc-Graphics windows dependant sources path
file(GLOB Win32_CPP
  ./Window/WW*.cpp         ./Window/WW*.h
  ./Window/WC*.cpp         ./Window/WC*.h
  ./Context/WGL*.cpp       ./Context/WGL*.h
  ./Input/WW*.cpp          ./Input/WW*.h
)

# So If we are on windows, just remove the linux dependant files and compile directly the libpng in the Nc-Graphics to avoid another dll
# on linux, remove the windows dependant files
if(WIN32)
  list(REMOVE_ITEM Nc-Graphics-sources ${Linux_CPP})
        file(GLOB libpngzlib-sources
          ${NC_ROOT}/extlibs/libpng/*.c                 ${NC_ROOT}/extlibs/libpng/*.h
          ${NC_ROOT}/extlibs/zlib/*.c                   ${NC_ROOT}/extlibs/zlib/*.h
	  )
  list(APPEND Nc-Graphics-sources ${libpngzlib-sources})
else()
  list(REMOVE_ITEM Nc-Graphics-sources ${Win32_CPP})
endif()



add_library(Nc-Graphics SHARED ${Nc-Graphics-sources})

if(UNIX)
  target_link_libraries(Nc-Graphics Nc-Core freetype GL png X11 Xrandr)
else()
  target_link_libraries(Nc-Graphics Nc-Core freetype opengl32)
endif()

add_dependencies(Nc-Graphics	Nc-Core)

# set the export defines to export the symbole in the dll
# those defines are more likely more usefull for Windows, but it could be interesting to keep these define on the other systems
set_target_properties(Nc-Graphics PROPERTIES DEFINE_SYMBOL EXPORT_GRAPHICS)

# copy dll and .pdb files for excutable dependencies if NC_COPY_DEPENDENCIES_PATH is set
if (WIN32)
  if(NC_COPY_DEPENDENCIES_PATH)
     add_custom_command(TARGET Nc-Graphics POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${NC_DIR}/lib/${CMAKE_CFG_INTDIR}/Nc-Graphics.dll ${NC_COPY_DEPENDENCIES_PATH}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

     if (MSVC_IDE)
       if (${CMAKE_CFG_INTDIR} MATCHES "Debug")
	 add_custom_command(TARGET Nc-Graphics POST_BUILD COMMAND "${CMAKE_COMMAND}" -E copy ${NC_DIR}/lib/${CMAKE_CFG_INTDIR}/Nc-Graphics.pdb ${NC_COPY_DEPENDENCIES_PATH}/ WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
       endif()
     endif()
   endif()
endif()
