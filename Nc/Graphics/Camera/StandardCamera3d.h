
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
/*-----------------------------------------------------------------------------

    Implementation de la classe "StandardCamera3d"
    Permet la gestion d'une camera de type trackball ou turntable

Herite de : Camera3d

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_CAMERATRACKBALL_H_
#define NC_GRAPHIC_CAMERATRACKBALL_H_

#include "../Define.h"
#include "Camera3d.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS StandardCamera3d : public Camera3d
        {
            public:
                /// To determine the patern of the camera
                enum Pattern
                {
                    Trackball,
                    Turntable
                };

            // two default X PixMap cursor
                static const char *XpmHandOpen[];
                static const char *XpmHandClose[];

            public:
                StandardCamera3d(Window *win, Pattern p = Turntable);
                virtual ~StandardCamera3d();

                virtual void Resized(const System::Event &event);

                inline Pattern  GetPattern() const      {return _pattern;}

                // Manage events
                virtual void    MouseMotionEvent(const System::Event &event);
                virtual void    MouseButtonEvent(const System::Event &event);
                virtual void    KeyboardEvent(const System::Event &) {};

                // to compute the actual position
                virtual void    Update(float runningTime);

            private:
                void MajEye();

                Pattern     _pattern;
                float       _moveSpeed;
                float       _SensibilityRotate;
                float       _SensibilityTranslate;
                float       _SensibilityZoom;
                bool        _StateButtonRight;
                bool        _StateButtonLeft;
                float       _distance;
                Vector2f    _angles;
                Vector2i    _lastPosMouse;

                ICursor     *_cursorOpen;
                ICursor     *_cursorClose;
        };
    }
}

#endif
