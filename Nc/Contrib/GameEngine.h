
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

    File Created At:        01/08/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CONTRIB_GAMEENGINE_H_
#define NC_CONTRIB_GAMEENGINE_H_

#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include <Nc/Core/Engine/MainEngine.h>
#include <Nc/Graphics/Scene/SceneGraphManager.h>

namespace Nc
{
    namespace Contrib
    {
        /// Helps to create an engine witch use the graphic engine with a shared context
        class GameEngine : public Engine::MainEngine
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT(Nc::Engine::MainEngine, Nc::Contrib::GameEngine);

            public:
                GameEngine(Engine::Manager *manager)
                    : Engine::MainEngine(manager), _context(NULL)                       {}
                virtual ~GameEngine()                                                   {delete _context;}

                /** To setup the window from the sample viewer */
                void            SetWindow(Nc::Graphic::Window *win)                     {_window = win;}

            protected:
                /** Create the shared ogl context */
                virtual void    CreateContext()     {_context = _window->CreateSharedContext();}
                virtual void    ActiveContext()     {if (_context != NULL) _context->Active();}
                virtual void    DisableContext()    {if (_context != NULL) _context->Disable();}

            protected:
                Graphic::Window     *_window;       ///< instance of the window used to render
                Graphic::GLContext  *_context;      ///< instance of the context shared with the context of the graphic engine
        };
    }
}

#endif
