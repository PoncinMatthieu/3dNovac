
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
        /**
            Interface to render in 2d, Like GUI
        */
        class LGRAPHICS I2dSceneGraph : public ISceneGraph
        {
            public:
                I2dSceneGraph(Camera2d *camera) : _camera(camera) {}
                virtual ~I2dSceneGraph()    {}

                virtual void    Render();
                virtual void    RenderIn2D() = 0;

                virtual void    SetCurrentScene();

                /// Set the Camera3d
                inline void                 SetCamera(Camera2d* aCamera)      {_camera = aCamera;}

                /// Return the camera (the camera is deleted in the Basic3dRenderer in the destructor)
                inline Camera2d             *GetCamera()                    {return _camera;}
                inline const Camera2d       *GetCamera() const              {return _camera;}

            protected:
                Camera2d        *_camera;
        };
    }
}

#endif // IGUISCENEGRAPH_H_INCLUDED
