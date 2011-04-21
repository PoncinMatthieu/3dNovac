
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

    File Created At:        28/02/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_SCENEGRAPHMANAGER_H_
#define NC_CORE_SYSTEM_SCENEGRAPHMANAGER_H_

#include "I2dSceneGraph.h"
#include "I3dSceneGraph.h"

namespace Nc
{
    namespace Graphic
    {
        /// Provide a way to organise the rendering in a scene
        /**
            The SceneGraphManager class, is used to define a scene. Basicly contain and render 2 SceneGraph Object. <br/>
            (one for a 3d rendering, and another for the 2d rendering in front of the screen like GUI) <br/>
            The SceneGraphs contained by the SceneGraphManager are not deleted when the SceneGraphManager is destroyed.
            So it's your role to destroy it if they are dynamically allocated.
        */
        class LGRAPHICS SceneGraphManager
        {
            public:
                SceneGraphManager();
                ~SceneGraphManager();

                /** Init the OpenGL Lib */
                void                InitGL(bool multisampling);

                /** Set the 3dSceneGraph */
                inline void         Set3dSceneGraph(I3dSceneGraph *s)       {_3dSceneGraph = s; if (s != NULL) _3dSceneGraph->SetCurrentScene();}

                /** Set the 2dSceneGraph */
                inline void         Set2dSceneGraph(I2dSceneGraph *s)       {_2dSceneGraph = s; if (s != NULL) _2dSceneGraph->SetCurrentScene();}

                /** return the 3dSceneGraph */
                inline I3dSceneGraph *Get3dSceneGraph()                     {return _3dSceneGraph;}

                /** return the 2dSceneGraph */
                inline I2dSceneGraph *Get2dSceneGraph()                     {return _2dSceneGraph;}

                /** Render the scene by using the given GLContext */
                void            Render(GLContext *context);

            private:
                I3dSceneGraph   *_3dSceneGraph;     ///< the instance of the 3d scene graph
                I2dSceneGraph   *_2dSceneGraph;     ///< the instance of the 2d scene graph
                GLbitfield      _clearMask;         ///< the clear mask (defaul: GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
                Color           _clearColor;        ///< the clear color of the scene
        };
    }
}

#endif
