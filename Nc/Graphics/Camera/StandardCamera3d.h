
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
            To manage trackball or turntable camera
            \todo Implemete the freefly system
        */
        class LGRAPHICS StandardCamera3d : public Camera3d
        {
            public:
                /// To determine the patern of the camera
                enum Pattern
                {
                    Trackball,
                    Turntable,
                    Freefly
                };

            // two default X PixMap cursor
                static const char *XpmHandOpen[];
                static const char *XpmHandClose[];

            public:
                StandardCamera3d(Window *win, Pattern p = Turntable);
                virtual ~StandardCamera3d();

                virtual void Resized(const System::Event &event);

                /** \return the pattern used by camera */
                inline Pattern  GetPattern() const                  {return _pattern;}
                /** Set the pattern used by the camera */
                inline void     SetPattern(Pattern p)               {_pattern = p;}

                /** Set the button with active the movement of the camera */
                void            SetMoveButton(System::Mouse::Button button) {_mouveButton = button;}

                // Manage events
                /** Manage the mouse motion event */
                virtual void    MouseMotionEvent(const System::Event &event);
                /** Manage the mouse button event */
                virtual void    MouseButtonEvent(const System::Event &event);
                /** Manage the keyboard event */
                virtual void    KeyboardEvent(const System::Event &) {};

                /** to compute the actual position */
                virtual void    Update(float runningTime);

            private:
                /** Recompute the eye, center and up vector after a modification */
                void MajEye();

                /** Recompute the trackball point with the given mouse position */
                void MajTrackballPoint(int x, int y);

                System::Mouse::Button   _mouveButton;   ///< the button witch active the movement of the camera

                Pattern     _pattern;                   ///< the pattern of the camera
                float       _moveSpeed;                 ///< the speed of the movement of the camera
                float       _sensibilityRotate;         ///< the rotation sensibility
                float       _sensibilityTranslate;      ///< the translation sensibility
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
