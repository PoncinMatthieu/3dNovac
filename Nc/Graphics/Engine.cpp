
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

#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Input/WindowInput.h>

#include "Engine.h"
#include "Material/FactoryDefaultMaterials.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;

double      Graphic::Engine::_elapsedTime = 0;

Graphic::Engine::Engine(IWindow *attachedWindow, Nc::Engine::Manager *manager)
	: Engine::IEngine(manager, Nc::Engine::HasAContext | Nc::Engine::WaitingLoadContentsOfOthersEngines | Nc::Engine::Synchronized, 0xff, 0xff, 0xff),
      _win(attachedWindow), _context(NULL)
{
}

Graphic::Engine::~Engine()
{
}

void Graphic::Engine::CreateContext()
{
	_win->Create();
	_context = _win->CreateGLContext();

    // initialize opengl context
    _context->Active();
    _win->InitSceneGraphManager();
    _context->Disable();

	_win->Enable(true);
}

void Graphic::Engine::ReleaseContent()
{
// destruction des resources
//    ActiveContext();  // create an X error on linux (bad value) with Qt. Maybe we will need to delete contents in a particular slot before the window is destroyed
    FactoryDefaultMaterials::DeleteInstance();
}

void Graphic::Engine::LoadContent()
{
    FactoryDefaultMaterials &factory = FactoryDefaultMaterials::Instance();
    factory.AddDefaultMaterial<DefaultMaterial<GL::DefaultVertexType::Colored2d> >();
    factory.AddDefaultMaterial<DefaultMaterial<GL::DefaultVertexType::Colored> >();
    factory.AddDefaultMaterial<DefaultMaterial<GL::DefaultVertexType::Textured> >();
    factory.AddDefaultMaterial<DefaultMaterial<GL::DefaultVertexType::Textured2d> >();
    factory.AddDefaultMaterial<DefaultMaterial<GL::DefaultVertexType::Textured3d> >();
}

void Graphic::Engine::Execute(float runningTime)
{
    CALLSTACK_INFO();

	_elapsedTime = runningTime;

	_win->Input()->CheckEvents();

	// display the scene graph
	if (_win->Enabled())
		_win->Render(_context);
}
