
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

    File Created At:        13/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPIHCS_GRAPHIC_ENGINE_H_
#define NC_GRAPIHCS_GRAPHIC_ENGINE_H_

#include <map>

#include <Nc/Core/Engine/MainEngine.h>

#include "Define.h"
#include "Context/GLContext.h"
#include "GL/State.h"

namespace Nc
{
    namespace Graphic
    {
        /// The threaded GraphicEngine of 3dNovac
        /**
            Helps you to create and manage a rendering thread.
            The engine has a Window instance and to create it you will need to set the pointer to a member function of the "Main" engine    <br\>
            The rendering is done with the SceneGraphManager.

            \todo Think about removing the static elapsedTime. Statics are evil :D
        */
        class LGRAPHICS   Engine : public Nc::Engine::IEngine
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT(Nc::Engine::IEngine, Nc::Graphic::Engine);

            public:
                Engine(IWindow *attachedWindow, Nc::Engine::Manager *manager);
                virtual ~Engine();

                /** Load default content, like the DefaultMaterials */
                virtual void            LoadContent();
				/** Release the contents like the DefaultMaterials */
                virtual void            ReleaseContent();

                /** Render the scene with the SceneGraphManager, Check the WindowInput Event and update the elapsed time to compute the FPS */
                virtual void            Execute(float runningTime);

            // accessor
                /** \return the window instance used to setup the Window/WindowInput/GLContext */
                inline IWindow           *GetWindow()                        {return _win;}
                /** \return the current number of frame per second (static, so you can call it at anytime) */
                static unsigned int     GetFPS()                            {return (unsigned int)(1./_elapsedTime);}

            protected:
                /** To Create the GLContext, this function is called by the IEngine */
                virtual inline void     CreateContext();
                /** To Active the GLContext, this function is called by the IEngine */
                virtual inline void     ActiveContext()                     {if (_context != NULL) _context->Active();}
                /** To Disable the GLContext, this function is called by the IEngine */
                virtual inline void     DisableContext()                    {if (_context != NULL) _context->Disable();}

                IWindow                  *_win;                  ///< The instance of the window
                GLContext               *_context;              ///< The GL context used to render the scene with the scene graph manager
                GL::State               _renderState;           ///< The render state associated to the rendering context
                static double           _elapsedTime;           ///< Elapsed Time between 2 frame (in second)
        };
    }
}

#endif

