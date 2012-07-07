
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

#include <sys/types.h>
#include "GLContext.h"
#include "../Window/IWindow.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;

GLContext::GLContext(IWindow *win) : _win(win)
{
    if (_win == NULL)
        throw Utils::Exception("GLContext", "Can't create any Renderer if the window is null");
    _isCreate = false;
    _isShared = false;
	_currentThreadId = 0;
}

GLContext::~GLContext()
{
}
