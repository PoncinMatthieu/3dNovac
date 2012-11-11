
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

#include "IWindow.h"
#include "ICursor.h"
#include "../Scene/SceneGraphManager.h"
#include "SubWindow.h"

using namespace Nc;
using namespace Nc::Graphic;

IWindow::IWindow(SceneGraphManager *sceneGraphManager)
	: _sceneGraphManager(sceneGraphManager), _isCreate(false), _isEnable(false), _input(NULL), _own(false), _defaultCursor(NULL), _currentCursor(NULL)
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

IWindow::IWindow(const std::string &title, const Math::Vector2ui &size, const Utils::Mask<Style> &style, const Utils::FileName &icon, unsigned int antialiasingLevel, SceneGraphManager *sceneGraphManager)
	: _sceneGraphManager(sceneGraphManager), _isCreate(false), _isEnable(false), _input(NULL), _own(false), _defaultCursor(NULL), _currentCursor(NULL)
{
	_title = title;
	_icon = icon;
	_style = style;
	_antialiasingLevel = antialiasingLevel;
    _depth = 24;
    _stencil = 8;
    _bitsPerPixel = 32;
    _width = size[0];
    _height = size[1];

    if (_sceneGraphManager == NULL)
        _sceneGraphManager = new SceneGraphManager();
}

IWindow::~IWindow()
{
    if (_sceneGraphManager != NULL)
        delete _sceneGraphManager;
    if (_defaultCursor != NULL)
        delete _defaultCursor;
}

void	IWindow::Create()
{
	Create(_title, Vector2ui(_width, _height), _style, _icon, _antialiasingLevel);
}

void    IWindow::InitSceneGraphManager()
{
    _sceneGraphManager->Init((_antialiasingLevel > 0));
}

void    IWindow::Render(GLContext *context)
{
    CALLSTACK_INFO("IWindow::Render");

    // Render the scene graph manager
    _sceneGraphManager->Render(_context);

    // Render the sub windows
    System::Locker l(&_mutexListSubWindow);
    for (ListSubWindow::iterator it = _listSubWindow.begin(); it != _listSubWindow.end(); ++it)
    {
        if ((*it)->Enabled())
            (*it)->Render(context);
    }
}

Graphic::GLContext   *IWindow::CreateSharedContext()
{
    return _context->CreateNewSharedContext();
}

void    IWindow::RemoveSubWindow(SubWindow *w)
{
    System::Locker l(&_mutexListSubWindow);
    _listSubWindow.remove(w);
}

void    IWindow::AddSubWindow(SubWindow *w)
{
    System::Locker l(&_mutexListSubWindow);
    _listSubWindow.push_back(w);
}
