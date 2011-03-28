
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
/*-----------------------------------------------------------------------------

	Description : Interface for camera

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
        class LGRAPHICS Camera
        {
            public:
                Camera() : _resized(false), _projectionMatrix(NULL), _viewMatrix(NULL) {}
                virtual ~Camera() {}

                void SetProjectionMatrix(TMatrix *m)        {_projectionMatrix = m;}
                void SetViewMatrix(TMatrix *m)              {_viewMatrix = m;}

                virtual void UpdateProjection() = 0;
                virtual void Fix()
                {
                    if (_resized)
                    {
                        glViewport(0, 0, Window::Width(), Window::Height());
                        UpdateProjection();
                        _resized = false;
                    }
                }

                virtual void Resized(const System::Event &event)
                {
                    if (event.Type == System::Event::Resized)
                        _resized = true;
                }

            protected:
                bool        _resized;
                TMatrix     *_projectionMatrix;
                TMatrix     *_viewMatrix;
        };
    }
}

#endif
