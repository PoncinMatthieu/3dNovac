
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

    File Created At:        25/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CAMERA_CAMERA2D_H_
#define NC_CAMERA_CAMERA2D_H_

#include "Camera.h"

namespace Nc
{
    namespace Graphic
    {
        /// Implementation of a 2d Camera which used an orthonormal projection
        class LGRAPHICS Camera2d : public Camera
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Camera, System::Object, Nc::Graphic::Camera);

            public:
                Camera2d(Window *attachedWindow) : Camera(attachedWindow, true)   {}

                virtual ISceneNode  *Clone() const                              {return new Camera2d(*this);}

                /** To reception the mouse motion event */
                virtual void    MouseMotionEvent(const Nc::System::Event &)     {}
                /** To reception the mouse button event */
                virtual void    MouseButtonEvent(const Nc::System::Event &)     {}
                /** To reception the keybord event */
                virtual void    KeyboardEvent(const Nc::System::Event &)        {}

                /** Update the projection Matrix to an orthonormal projection */
                virtual void    UpdateProjection(SceneGraph *scene);

                /** Fix the camera */
                virtual void    Fix(SceneGraph *scene);

                /** \return true if the given \p point is in the frustum */
                virtual bool    PointInFrustum(const Vector3f &point);
                /** \return true if the given \p box is in the frustum */
                virtual bool    BoxInFrustum(const Vector3f &center, float size);
        };
    }
}

#endif // NC_CAMERA_CAMERA2D_H_
