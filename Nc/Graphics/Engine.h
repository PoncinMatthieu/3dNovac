
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
/*-----------------------------------------------------------------------------

                    Implementation de la classe "GraphicEngine"

                  Moteur permettant la gestion du moteur graphic,
                    permet la relation avec le graphe de scene

Herite de Engine

-----------------------------------------------------------------------------*/


#ifndef NC_GRAPIHCS_GRAPHIC_ENGINE_H_
#define NC_GRAPIHCS_GRAPHIC_ENGINE_H_

#include <map>
#include "Define.h"
#include "Scene/SceneGraphManager.h"
#include "Window/Context/GLContext.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS   Engine : public Nc::Engine::IEngine
        {
            public:
                typedef void (Nc::Engine::MainEngine::*CreateWindowFunc)(Window *win);

            public:
                Engine(Nc::Engine::Manager *manager, CreateWindowFunc func);
                virtual ~Engine();

                virtual inline void CreateContext();
                virtual inline void ActiveContext()                         {if (_context != NULL) _context->Active();}
                virtual inline void DisableContext()                        {if (_context != NULL) _context->Disable();}
                inline GLContext    *CreateSharedContext()                  {return _context->CreateNewSharedContext();}

            /// accesseurs
                inline SceneGraphManager        &GetSceneManager()      {return _sceneGraphManager;}
                inline Window                   *GetWindow()            {return _win;}
                static unsigned int             GetFPS()                {return (unsigned int)(1./_elapsedTime);}

                virtual void CheckGLVersion();
                virtual void Execute(float runningTime);

            protected:
                SceneGraphManager       _sceneGraphManager;

                bool                    _windowCreated;         // permet au thread de savoir si le context de la fenetre a bien ete creer
                CreateWindowFunc        _createWinFunction;
                Window                  *_win;
                GLContext               *_context;
                static double           _elapsedTime;           // elapsed Time between 2 frame
        };
    }
}

#endif

