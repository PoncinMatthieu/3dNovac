
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

#ifdef SYSTEM_WINDOWS
    #include "../Window/Window/WCursor.h"
#else
    #include "../Window/Window/XCursor.h"
#endif

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

StandardCamera3d::StandardCamera3d(Graphic::Window *win, double ratioAspect, double near, double far, double fielOfView, Pattern p)
    : Camera3d(ClassName(), ratioAspect, near, far, fielOfView),
      _mouveButton(System::Mouse::Right), _pattern(p),
      _inhibitMovement(false), _drawFrustum(false),
      _angles(-90, -15)
{
    // create cursors
    _cursorOpen = win->NewCursor();
    _cursorOpen->LoadFromXpm(XpmHandOpen);
    _cursorClose = win->NewCursor();
    _cursorClose->LoadFromXpm(XpmHandClose);
    _cursorOpen->Enable();

    _moveSpeed = 1;
    _sensibilityRotate = 1;
    _sensibilityTranslate = 0.1f;
    _sensibilityZoom = 1;
    _stateButtonRight = false;
    _stateButtonLeft = false;
    _distance = 7;
    MajEye();
}

StandardCamera3d::StandardCamera3d(Graphic::Window *win, Pattern p)
    : Camera3d(ClassName(), (float)win->Width()/(float)win->Height(), 0.1, 1000, 70),
      _mouveButton(System::Mouse::Right), _pattern(p),
      _inhibitMovement(false), _drawFrustum(false),
      _angles(-90, -15)
{
    // create cursors
    _cursorOpen = win->NewCursor();
    _cursorOpen->LoadFromXpm(XpmHandOpen);
    _cursorClose = win->NewCursor();
    _cursorClose->LoadFromXpm(XpmHandClose);
    _cursorOpen->Enable();

    _moveSpeed = 1;
    _sensibilityRotate = 1;
    _sensibilityTranslate = 0.1f;
    _sensibilityZoom = 1;
    _stateButtonRight = false;
    _stateButtonLeft = false;
    _distance = 7;
    MajEye();
}

StandardCamera3d::StandardCamera3d(const StandardCamera3d &cam)
    : Camera3d(cam),
      _mouveButton(cam._mouveButton), _pattern(cam._pattern),
      _inhibitMovement(false), _angles(cam._angles)
{
    _cursorOpen = new Graphic::Cursor(*(Cursor*)cam._cursorOpen);
    _cursorClose = new Graphic::Cursor(*(Cursor*)cam._cursorClose);
    _moveSpeed = cam._moveSpeed;
    _sensibilityRotate = cam._sensibilityRotate;
    _sensibilityTranslate = cam._sensibilityTranslate;
    _sensibilityZoom = cam._sensibilityZoom;
    _stateButtonRight = cam._stateButtonRight;
    _stateButtonLeft = cam._stateButtonLeft;
    _distance = cam._distance;
    MajEye();
}

StandardCamera3d &StandardCamera3d::operator = (const StandardCamera3d &cam)
{
    if (_cursorOpen != NULL)
        delete _cursorOpen;
    if (_cursorClose != NULL)
        delete _cursorClose;

    _cursorOpen = new Graphic::Cursor(*(Cursor*)cam._cursorOpen);
    _cursorClose = new Graphic::Cursor(*(Cursor*)cam._cursorClose);
    _inhibitMovement = cam._inhibitMovement;
    _mouveButton = cam._mouveButton;
    _pattern = cam._pattern;
    _angles = cam._angles;
    _moveSpeed = cam._moveSpeed;
    _sensibilityRotate = cam._sensibilityRotate;
    _sensibilityTranslate = cam._sensibilityTranslate;
    _sensibilityZoom = cam._sensibilityZoom;
    _stateButtonRight = cam._stateButtonRight;
    _stateButtonLeft = cam._stateButtonLeft;
    _distance = cam._distance;
    MajEye();
    return *this;
}


StandardCamera3d::~StandardCamera3d()
{
    delete _cursorOpen;
    delete _cursorClose;
}

void StandardCamera3d::MouseMotionEvent(const System::Event &event)
{
    if (_stateButtonRight && !_inhibitMovement)
    {
        if (_pattern == Turntable)
        {
            _angles.Data[0] += (_lastPosMouse.Data[0] - event.MouseMove.X) * _sensibilityRotate;
            _angles.Data[1] -= (_lastPosMouse.Data[1] - event.MouseMove.Y) * _sensibilityRotate;
        }
        else
        {
            MajTrackballPoint(event.MouseMove.X, event.MouseMove.Y);
        }

        MajEye();

        if (_pattern == Trackball)
            _lastSpherePoint = _currentSpherePoint;
        else
        {
            _lastPosMouse.Data[0] = event.MouseMove.X;
            _lastPosMouse.Data[1] = event.MouseMove.Y;
        }
    }
}

