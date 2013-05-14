
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

    #include <Nc/Graphics/Graphics.h>

// linking option (static or shared)
    #ifdef SYSTEM_WINDOWS                                       // windows
        #ifdef NC_DYNAMIC_LINK                                     // dynamic
            #ifdef NC_EXPORT_GUI
                #define LIB_NC_GUI     __declspec(dllexport)
            #else
                #define LIB_NC_GUI     __declspec(dllimport)
            #endif
        #else                                                   // static (do nothing)
            #define LIB_NC_GUI
        #endif
    #else                                                       // othes os (do nothing)
        #define LIB_NC_GUI
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
        /// This namespace provide many Widget and a SceneGraph to create and render a 2d Graphical User Interface.
        namespace GUI
        {
            // definitions
            struct ILook;

            class Widget;
            class Item;
            class TreeWidget;

            // typedef
            typedef std::list<Widget*>      ListPWidget;
            typedef std::list<Item*>        ListPItem;
            //typedef Graph::Node<Item*>      ItemNode;

            /// Define a set of visitor aiming to visit widget.
            namespace Visitor
            {
                template<typename VisitorType, bool IsConst = false, typename ReturnType = void>
                class WidgetVisitor;

                struct CheckFocus;
            }
        }
    }

    #include "Enum.h"

#endif
