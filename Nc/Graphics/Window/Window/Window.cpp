
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

#include "Window.h"
#include "ICursor.h"
#include "../../Scene/SceneGraphManager.h"
#include "SubWindow.h"

using namespace Nc;

Graphic::Window::Window(SceneGraphManager *sceneGraphManager)
	: _sceneGraphManager(sceneGraphManager), _isCreate(false), _input(NULL), _own(false), _defaultCursor(NULL), _currentCursor(NULL)
{
    _antialiasingLevel = 0;
    _depth = 24;
    _stencil = 8;
    _bitsPerPixel = 32;
    _width = 0;
    _height = 0;

    if (_sceneGraphManager == NULL)
        _sceneGraphManager = new SceneGraphManager();
}

Graphic::Window::~Window()
{
    if (_defaultCursor != NULL)
        delete _defaultCursor;
}

void    Graphic::Window::InitSceneGraphManager()
{
    _sceneGraphManager->Init((_antialiasingLevel > 0));
}

void    Graphic::Window::Render(GLContext *context)
{
    // Render the scene graph manager
    _sceneGraphManager->Render(_context);

    // Render the sub windows
    _mutexListSubWindow.Lock();
    for (ListSubWindow::iterator it = _listSubWindow.begin(); it != _listSubWindow.end(); ++it)
        (*it)->Render(context);
    _mutexListSubWindow.Unlock();
}

Graphic::GLContext   *Graphic::Window::CreateSharedContext()
{
    return _context->CreateNewSharedContext();
}

void    Graphic::Window::RemoveSubWindow(SubWindow *w)
{
    _mutexListSubWindow.Lock();
    _listSubWindow.remove(w);
    _mutexListSubWindow.Unlock();
}

void    Graphic::Window::AddSubWindow(SubWindow *w)
{
    _mutexListSubWindow.Lock();
    _listSubWindow.push_back(w);
    _mutexListSubWindow.Unlock();
}
