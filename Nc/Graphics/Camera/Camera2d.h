
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
#include "../Object/2D/Object2d.h"

namespace Nc
{
    namespace Graphic
    {
        /// Implementation of a 2d Camera to used an orthonormal projection
        class LGRAPHICS Camera2d : public Camera, public Object2d
        {
            public:
                /** Update the projection Matrix to an orthonormal projection */
                inline void     UpdateProjection()          {_projectionMatrix->SetOrtho(0.0, Window::Width(), 0.0, Window::Height(), -1, 1);}

                virtual void    Fix()
                {
                    Camera::Fix();
                    // init the view matrix
                    if (!_viewMatrix->IsIdentity())
                        _viewMatrix->SetIdentity();
                }

                /** Do nothing */
                virtual void    Render(ISceneGraph *)  {}
        };
    }
}

#endif // NC_CAMERA_CAMERA2D_H_
