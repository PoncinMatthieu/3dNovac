
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

    File Created At:        07/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CAMERA_CAMERA_H_
#define NC_GRAPHICS_CAMERA_CAMERA_H_

#include "../Core/GL.h"
#include <Nc/Core/System/Input/Event.h>
#include "../Define.h"
#include "../Window/Window/Window.h"
#include "../Window/Input/WindowInput.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to define a camera
        /**
            A camera typically used to set the Projection and the View Matrix in a ISceneGraph
        */
        class LGRAPHICS Camera
        {
            public:
                Camera() : _resized(false), _projectionMatrix(NULL), _viewMatrix(NULL) {}
                virtual ~Camera() {}

                /** called by the SceneGraph to set instance of the ProjectionMatrix */
                void SetProjectionMatrix(TMatrix *m)        {_projectionMatrix = m;}
                /** called by the SceneGraph to set instance of the ViewMatrix */
                void SetViewMatrix(TMatrix *m)              {_viewMatrix = m;}

                /** Update the projection of the scene */
                virtual void UpdateProjection() = 0;

                /** Fix the camera, Should set the ViewMatrix. If the window has been resized, the camera will resize the viewport of OpenGL */
                virtual void Fix()
                {
                    if (_resized)
                    {
                        glViewport(0, 0, Window::Width(), Window::Height());
                        UpdateProjection();
                        _resized = false;
                    }
                }

                /** Set true the resized statement */
                virtual inline void Resized(const System::Event &)               {_resized = true;}

            protected:
                bool        _resized;               ///< resize statement
                TMatrix     *_projectionMatrix;     ///< instance of the ProjectionMatrix
                TMatrix     *_viewMatrix;           ///< instance of the ViewMatrix
        };
    }
}

#endif
