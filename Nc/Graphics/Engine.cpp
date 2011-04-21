
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

Graphic::Engine::Engine(Nc::Engine::Manager *manager, CreateWindowFunc func)
    : Engine::IEngine("Graphic Engine", manager, Nc::Engine::HasAContext | Nc::Engine::DontWaitOthersContext | Nc::Engine::WaitingLoadContentsOfOthersEngines, 0xff, 0xff, 0xff),
      _createWinFunction(func), _context(NULL)
{
}

Graphic::Engine::~Engine()
{
// destruction des ressources
//    ActiveContext();  // create an X error (bad value) with Qt. May be we will need to delete contents in a particular slot before the window is destroyed
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

    // activation du context opengl
    _context->Active();

    // initialise opengl
    CheckGLVersion();
    _sceneGraphManager.InitGL((_win->AntialiasingLevel() > 0));
}

void Graphic::Engine::CheckGLVersion()
{
    float   nbr;

    // on a linux system, we check if the graphic acceleration is support and enable
    #ifdef SYSTEM_LINUX
    LOG << "Direct Rendering : \t\t\t\t\t";
    char *alwaysIndirect = getenv("LIBGL_ALWAYS_INDIRECT");     // if the `LIBGL_ALWAYS_INDIRECT` is set, no graphic acceleration
    if (alwaysIndirect != NULL && string(alwaysIndirect) == "1")
    {
        LOG << "Bad" << std::endl;
        System::Config::Error("GraphicEngine", "Graphic acceleration disabled.\nPlease check that. On compiz, you could try to unset LIBGL_ALWAYS_INDIRECT");
    }
    else
        LOG << "Ok" << std::endl;
    #endif

    // check la version actuel d'opengl
    std::string version((const char*)(EXT.GetInfo(GL_VERSION)));
    LOG << "GL_VERSION = `" << version << "`\t";
    Utils::Convert::StringTo(version, nbr);
    if (nbr < VERSION_MIN_OPENGL)
    {
        LOG << "Bad" << std::endl;
        System::Config::Error("GraphicEngine", "Bad OpenGl version, minimum is '" + Utils::Convert::ToString(VERSION_MIN_OPENGL) + "'\nPlease upgrade your opengl driver");
    }
    else
        LOG << "OK" << std::endl;
    LOG << "GL_VENDOR = `" << EXT.GetInfo(GL_VENDOR) << "`" << std::endl;
    LOG << "GL_RENDERER = `" << EXT.GetInfo(GL_RENDERER) << "`" << std::endl;
    //LOG << "GL_EXTENSIONS = `" << EXT.GetInfo(GL_EXTENSIONS) << "`" << std::endl;
    try
        {LOG << "GL_SHADING_LANGUAGE_VERSION = `" << EXT.GetInfo(GL_SHADING_LANGUAGE_VERSION) << "`" << std::endl;}
    catch (...)
        {System::Config::Error("GraphicEngine", "Failed to fetch GL_SHADING_LANGUAGE_VERSION, Shaders is probably not supported");}
}

void Graphic::Engine::Execute(float runningTime)
{
    _elapsedTime = runningTime;

    // check the window inputs
    _win->GetInput()->CheckEvents();

    // display the scene graph
    _sceneGraphManager.Render(_context);
}