void StandardCamera3d::MouseButtonEvent(const System::Event &event)
{
    if (!_inhibitMovement)
    {
    // bouton gauche
        if (event.MouseButton.Button == _mouveButton)
        {
            if (!_stateButtonRight && event.Type == System::Event::MouseButtonPressed)
            {
                _cursorClose->Enable();
                _stateButtonRight = true;
            }
            else if (_stateButtonRight && event.Type == System::Event::MouseButtonReleased)
            {
                _cursorOpen->Enable();
                _stateButtonRight = false;
            }

            if (_pattern == Trackball)
            {
                MajTrackballPoint(WindowInput::MousePosition().Data[0], WindowInput::MousePosition().Data[1]);
                _lastSpherePoint = _currentSpherePoint;
            }
            else
                _lastPosMouse = WindowInput::MousePosition();
        }
    // molette
        if (event.Type == System::Event::MouseWheelMoved)
        {
            _distance -= event.MouseWheel.Delta * _sensibilityZoom;
            if (_distance < 1)
                _distance = 1;
            else if (_distance > 100)
                _distance = 100;
            MajEye();
        }
    }
}

void StandardCamera3d::Update(float runningTime)
{
// ici la vitesse de translation sera aussi fonction de la distance entre l'oeil et le centre
// cela permet d'avoir un deplacement proportionnel a la distance,
// et donc parcourir une plus grande distance si on s'eloigne du centre
    if (!_inhibitMovement)
    {
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
            if (_pattern == Trackball)
            {
            }
            else
                _angles.Data[0] +=  _sensibilityRotate * 100 * runningTime;
            MajEye();
        }
        if (WindowInput::KeyState(System::Key::D))
        {
            if (_pattern == Trackball)
            {
            }
            else
                _angles.Data[0] -=  _sensibilityRotate * 100 * runningTime;
            MajEye();
        }
    }
}

void StandardCamera3d::MajEye()
{
// mise a jour de l'oeil
    if (_pattern == Turntable)
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

        _eye.Init(); // init l'oeil
        TMatrix matriceEye;
        matriceEye.TranslationX(_distance);
        matriceEye.AddRotation(Vector3f(0, 1, 0), _angles.Data[1], 0);
        matriceEye.AddRotation(Vector3f(0, 0, 1), _angles.Data[0], 0);
        matriceEye.AddTranslation(_center);
        matriceEye.Transform(_eye);
        _up.Data[2] = (_angles.Data[1] > -90 && _angles.Data[1] < 90) ? 1 : -1;  // inverse le vecteur up, dans le cas ou l'on retourne la camera
    }
    else if (_pattern == Trackball)
    {
        // get the reel sphere and last sphere point
        Vector3f currentSpherePoint(_currentSpherePoint), lastSpherePoint(_lastSpherePoint);
        _matrixRotationEye.Transform(currentSpherePoint);
        _matrixRotationEye.Transform(lastSpherePoint);

        // compute the axis and theta rotation
        Vector3f axis;
        lastSpherePoint.Cross(currentSpherePoint, axis);
        axis.Normalize();
        float theta = lastSpherePoint.Angle(currentSpherePoint);

        _eye.Init(_distance, 0, 0);
        _up.Init(0,0,1);

        if (axis.Data[0] != 0 || axis.Data[1] != 0 || axis.Data[2] != 0)
            _matrixRotationEye.AddRotation(axis, -theta);
        _matrixRotationEye.Transform(_eye);
        _matrixRotationEye.Transform(_up);

        TMatrix matrixTranslation;
        matrixTranslation.Translation(_center);
        matrixTranslation.Transform(_eye);
    }
    else
    {
        LOG << "The Freefly pattern is not yet implemented." << std::endl;
    }
    UpdateViewFrustum();
    Matrix.Translation(_center); // positionne la matrice de la camera sur le centre
}

