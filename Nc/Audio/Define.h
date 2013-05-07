
/*-----------------------------------------------------------------------------

	3dNovac Audio
	Copyright (C) 2010-2011, The 3dNovac Team

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

    File Created At:        20/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_AUDIO_DEFINE_H_
#define NC_AUDIO_DEFINE_H_

    #include <Nc/Core/Define.h>

// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef NC_DYNAMIC_LINK                                      // dynamic
            #ifdef NC_EXPORT_AUDIO
                #define LIB_NC_AUDIO          __declspec(dllexport)
            #else
                #define LIB_NC_AUDIO          __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LIB_NC_AUDIO
        #endif
    #else                                                       // others os (do nothing)
        #define LIB_NC_AUDIO                  ///< Linking option of Nc-Audio module
    #endif

#endif
