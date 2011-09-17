
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

#include "Define.h"
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

Graphic::Engine::Engine(Nc::Engine::Manager *manager, CreateWindowFunc func, SceneGraphManager *sceneGraphManager)
    : Engine::IEngine(ClassName(), "Graphic Engine", manager, Nc::Engine::HasAContext /*| Nc::Engine::DontWaitOthersContext*/ | Nc::Engine::WaitingLoadContentsOfOthersEngines, 0xff, 0xff, 0xff),
      _sceneGraphManager(sceneGraphManager), _createWinFunction(func), _context(NULL)
{
}

Graphic::Engine::~Engine()
{
// destruction des ressources
//    ActiveContext();  // create an X error (bad value) with Qt. May be we will need to delete contents in a particular slot before the window is destroyed
    FactoryDefaultMaterials::DeleteInstance();

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
    (static_cast<Nc::Engine::MainEngine*>(_manager->GetEngine("Main"))->*_createWinFunction)(_win);
    _context = _win->CreateGLContext();

    // initialize opengl context
    _context->Active();
    _renderState.InitContext(_context);
    _renderState.Enable();
    if (_sceneGraphManager == NULL)
        _sceneGraphManager = new SceneGraphManager();
    _sceneGraphManager->Init((_win->AntialiasingLevel() > 0));
    _renderState.Disable();
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

	WindowInput *input = _win->GetInput();
	input->CheckEvents();

    // display the scene graph
    _renderState.Enable();
    _sceneGraphManager->Render(_context);
    _renderState.Disable();
}