void StandardCamera3d::MajTrackballPoint(int x, int y)
{
    //set xy to -1/-1, 1/1 coord
    _currentSpherePoint.Data[1] = ((float)x / (float)(Window::Width()/2.f)) - 1;
    _currentSpherePoint.Data[2] = 1 - ((float)y / (float)(Window::Height()/2.f));
    _currentSpherePoint.Data[0] = 1 - (_currentSpherePoint.Data[1] * _currentSpherePoint.Data[1]) - (_currentSpherePoint.Data[2] * _currentSpherePoint.Data[2]);
    _currentSpherePoint.Data[0] = (_currentSpherePoint.Data[0] > 0) ? sqrt(_currentSpherePoint.Data[0]) : 0;
}

void StandardCamera3d::DrawFrustum(bool state)
{
    _drawFrustum = state;
    if (_drawFrustum == true)
    {
        if (_drawables.size() == 0)
        {
            GL::GeometryBuffer<DefaultVertexType::Colored, false> *geometry = new GL::GeometryBuffer<DefaultVertexType::Colored, false>(GL::Enum::Lines);
            geometry->VBO().Init(8, GL::Enum::StreamDraw);
            _drawables.push_back(new Drawable(geometry));
            GL::GeometryBuffer<DefaultVertexType::Colored, false> *geometryPlan = new GL::GeometryBuffer<DefaultVertexType::Colored, false>(GL::Enum::Triangles);
            geometryPlan->VBO().Init(12, GL::Enum::StreamDraw);
            Drawable *drawablePlan = new Drawable(geometryPlan);
            drawablePlan->Config->SetBlend(GL::Blend::Multiply);
            _drawables.push_back(drawablePlan);
            ChooseDefaultMaterial();
        }
    }
}

void StandardCamera3d::UpdateViewFrustum()
{
    Camera3d::UpdateViewFrustum();

    if (_drawFrustum && _drawables.size() > 0)
    {
        Color                                   c(1,1,1);
        Array<DefaultVertexType::Colored, 8>   vertices;
        vertices[0].Fill(_eye[0], _eye[1], _eye[2], c);
        vertices[1].Fill(_frustumFTL[0], _frustumFTL[1], _frustumFTL[2], c);
        vertices[2].Fill(_eye[0], _eye[1], _eye[2], c);
        vertices[3].Fill(_frustumFTR[0], _frustumFTR[1], _frustumFTR[2], c);
        vertices[4].Fill(_eye[0], _eye[1], _eye[2], c);
        vertices[5].Fill(_frustumFBL[0], _frustumFBL[1], _frustumFBL[2], c);
        vertices[6].Fill(_eye[0], _eye[1], _eye[2], c);
        vertices[7].Fill(_frustumFBR[0], _frustumFBR[1], _frustumFBR[2], c);
        static_cast<GL::GeometryBuffer<DefaultVertexType::Colored, false>*>(_drawables[0]->Geometry)->VBO().UpdateData(vertices.Data);

        Color                                   cPlan(0.3,0.3,0.3);
        Array<DefaultVertexType::Colored, 12>   verticesPlan;
        verticesPlan[0].Fill(_frustumNTL[0], _frustumNTL[1], _frustumNTL[2], cPlan);
        verticesPlan[1].Fill(_frustumNTR[0], _frustumNTR[1], _frustumNTR[2], cPlan);
        verticesPlan[2].Fill(_frustumNBR[0], _frustumNBR[1], _frustumNBR[2], cPlan);

        verticesPlan[3].Fill(_frustumNTL[0], _frustumNTL[1], _frustumNTL[2], cPlan);
        verticesPlan[4].Fill(_frustumNBR[0], _frustumNBR[1], _frustumNBR[2], cPlan);
        verticesPlan[5].Fill(_frustumNBL[0], _frustumNBL[1], _frustumNBL[2], cPlan);

        verticesPlan[6].Fill(_frustumFTL[0], _frustumFTL[1], _frustumFTL[2], cPlan);
        verticesPlan[7].Fill(_frustumFTR[0], _frustumFTR[1], _frustumFTR[2], cPlan);
        verticesPlan[8].Fill(_frustumFBR[0], _frustumFBR[1], _frustumFBR[2], cPlan);

        verticesPlan[9].Fill(_frustumFTL[0], _frustumFTL[1], _frustumFTL[2], cPlan);
        verticesPlan[10].Fill(_frustumFBR[0], _frustumFBR[1], _frustumFBR[2], cPlan);
        verticesPlan[11].Fill(_frustumFBL[0], _frustumFBL[1], _frustumFBL[2], cPlan);
        static_cast<GL::GeometryBuffer<DefaultVertexType::Colored, false>*>(_drawables[1]->Geometry)->VBO().UpdateData(verticesPlan.Data);
    }
}

