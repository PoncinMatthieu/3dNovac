
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

#ifndef IGUISCENEGRAPH_H_INCLUDED
#define IGUISCENEGRAPH_H_INCLUDED

#include "ISceneGraph.h"
#include "../Camera/Camera2d.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to render a scene in 2d, Like GUI
        /**
            Typically disable the depth test before the rendering
            The camera associated to the scene graph is not deleted by the scene graph
        */
        class LGRAPHICS I2dSceneGraph : public ISceneGraph
        {
            public:
                I2dSceneGraph(Camera2d *camera) : _camera(camera) {}
                virtual ~I2dSceneGraph()    {}

                /** Render the scene, call the method RenderIn2D */
                virtual void    Render();

                /** Render the scene in 2d on top of the screen */
                virtual void    RenderIn2D() = 0;

                /** To update the projection and view matrix */
                virtual void    SetCurrentScene();

                /** Set the Camera2d associated to the scene, the camera is used to setup the projection and view matrix */
                inline void                 SetCamera(Camera2d* aCamera)      {_camera = aCamera;}

                /** Return the camera */
                inline Camera2d             *GetCamera()                    {return _camera;}
                inline const Camera2d       *GetCamera() const              {return _camera;}

            protected:
                Camera2d        *_camera;           ///< the 2d Camera associated to the scene, it used to update the projection and view matrix
        };
    }
}

#endif // IGUISCENEGRAPH_H_INCLUDED
