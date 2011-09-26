cmake_minimum_required(VERSION 2.8)

# project name
project(3dNovac)

# libraries output path
set(LIBRARY_OUTPUT_PATH ../lib/)

# force the build type to release if the type is not already set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# set the include directories
if(UNIX)
	include_directories(
	  ../
	  ../extlibs/
	  ../extlibs/freetype/
	  ../extlibs/GL3/
	  ../extlibs/libpng/
	  ../extlibs/zlib/
	  ../extlibs/SFML/include/
#	  /usr/include/freetype2/
	  /usr/local/include/colladadom/
	  /usr/local/include/colladadom/1.4/
	)
else()
	include_directories(
	  ../
	  ../extlibs/
	  ../extlibs/freetype/
	  ../extlibs/GL3/
	  ../extlibs/libpng/
	  ../extlibs/zlib/
	  ../extlibs/SFML/include/
	  ../extlibs/Collada/
	  ../extlibs/Collada/extlibs/boost/
	)
endif()

# Set the Nc-Core sources path only the API folder is not set
# we use a global and not a global recurse to avoid the svn directories and because some folder shouldn't be added to the compilation process
file(GLOB Nc-Core-sources
  ../Nc/*.h
  ../Nc/Core/*.cpp							../Nc/Core/*.h
  ../Nc/Core/Utils/*.cpp					../Nc/Core/Utils/*.h
  ../Nc/Core/Utils/Debug/*.cpp				../Nc/Core/Utils/Debug/*.h
  ../Nc/Core/Utils/Xml/*.cpp				../Nc/Core/Utils/Xml/*.h
  ../Nc/Core/System/*.cpp					../Nc/Core/System/*.h
  ../Nc/Core/System/API/*.cpp				../Nc/Core/System/API/*.h
  ../Nc/Core/System/Input/*.cpp				../Nc/Core/System/Input/*.h
  ../Nc/Core/Math/*.cpp						../Nc/Core/Math/*.h
  ../Nc/Core/Graph/*.cpp					../Nc/Core/Graph/*.h
  ../Nc/Core/Engine/*.cpp					../Nc/Core/Engine/*.h
  ../Nc/Core/Network/*.cpp					../Nc/Core/Network/*.h
)

# Set the Nc-Graphics sources path
file(GLOB Nc-Graphics-sources
  ../Nc/Graphics/*.cpp						../Nc/Graphics/*.h
  ../Nc/Graphics/Camera/*.cpp				../Nc/Graphics/Camera/*.h
  ../Nc/Graphics/Core/*.cpp					../Nc/Graphics/Core/*.h
  ../Nc/Graphics/Core/GL/*.cpp				../Nc/Graphics/Core/GL/*.h
  ../Nc/Graphics/Effect/*.cpp				../Nc/Graphics/Effect/*.h
  ../Nc/Graphics/Material/*.cpp				../Nc/Graphics/Material/*.h
  ../Nc/Graphics/Object/*.cpp				../Nc/Graphics/Object/*.h
  ../Nc/Graphics/Scene/*.cpp				../Nc/Graphics/Scene/*.h
  ../Nc/Graphics/SpacePartitioning/*.cpp	../Nc/Graphics/SpacePartitioning/*.h
  ../Nc/Graphics/Window/*.cpp				../Nc/Graphics/Window/*.h
  ../Nc/Graphics/Window/Window/*.cpp		../Nc/Graphics/Window/Window/*.h
  ../Nc/Graphics/Window/Context/*.cpp		../Nc/Graphics/Window/Context/*.h
  ../Nc/Graphics/Window/Input/*.cpp			../Nc/Graphics/Window/Input/*.h
)

# Identify the Nc-Graphics linux dependant sources path
file(GLOB Linux_CPP
  ../Nc/Graphics/Window/Window/X*.cpp		../Nc/Graphics/Window/Window/X*.h
  ../Nc/Graphics/Window/Context/GLX*.cpp	../Nc/Graphics/Window/Context/GLX*.h
  ../Nc/Graphics/Window/Input/XW*.cpp		../Nc/Graphics/Window/Input/XW*.h
)

# Identify the Nc-Graphics windows dependant sources path
file(GLOB Win32_CPP
  ../Nc/Graphics/Window/Window/WW*.cpp		../Nc/Graphics/Window/Window/WW*.h
  ../Nc/Graphics/Window/Window/WC*.cpp		../Nc/Graphics/Window/Window/WC*.h
  ../Nc/Graphics/Window/Context/WGL*.cpp	../Nc/Graphics/Window/Context/WGL*.h
  ../Nc/Graphics/Window/Input/WW*.cpp		../Nc/Graphics/Window/Input/WW*.h
)

# So If we are on windows, just remove the linux dependant files and compile directly the libpng in the Nc-Graphics to avoid another dll
# on linux, remove the windows dependant files
if(WIN32)
  list(REMOVE_ITEM Nc-Graphics-sources ${Linux_CPP})

	file(GLOB libpngzlib-sources
	  ../extlibs/libpng/*.c 		../extlibs/libpng/*.h
	  ../extlibs/zlib/*.c 			../extlibs/zlib/*.h
	)
  list(APPEND Nc-Graphics-sources ${libpngzlib-sources})
else()
  list(REMOVE_ITEM Nc-Graphics-sources ${Win32_CPP})
endif()

# Set the Nc-GUI / Nc-Contrib / Nc-Audio sources path
file(GLOB Nc-GUI-sources 		../Nc/GUI/*.cpp 		../Nc/GUI/*.h)
file(GLOB Nc-Contrib-sources	../Nc/Contrib/*.cpp 	../Nc/Contrib/*.h)
file(GLOB Nc-Audio-sources		../Nc/Audio/*.cpp 		../Nc/Audio/*.h)

# Set the ColladaPlugin sources path
file(GLOB PluginFileFormatCollada-sources	../Plugin/FileFormatCollada/*.cpp ../Plugin/FileFormatCollada/*.h)

# Create the Nc-Modules and plugins libraries
add_library(Nc-Core 					SHARED ${Nc-Core-sources})
add_library(Nc-Graphics 				SHARED ${Nc-Graphics-sources})
add_library(Nc-GUI 						SHARED ${Nc-GUI-sources})
add_library(Nc-Contrib 					SHARED ${Nc-Contrib-sources})
add_library(Nc-Audio 					SHARED ${Nc-Audio-sources})
add_library(PluginFileFormatCollada 	SHARED ${PluginFileFormatCollada-sources})

# Set the dependance libs
if(UNIX)
	target_link_libraries(Nc-Core 							pthread)
	target_link_libraries(Nc-Graphics 						freetype png X11 Xrandr)
	target_link_libraries(PluginFileFormatCollada 			collada14dom minizip xml2 pcre pcrecpp boost_filesystem)
else()
	target_link_libraries(Nc-Graphics	general		../extlibs/freetype
										general 	opengl32
										debug		../lib/Debug/Nc-Core
										optimized	../lib/Release/Nc-Core
										)

	target_link_libraries(Nc-Audio 		general		../extlibs/SFML/lib/sfml-audio
										debug		../lib/Debug/Nc-Core
										optimized	../lib/Release/Nc-Core
										)

	target_link_libraries(Nc-GUI 		general 	../extlibs/freetype opengl32
										debug 		../lib/Debug/Nc-Core
										debug		../lib/Debug/Nc-Graphics
										optimized	../lib/Release/Nc-Core
										optimized	../lib/Release/Nc-Graphics
										)

	target_link_libraries(Nc-Contrib 	general 	../extlibs/freetype opengl32
										debug 		../lib/Debug/Nc-Core
										debug		../lib/Debug/Nc-Graphics
										optimized	../lib/Release/Nc-Core
										optimized	../lib/Release/Nc-Graphics
										)

	target_link_libraries(PluginFileFormatCollada	general 	opengl32 
													general		wsock32 
													general		../extlibs/freetype 
													general		../extlibs/Collada/extlibs/libxml2_a 
													general		../extlibs/Collada/extlibs/zlib 
													general		../extlibs/Collada/extlibs/minizip
													debug		../lib/Debug/Nc-Core
													debug		../lib/Debug/Nc-Graphics
													debug		../extlibs/Collada/lib/libcollada14dom22-d
													debug		../extlibs/Collada/extlibs/boost/lib/boost_system-d
													debug		../extlibs/Collada/extlibs/pcred
													debug		../extlibs/Collada/extlibs/pcrecppd
													optimized	../lib/Release/Nc-Core
													optimized	../lib/Release/Nc-Graphics
													optimized	../extlibs/Collada/lib/libcollada14dom22
													optimized	../extlibs/Collada/extlibs/boost/lib/boost_system
													optimized	../extlibs/Collada/extlibs/pcre
													optimized 	../extlibs/Collada/extlibs/pcrecpp
													)
endif()

# we want to compile our libs dynamically
add_definitions(-DNC_DYNAMIC_LINK)

# set the export defines to export the symbole in the dll
# those defines are more likely more usefull for Windows, but it could be interesting to keep these define on the other systems
set_target_properties(Nc-Core 					PROPERTIES DEFINE_SYMBOL EXPORT_CORE)
set_target_properties(Nc-Graphics				PROPERTIES DEFINE_SYMBOL EXPORT_GRAPHICS)
set_target_properties(Nc-GUI 					PROPERTIES DEFINE_SYMBOL EXPORT_GUI)
set_target_properties(Nc-Audio 					PROPERTIES DEFINE_SYMBOL EXPORT_AUDIO)
set_target_properties(Nc-Contrib 				PROPERTIES DEFINE_SYMBOL EXPORT_CONTRIB)
set_target_properties(PluginFileFormatCollada	PROPERTIES DEFINE_SYMBOL EXPORT_FILEOBJPLUGIN)

# define _DEBUG
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  add_definitions(-D_DEBUG)
endif()

# for the PluginFileFormatCollada, I didn't find how to set more than one define in a target, so I just put it globaly
# that sucks I know. If you have a solution, tell me about it !
add_definitions(-DBOOST_ALL_NO_LIB -DPCRE_STATIC -DDOM_INCLUDE_LIBXML)

# Set les dependances entre projets
add_dependencies(Nc-Graphics 				Nc-Core)
add_dependencies(Nc-GUI						Nc-Graphics Nc-Core)
add_dependencies(Nc-Audio					Nc-Core)
add_dependencies(Nc-Contrib					Nc-Core Nc-Graphics)
add_dependencies(PluginFileFormatCollada	Nc-Core Nc-Graphics)
