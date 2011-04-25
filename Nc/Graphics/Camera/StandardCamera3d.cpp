
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "StandardCamera3d.h"
#include "../Window/Window/ICursor.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace Nc;
using namespace Graphic;

const char *StandardCamera3d::XpmHandOpen[] =
{
    // width height num_colors chars_per_pixel
    " 16 16 3 1 ",
    // colors
    "X c #000000",
    ". c #ffffff",
    "  c None",
    // pixels
    "       XX       ",
    "   XX X..XXX    ",
    "  X..XX..X..X   ",
    "  X..XX..X..X X ",
    "   X..X..X..XX.X",
    "   X..X..X..X..X",
    " XX X.......X..X",
    "X..XX..........X",
    "X...X.........X ",
    " X............X ",
    "  X...........X ",
    "  X..........X  ",
    "   X.........X  ",
    "    X.......X   ",
    "     X......X   ",
    "     X......X   ",
    // pos mouse center
    "0,0"
};

const char *StandardCamera3d::XpmHandClose[] =
{
    // width height num_colors chars_per_pixel
    " 16 16 3 1 ",
    // colors
    "X c #000000",
    ". c #ffffff",
    "  c None",
    // pixels
    "                ",
    "                ",
    "                ",
    "                ",
    "    XX XX XX    ",
    "   X..X..X..XX  ",
    "   X........X.X ",
    "    X.........X ",
    "   XX.........X ",
    "  X...........X ",
    "  X...........X ",
    "  X..........X  ",
    "   X.........X  ",
    "    X.......X   ",
    "     X......X   ",
    "     X......X   ",
    // pos mouse center
    "0,0"
};

StandardCamera3d::StandardCamera3d(Graphic::Window *win, Pattern p)
    : Camera3d((float)win->Width()/(float)win->Height(), 0.1, 1000, 70), _pattern(p), _angles(-90, -15)
{
    // create cursors
    _cursorOpen = win->NewCursor();
    _cursorOpen->LoadFromXpm(XpmHandOpen);
    _cursorClose = win->NewCursor();
    _cursorClose->LoadFromXpm(XpmHandClose);
    _cursorOpen->Enable();

    _moveSpeed = 1;
    _SensibilityRotate = 1;
    _SensibilityTranslate = 0.1f;
    _SensibilityZoom = 1;
    _StateButtonRight = false;
    _StateButtonLeft = false;
    _distance = 7;
    MajEye();
}

StandardCamera3d::~StandardCamera3d()
{
    delete _cursorOpen;
    delete _cursorClose;
}

void StandardCamera3d::Resized(const System::Event &event)
{
    _ratioAspect = (float)event.Size.Width/(float)event.Size.Height;
    Camera3d::Resized(event);
}

void StandardCamera3d::MouseMotionEvent(const System::Event &event)
{
    if (_StateButtonRight)
    {
        _angles.Data[0] += (_lastPosMouse[0] - event.MouseMove.X) * _SensibilityRotate;
        _angles.Data[1] -= (_lastPosMouse[1] - event.MouseMove.Y) * _SensibilityRotate;
        MajEye();
        _lastPosMouse[0] = event.MouseMove.X;
        _lastPosMouse[1] = event.MouseMove.Y;
    }
}

void StandardCamera3d::MouseButtonEvent(const System::Event &event)
{
// bouton gauche
    if (event.MouseButton.Button == System::Mouse::Right)
    {
        if (!_StateButtonRight && event.Type == System::Event::MouseButtonPressed)
        {
            _cursorClose->Enable();
            _StateButtonRight = true;
        }
        else if (_StateButtonRight && event.Type == System::Event::MouseButtonReleased)
        {
            _cursorOpen->Enable();
            _StateButtonRight = false;
        }
        _lastPosMouse = WindowInput::MousePosition();
    }
// molette
    if (event.Type == System::Event::MouseWheelMoved)
    {
        _distance -= event.MouseWheel.Delta * _SensibilityZoom;
        if (_distance < 1)
            _distance = 1;
        else if (_distance > 100)
            _distance = 100;
        MajEye();
    }
}

