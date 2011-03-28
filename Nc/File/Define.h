
/*-----------------------------------------------------------------------------

	3dNovac File
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

    File Created At:        05/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

        Define all constants of Nc-File

-----------------------------------------------------------------------------*/

#ifndef NC_FILE_DEFINE_H_
#define NC_FILE_DEFINE_H_

    #include <Nc/Define.h>
    #include <Utils/Utils.h>
    #include <Nc/Math/Math.h>
    #include <Nc/Graphics/Graphics.h>

    //#define DYNAMIC_LINK
    //#define EXPORT_FILE

/// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef DYNAMIC_LINK                                     // dynamic
            #ifdef EXPORT_FILE
                #define LFILE     __declspec(dllexport)
            #else
                #define LFILE     __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LFILE
        #endif
    #else                                                       // othes os (do nothing)
        #define LFILE
    #endif

/// includes interne

#endif
