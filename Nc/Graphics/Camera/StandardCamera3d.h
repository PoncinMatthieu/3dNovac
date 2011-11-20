
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

    File Created At:        07/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_CAMERATRACKBALL_H_
#define NC_GRAPHIC_CAMERATRACKBALL_H_

#include "../Define.h"
#include "Camera3d.h"

namespace Nc
{
    namespace Graphic
    {
        /// Implementation of a standard Camera3d
        /**
            To manage default freefly, trackball or turntable camera
        */
        class LGRAPHICS StandardCamera3d : public Camera3d
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(Camera3d);
                NC_UTILS_DEFINE_VISITABLE(System::Object);

                /// To determine the patern of the camera
                enum Pattern
                {
                    Trackball,
                    Turntable,
                    Freefly
                };

            // two default X PixMap cursor
                static const char   *XpmHandOpen[];
                static const char   *XpmHandClose[];

            public:
                StandardCamera3d(Window *win, float ratioAspect, float near = 0.1, float far = 1000, float fielOfView = 70, Pattern p = Turntable);
                StandardCamera3d(Window *win, Pattern p = Turntable);
                StandardCamera3d(const StandardCamera3d &cam);
                StandardCamera3d &operator = (const StandardCamera3d &cam);
                virtual ~StandardCamera3d();

                static const char   *ClassName()                            {return "StandardCamera3d";}
                virtual ISceneNode  *Clone() const                          {return new StandardCamera3d(*this);}

                /** \return the pattern used by camera */
                inline Pattern  GetPattern() const                          {return _pattern;}
                /** Set the pattern used by the camera */
                void            SetPattern(Pattern p);

                /** Set the button with active the movement of the camera */
                void            MoveButton(System::Mouse::Button button)    {_mouveButton = button;}
                /** Set the mouse motion status, if true, the movement of the mous will not be stoped by the move buttons */
                inline void     MouseMotionAlwaysActif(bool state)          {_mouseMotionAlwaysActif = state;}
                /** \return the mouse motion status */
                inline bool     MouseMotionAlwaysActif() const              {return _mouseMotionAlwaysActif;}

                /** Set the movement speed */
                inline void     MoveSpeed(float v)                          {_moveSpeed = v;}
                /** \return the movement speed */
                inline float    MoveSpeed() const                           {return _moveSpeed;}
                /** Set the sensibility of the rotation movements */
                inline void     SensibilityRotate(float v)                  {_sensibilityRotate = v;}
                /** \return the sensibility of the translation movements */
                inline float    SensibilityRotate() const                   {return _sensibilityRotate;}
                /** Set the sensibility of the zoom movements */
                inline void     SensibilityZoom(float v)                    {_sensibilityZoom = v;}
                /** \return the sensibility of the translation movements */
                inline float    SensibilityZoom() const                     {return _sensibilityZoom;}

                /** Recompute the eye, center and up vector after a modification */
                virtual void    MajEye();

                // Manage events
                /** Manage the mouse motion event */
                virtual void    MouseMotionEvent(const System::Event &event);
                /** Manage the mouse button event */
                virtual void    MouseButtonEvent(const System::Event &event);
                /** Manage the keyboard event */
                virtual void    KeyboardEvent(const System::Event &) {};

                /** to compute the actual position */
                virtual void    Update(float runningTime);

                /** Update the geometry witch draw the frustum of the camera */
                virtual void    UpdateViewFrustum();
                /** Draw the frustum */
                void            DrawFrustum(bool state);

                /** \return the Inihibit movement statement of the camera */
                inline bool     Inibited()                          {return _inhibitMovement;}
                /** Set the Inihibit movement statement to true */
                inline void     InibitMovement()                    {_inhibitMovement = true;}
                /** Set the Inihibit movement statement to false */
                inline void     UninibitMovement()                  {_inhibitMovement = false;}

            protected:
                /** Recompute the trackball point with the given mouse position */
                void MajTrackballPoint(int x, int y);

                System::Mouse::Button   _mouveButton;   ///< the button witch active the movement of the camera

                Pattern     _pattern;                   ///< the pattern of the camera
                bool        _inhibitMovement;           ///< if true, the camera will no receive the movements events
                bool        _mouseMotionAlwaysActif;    ///< if true, the movement of the mous will not be stoped by the move buttons
                bool        _drawFrustum;               ///< if true, the camera will draw his frustum
                float       _moveSpeed;                 ///< the speed of the movement of the camera
                float       _sensibilityRotate;         ///< the rotation sensibility
                float       _sensibilityZoom;           ///< the zoom sensibility
                bool        _stateButtonRight;          ///< the button right state
                bool        _stateButtonLeft;           ///< the button left state
                float       _distance;                  ///< the distance between the center and the eye
                Vector2f    _angles;                    ///< the angle of rotation
                Vector2i    _lastPosMouse;              ///< the last position of the mouse

                // fo the trackball system
                TMatrix     _matrixRotationEye;         ///< the current rotation matrix of the eye, for trackball system
                Vector3f    _currentSpherePoint;        ///< the current sphere point of the trackball
                Vector3f    _lastSpherePoint;           ///< the last sphere point of the trackball (between a mouse motion event)

                ICursor     *_cursorOpen;               ///< the cursor when the button mouse is up
                ICursor     *_cursorClose;              ///< the cursor when the button mouse is down
        };
    }
}

#endif
