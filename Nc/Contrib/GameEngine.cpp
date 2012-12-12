
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

#include <Nc/Graphics/Input/WindowInput.h>
#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Contrib;

GameEngine::GameEngine(IWindow *window, Nc::Engine::Manager *manager,
                        unsigned int pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority)
    : Engine::MainEngine(manager, pattern, deletePriority, loadingContextPriority, loadingPriority), _window(NULL), _context(NULL)
{
    if (window != NULL)
    {
        _window = window;
        AddInput(_window->Input());
    }
}

GameEngine::~GameEngine()
{
}

void    GameEngine::ReleaseContent()
{
    if (_context != NULL)
        delete _context;
}

void    GameEngine::CreateContext()
{
    _context = _window->CreateSharedContext();
}

void    GameEngine::ActiveContext()
{
    if (_context != NULL)
        _context->Active();
}

void    GameEngine::DisableContext()
{
    if (_context != NULL)
        _context->Disable();
}
