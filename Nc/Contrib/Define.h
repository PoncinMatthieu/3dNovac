
/*-----------------------------------------------------------------------------

	3dNovac Contrib
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

    File Created At:        13/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CONTRIB_DEFINE_H_
#define NC_CONTRIB_DEFINE_H_

#include <Nc/Graphics/Graphics.h>

// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef DYNAMIC_LINK                                      // dynamic
            #ifdef EXPORT_CONTRIB
                #define LCONTRIB    __declspec(dllexport)
            #else
                #define LCONTRIB    __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LCONTRIB
        #endif
    #else                                                       // others os (do nothing)
        #define LCONTRIB            ///< Linking option of Nc-Contrib module
    #endif

#endif // NC_CONTRIB_DEFINE_H_
