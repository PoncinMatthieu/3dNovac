
/*-----------------------------------------------------------------------------

	3dNovac FileObj Plugin
	Copyright (C) 2010-2011, 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        28/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_FILE_OBJ_PLUGIN_DEFINE_H_
#define NC_FILE_OBJ_PLUGIN_DEFINE_H_

    #include <Nc/Core/Define.h>

// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef DYNAMIC_LINK                                      // dynamic
            #ifdef EXPORT_FILEOBJPLUGIN
                #define LFILEOBJPLUGIN       __declspec(dllexport)
            #else
                #define LFILEOBJPLUGIN       __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LFILEOBJPLUGIN
        #endif
    #else                                                       // others os (do nothing)
        #define LFILEOBJPLUGIN
    #endif


    #ifdef LOG
        #undef LOG
        #undef LOG_ERROR
        #undef LOG_DEBUG
    #endif

    #if defined(_DEBUG) && defined(_DEBUG_LOG)
        #define _DEBUG_LOG_DEF  __FILE__, __LINE__,
    #else
        #define _DEBUG_LOG_DEF
    #endif

    #define LOG             Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 0) << "FormatCollada: "
    #define LOG_ERROR       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 1) << "FormatCollada: "
    #ifdef _DEBUG
        #define LOG_DEBUG       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF 2) << "FormatCollada: "
    #else
        #define LOG_DEBUG       Nc::Utils::Logger::Log(_DEBUG_LOG_DEF -1) << "FormatCollada: "
    #endif

#endif
