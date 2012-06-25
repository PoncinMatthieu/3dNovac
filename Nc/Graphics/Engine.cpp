
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

#include "Engine.h"
#include "Material/FactoryDefaultMaterials.h"

#ifdef SYSTEM_WINDOWS
    #include "Window/Window/WWindow.h"
#else
    #include "Window/Window/XWindow.h"
#endif

#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;

double      Graphic::Engine::_elapsedTime = 0;

Graphic::Engine::Engine(const std::string &mainEngineClassName, Nc::Engine::Manager *manager, CreateWindowFunc func)
	: Engine::IEngine(manager, Nc::Engine::WaitingLoadContentsOfOthersEngines | Nc::Engine::Synchronized, 0xff, 0xff, 0xff),
      _createWinFunction(func), _win(NULL), _mainEngineClassName(mainEngineClassName)
{
}

Graphic::Engine::~Engine()
{
    if (_win)
        delete _win;
}

void Graphic::Engine::CreateContext()
{
	// creation de la fenetre et du renderer
#ifdef SYSTEM_WINDOWS
    _win = new WWindow();
#else
    _win = new XWindow();
#endif
    (static_cast<Nc::Engine::MainEngine*>(_manager->GetEngine(_mainEngineClassName))->*_createWinFunction)(_win);
    GLContext *newContext = _win->CreateGLContext();
    _context = newContext;

    // initialize opengl context
    newContext->Active();
    _renderState.InitContext(newContext);
    _renderState.Enable();
    _win->InitSceneGraphManager();
	_renderState.Disable();
    newContext->Disable();
	_win->Enable(true);

}

void Graphic::Engine::ReleaseContent()
{
// destruction des ressources
//    ActiveContext();  // create an X error on linux (bad value) with Qt. May be we will need to delete contents in a particular slot before the window is destroyed
    FactoryDefaultMaterials::DeleteInstance();
}

void Graphic::Engine::LoadContent()
{
    FactoryDefaultMaterials::Instance().AddDefaultMaterial<DefaultMaterial<DefaultVertexType::Colored2d> >();
    FactoryDefaultMaterials::Instance().AddDefaultMaterial<DefaultMaterial<DefaultVertexType::Colored> >();
    FactoryDefaultMaterials::Instance().AddDefaultMaterial<DefaultMaterial<DefaultVertexType::Textured> >();
    FactoryDefaultMaterials::Instance().AddDefaultMaterial<DefaultMaterial<DefaultVertexType::Textured2d> >();
    FactoryDefaultMaterials::Instance().AddDefaultMaterial<DefaultMaterial<DefaultVertexType::Textured3d> >();
}

void Graphic::Engine::Execute(float runningTime)
{
	_elapsedTime = runningTime;

	WindowInput *input = _win->Input();
	input->CheckEvents();

	// display the scene graph
	_renderState.Enable();
	if (_win->Enabled())
		_win->Render(static_cast<GLContext*>(_context));
	_renderState.Disable();
}
