
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
#include "../Object/Object.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to define a camera
        /**
            A camera is typically used to set the Projection and the View Matrix in a SceneGraph.
            And also manage the viewport of the current opengl Context
        */
        class LGRAPHICS Camera : public Object
        {
            public:
                Camera(const char *className, bool is2d)
                    : Object(className), _resized(true), _setAsCurrentCamera(true), _fix(true), _is2d(is2d), _useWindowSizeForViewport(true)  {}
                virtual ~Camera() {}

                /** Clone the camera */
                virtual ISceneNode  *Clone() const = 0;

                /** To reception the mouse motion event */
                virtual void    MouseMotionEvent(const Nc::System::Event &event) = 0;
                /** To reception the mouse button event */
                virtual void    MouseButtonEvent(const Nc::System::Event &event) = 0;
                /** To reception the keybord event */
                virtual void    KeyboardEvent(const Nc::System::Event &event) = 0;

                /** Update the projection of the scene */
                virtual void    UpdateProjection(SceneGraph *scene) = 0;

                virtual void    TransformModelMatrixToRender(SceneGraph *scene)    {scene->ModelMatrix().AddTransformation(Matrix);}

                /** Fix the camera, Should set the ViewMatrix. If the window has been resized, the camera will resize the viewport of OpenGL */
                virtual void    Fix(SceneGraph *scene);

                /** Set to true the resized statement, need to be called to reset the viewport and projection matrix */
                virtual void    Resized(const System::Event &)              {_resized = true;}

                /** Set the state setHasCurrentCamera. if false, the camera will not be set has current camera at the rendering */
                void            SetAsCurrentCamera(bool state)              {_setAsCurrentCamera = state;}
                /** Set the fix state. if false, the camera will not be fixed at the rendering */
                void            SetFixState(bool state)                     {_fix = state;}

                /** \return true if the camera is a Camera2d */
                bool            Is2d()                                      {return _is2d;}
                /** \return true if the camera is a Camera3d */
                bool            Is3d()                                      {return !_is2d;}

                /** Set the viewport */
                void                    SetViewport(unsigned int viewportX, unsigned int viewportY, unsigned int viewportWidth, unsigned int viewportHeight);
                /** \return the lower left x componant of the viewport rectangle */
                inline unsigned int     ViewportX()                         {return _viewportX;}
                /** \return the lower left y componant of the viewport rectangle */
                inline unsigned int     ViewportY()                         {return _viewportY;}
                /** \return the width of the viewport */
                inline unsigned int     ViewportWidth()                     {return _viewportWidth;}
                /** \return the height of the viewport */
                inline unsigned int     ViewportHeight()                    {return _viewportHeight;}
                /** Reset the viewport (use the size of the attached window) */
                inline void             ResetViewport()                     {_useWindowSizeForViewport = true; _resized = true;}

                /** \return true if the given 2d point is in the viewport of the camera */
                bool                    InViewport(const Vector2ui pos);
                /** \return true if the given point is in the frustum of the camera */
                virtual bool            PointInFrustum(const Vector3f &point) = 0;
                /** \return true if the given box is in the frustum of the camera */
                virtual bool            BoxInFrustum(const Vector3f &center, float size) = 0;


            protected:
                /** Fix the camera and and set it as current camera before rendering itselfs and childs */
                virtual void    Render(SceneGraph *scene);

            protected:
                bool                    _resized;                   ///< resize statement
                bool                    _setAsCurrentCamera;        ///< if false, the camera will not be set has current camera at the rendering
                bool                    _fix;                       ///< if false, the camera will not be fixed at the rendering
                bool                    _is2d;                      ///< true if the camera is a Camera2d

                bool                    _useWindowSizeForViewport;  ///< if true, automatically update the viewport to the window size
                unsigned int            _viewportX;                 ///< the lower left x componant of the viewport rectangle
                unsigned int            _viewportY;                 ///< the lower left y componant of the viewport rectangle
                unsigned int            _viewportWidth;             ///< the width of the viewport
                unsigned int            _viewportHeight;            ///< the height of the viewport
        };
    }
}

#endif
