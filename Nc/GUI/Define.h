
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        04/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                            Define all constant

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_DEFINE_H_
#define NC_GUI_DEFINE_H_

    #include <Nc/Define.h>
    #include <Nc/Graphics/Graphics.h>

    //#define NC_DYNAMIC_LINK
    //#define EXPORT_MATH

// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef NC_DYNAMIC_LINK                                     // dynamic
            #ifdef EXPORT_GUI
                #define LGUI     __declspec(dllexport)
            #else
                #define LGUI     __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LGUI
        #endif
    #else                                                       // othes os (do nothing)
        #define LGUI
    #endif

// includes interne

    #ifdef _DEBUG
        //#define _DEBUG_GUI
        //#define _DEBUG_GUI_BOX
        //#define _DEBUG_GUI_FOCUS
        //#define _DEBUG_GUI_DISABLE_CONSOLE_LOGGING
    #endif

    namespace Nc
    {
        namespace GUI
        {
            // definition of the classes to typedef the diferrent types
            class Widget;

            // typedef
            typedef std::list<Widget*>      ListPWidget;
        }
    }

#endif
