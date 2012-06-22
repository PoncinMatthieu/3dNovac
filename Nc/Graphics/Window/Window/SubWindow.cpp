
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

    File Created At:        22/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "ICursor.h"
#include "../Input/SubWindowInput.h"
#include "SubWindow.h"

using namespace Nc;
using namespace Nc::Graphic;


SubWindow::SubWindow(Window *parent, SceneGraphManager *sceneGraphManager)
    : Window(sceneGraphManager), _parent(parent)
{
}

SubWindow::~SubWindow()
{
    Close();
}

void    SubWindow::Close()
{
    _parent->_listSubWindow.remove(this);
}

void    SubWindow::Create(const Math::Vector2ui &size)
{
    _width = size.Data[0];
    _height = size.Data[1];
    _context = _parent->GetContext();
    _own = _parent->IsOwn();
    _antialiasingLevel = _parent->AntialiasingLevel();
    _depth = _parent->Depth();
    _stencil = _parent->Stencil();
    _bitsPerPixel = _parent->BitsPerPixel();
    _defaultCursor = _parent->DefaultCursor()->Clone();
    _currentCursor = _defaultCursor;

    _input = new SubWindowInput(this, _parent->GetInput());
    _input->Create();

    _parent->_listSubWindow.push_back(this);

    _isCreate = true;
}

void    SubWindow::Create(const std::string &, const Math::Vector2ui &size, unsigned long, const Utils::FileName &, unsigned int)
{
    Create(size);
}

void    SubWindow::UseExistingWindow(void *, int, const Math::Vector2ui &size, unsigned int)
{
    _width = size.Data[0];
    _height = size.Data[1];
}

void    SubWindow::Resize(unsigned int width, unsigned int height)
{
    _width = width;
    _height = height;
}

GLContext   *SubWindow::CreateGLContext()
{
    _context = _parent->CreateGLContext();
    return _context;
}

ICursor *SubWindow::NewCursor()
{
    return _parent->NewCursor();
}
