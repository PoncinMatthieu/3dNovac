
/*-----------------------------------------------------------------------------

	3dNovac Core
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "MainEngine.h"
#include "Manager.h"
#include "../Utils/Debug/OverloadAlloc.h"

using namespace std;
using namespace Nc::Engine;

MainEngine::MainEngine(const char *className, Manager *manager, unsigned int pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority)
    : IEngine(className, "Game Engine", manager, pattern, deletePriority, loadingContextPriority, loadingPriority)
{
}

MainEngine::MainEngine(Manager *manager, unsigned int pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority)
    : IEngine(ClassName(), "Game Engine", manager, pattern, deletePriority, loadingContextPriority, loadingPriority)
{
}

MainEngine::~MainEngine()
{
}

void MainEngine::Execute(float runningTime)
{
    ManageWindowEvents();// maj des event window
    Update(runningTime);
}

void MainEngine::ManageWindowEvents()
{
// traitement des evenements de la fenetre
    System::Event event;

    while (PollEvent(event))
        ManageWindowEvent(event);
}

void MainEngine::ManageWindowEvent(Nc::System::Event &event)
{
    if (event.Type == System::Event::Closed)
        Quit();

    if (event.Type == System::Event::KeyPressed)
        KeyboardEvent(event);
    else if (event.Type == System::Event::KeyReleased)
        KeyboardEvent(event);
    else if (event.Type == System::Event::MouseMoved)
        MouseMotionEvent(event);
    else if (event.Type == System::Event::MouseButtonPressed ||
             event.Type == System::Event::MouseButtonReleased ||
             event.Type == System::Event::MouseWheelMoved)
        MouseButtonEvent(event);
}

void MainEngine::Quit()
{
    if (ReleaseContent())
        _manager->Stop();
}
