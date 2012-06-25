
/*-----------------------------------------------------------------------------

	3dNovac Contrib
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

    File Created At:        23/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/Graphics/Window/Input/WindowInput.h>
#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Contrib;

GameEngine::GameEngine(Engine::Manager *manager)
    : Engine::MainEngine(manager)
{
}

GameEngine::~GameEngine()
{
    delete _context;
}

void    GameEngine::SetWindow(Nc::Graphic::Window *win)
{
    _window = win;
    AddInput(_window->Input());
}

void    GameEngine::CreateContext()
{
    _context = _window->CreateSharedContext();
}
