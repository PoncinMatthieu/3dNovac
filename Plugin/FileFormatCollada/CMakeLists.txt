
cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)

project(Nc-Plugin-FileFormatCollada)

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
include_directories(${NC_ROOT}/extlibs/Collada/)
link_directories(${NC_ROOT}/lib/${CMAKE_CFG_INTDIR})
if(UNIX)
  link_directories(${NC_ROOT}/extlibs/Collada/lib/)
  link_directories(${NC_ROOT}/extlibs/Collada/extlibs/)
else()
  include_directories(${NC_ROOT}/extlibs/Collada/extlibs/boost/)
  link_directories(${NC_ROOT}/extlibs/)
  link_directories(${NC_ROOT}/extlibs/Collada/lib/)
  link_directories(${NC_ROOT}/extlibs/Collada/extlibs/)
  link_directories(${NC_ROOT}/extlibs/Collada/extlibs/boost/lib/)
endif()

# we want to compile our libs dynamically
add_definitions(-DNC_DYNAMIC_LINK)
add_definitions(-DBOOST_ALL_NO_LIB -DPCRE_STATIC -DDOM_INCLUDE_LIBXML)

# Set the ColladaPlugin sources path
file(GLOB PluginFileFormatCollada-sources       ./*.cpp ./*.h)

add_library(Nc-Plugin-FileFormatCollada SHARED ${PluginFileFormatCollada-sources})

if(UNIX)
  target_link_libraries(Nc-Plugin-FileFormatCollada collada14dom minizip xml2 pcre pcrecpp boost_filesystem boost_system)
else()
  target_link_libraries(Nc-Plugin-FileFormatCollada general opengl32
						general         wsock32
						general         freetype
						general         libxml2_a
                                                general         zlib
                                                general         minizip
                                                general         Nc-Core
                                                general         Nc-Graphics
                                                debug           libcollada14dom22-d
                                                debug           boost_system-d
                                                debug           pcred
                                                debug           pcrecppd
                                                optimized       libcollada14dom22
                                                optimized       boost_system
                                                optimized       pcre
                                                optimized       pcrecpp)
endif()

add_dependencies(Nc-Plugin-FileFormatCollada	Nc-Core Nc-Graphics)

# set the export defines to export the symbole in the dll
# those defines are more likely more usefull for Windows, but it could be interesting to keep these define on the other systems
set_target_properties(Nc-Plugin-FileFormatCollada PROPERTIES DEFINE_SYMBOL EXPORT_PLUGIN_FILEFORMATCOLLADA)

