
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

#include "../Core/GL.h"

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

            \todo manage the redudant state glClearColor
        */
        class LGRAPHICS SceneGraphManager
        {
            public:
                SceneGraphManager();
                ~SceneGraphManager();

                /** Init some opengl statements */
                void            Init(bool multisampling);

                /** \return the scenes */
                inline ListPScene           &Scenes()                       {return _listScene;}
                /** \return the scenes */
                inline const ListPScene     &Scenes() const                 {return _listScene;}

                /** Add a scene to the scene manager */
                inline void         AddScene(SceneGraph *scene)             {_listScene.push_back(scene);}

                /** Remove the given scene */
                inline void         RemoveScene(SceneGraph *scene)          {_mutex.Lock(); _listScene.remove(scene); _mutex.Unlock();}

                /** Bring the given scene to front (these scene would be displayed at the end) */
                void                BringToFront(SceneGraph *scene);

                /** Render the scenes by using the given GLContext */
                void                Render(GLContext *context);

                /** Update the scenes */
                void                Update(float elapsedTime);

            private:
                ListPScene                  _listScene;         ///< the list of scene witch will be rendered
                GL::State::BufferBitMask    _clearMask;         ///< the clear mask (defaul: GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
                Color                       _clearColor;        ///< the clear color of the scene
                System::Mutex               _mutex;             ///< a mutex used to protect the scene
        };
    }
}

#endif
