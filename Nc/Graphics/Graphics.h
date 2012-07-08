
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

        include all graphics component

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GRAPHICS_H_
#define NC_GRAPHICS_GRAPHICS_H_

	#include "Define.h"

// Window
    #ifdef SYSTEM_WINDOWS
        #include "Window/WWindow.h"
    #else
        #include "Window/XWindow.h"
    #endif
    #include "Context/GLContext.h"
    #include "Input/WindowInput.h"

// Object
    #include "Object/Text.h"

// Scene
    #include "Scene/SceneGraph.h"
    #include "Engine.h"

#endif
