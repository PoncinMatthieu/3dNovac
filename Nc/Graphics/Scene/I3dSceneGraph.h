
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

    File Created At:        06/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_SCENE_I3DSCENEGRAPH_H_
#define NC_GRAPHICS_SCENE_I3DSCENEGRAPH_H_

#include "ISceneGraph.h"
#include "../Camera/Camera3d.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS I3dSceneGraph : public ISceneGraph
        {
            public:
                I3dSceneGraph(Camera3d *camera);
                virtual ~I3dSceneGraph();

                virtual void     SetCurrentScene();

                /// Set the Camera3d
                inline void                 SetCamera(Camera3d* aCamera)    {_camera = aCamera;}

                /// Return the camera (the camera is deleted in the Basic3dRenderer in the destructor)
                inline Camera3d             *GetCamera()                    {return _camera;}
                inline const Camera3d       *GetCamera() const              {return _camera;}

            protected:
                Camera3d      *_camera;
        };
    }
}

#endif // NC_GRAPHICS_SCENE_I3DSCENEGRAPH_H_