void StandardCamera3d::Update(float runningTime)
{
// ici la vitesse de translation sera aussi fonction de la distance entre l'oeil et le centre
// cela permet d'avoir un deplacement proportionnel a la distance,
// et donc parcourir une plus grande distance si on s'eloigne du centre
    if (WindowInput::KeyState(System::Key::W))
    {
        _center += ((_center - _eye) * _moveSpeed * runningTime);
        MajEye();
    }
    if (WindowInput::KeyState(System::Key::S))
    {
        _center -= ((_center - _eye) * _moveSpeed * runningTime);
        MajEye();
    }
    if (WindowInput::KeyState(System::Key::A))
    {
        _angles.Data[0] +=  _SensibilityRotate * 100 * runningTime;
        MajEye();
    }
    if (WindowInput::KeyState(System::Key::D))
    {
        _angles.Data[0] -=  _SensibilityRotate * 100 * runningTime;
        MajEye();
    }
}

void StandardCamera3d::MajEye()
{
// reglage de l'angle
    if (_angles.Data[0] > 180)
        _angles.Data[0] -= 360;
    if (_angles.Data[0] < -180)
        _angles.Data[0] += 360;

    if (_angles.Data[1] > 180)
        _angles.Data[1] -= 360;
    if (_angles.Data[1] < -180)
        _angles.Data[1] += 360;

// mise a jour de l'oeil
    if (_pattern == Turntable)
    {
        _eye.Init(); // init l'oeil
        TMatrix matriceEye;
        matriceEye.TranslationX(_distance);
        matriceEye.AddRotation(Vector3f(0, 1, 0), _angles.Data[1], 0);
        matriceEye.AddRotation(Vector3f(0, 0, 1), _angles.Data[0], 0);
        matriceEye.AddTranslation(_center);
        matriceEye.Transform(_eye);
        _up.Data[2] = (_angles.Data[1] > -90 && _angles.Data[1] < 90) ? 1 : -1;  // inverse le vecteur up, dans le cas ou l'on retourne la camera

        Matrix.Translation(_center); // positionne la matrice de la camera sur le centre
    }
    else
    {
        LOG << "Trackball not implemented yet" << std::endl;
/*
    static TMatrix  matriceRotationEye;
    Vector3f lastSpherePoint;
    Vector3f spherePoint;

    //set xy to -1/-1, 1/1 coord
    spherePoint.Data[0] = ((float)WindowInput::MousePosition().Data[0] / (float)(Window::Width()/2.f)) - 1;
    spherePoint.Data[2] = 1 - ((float)WindowInput::MousePosition().Data[1] / (float)(Window::Height()/2.f));
    spherePoint.Data[1] = 1 - (spherePoint.Data[0] * spherePoint.Data[0]) - (spherePoint.Data[2] * spherePoint.Data[2]);
    spherePoint.Data[1] = (spherePoint.Data[1] > 0) ? sqrt(spherePoint.Data[1]) : 0;

    lastSpherePoint.Data[0] = ((float)_lastPosMouse.Data[0] / (float)(Window::Width()/2.f)) - 1;
    lastSpherePoint.Data[2] = 1 - ((float)_lastPosMouse.Data[1] / (float)(Window::Height()/2.f));
    lastSpherePoint.Data[1] = 1 - (lastSpherePoint.Data[0] * lastSpherePoint.Data[0]) - (lastSpherePoint.Data[2] * lastSpherePoint.Data[2]);
    lastSpherePoint.Data[1] = (lastSpherePoint.Data[1] > 0) ? sqrt(lastSpherePoint.Data[1]) : 0;

    matriceRotationEye.Transform(spherePoint);
    matriceRotationEye.Transform(lastSpherePoint);

    // compute the axis and theta rotation
    Vector3f axis;
    lastSpherePoint.Cross(spherePoint, axis);
    axis.Normalize();

    float theta = lastSpherePoint.Angle(spherePoint);

//    LOG << "lastSpherePoint: " << lastSpherePoint << std::endl;
//    LOG << "spherePoint: " << spherePoint << std::endl;
//    LOG << "axis:" << axis << "\ntheta:" << theta << std::endl;
//    LOG << std::endl;

    _eye.Init(_distance, 0, 0);
    _up.Init(0,0,1);

    if (axis.Data[0] != 0 || axis.Data[1] != 0 || axis.Data[2] != 0)
        matriceRotationEye.AddRotation(axis, theta);
    matriceRotationEye.Transform(_eye);
    matriceRotationEye.Transform(_up);

    TMatrix matrixTranslation;
    matrixTranslation.Translation(_center);
    matrixTranslation.Transform(_eye);

    Matrix.Translation(_center); // positionne la matrice de la camera sur le centre pour les childs
*/
    }
}
