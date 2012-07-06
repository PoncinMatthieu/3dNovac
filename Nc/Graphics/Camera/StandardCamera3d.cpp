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
using namespace Nc::Graphic;

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

StandardCamera3d::StandardCamera3d(Window *attachedWindow, float ratioAspect, float nearD, float farD, float fielOfView, Pattern p)
    : Camera3d(attachedWindow, ratioAspect, nearD, farD, fielOfView),
      _mouveButton(System::Mouse::Right), _pattern(p),
      _inhibitMovement(false), _drawFrustum(false),
      _angles(-90, -15)
{
    // create cursors
	if (p != StandardCamera3d::Freefly)
	{
		_cursorOpen = _window->NewCursor();
		_cursorOpen->LoadFromXpm(XpmHandOpen);
		_cursorClose = _window->NewCursor();
		_cursorClose->LoadFromXpm(XpmHandClose);
		_cursorOpen->Enable();
	}
	else
	{
		_cursorOpen = NULL;
		_cursorClose = NULL;
	}

    _moveSpeed = 1;
    _sensibilityRotate = 1;
    _sensibilityZoom = 1;
    _stateButtonRight = false;
    _stateButtonLeft = false;
    _mouseMotionAlwaysActif = false;
    _distance = 7;
    MajEye();
}

StandardCamera3d::StandardCamera3d(Window *attachedWindow, Pattern p)
    : Camera3d(attachedWindow, (float)attachedWindow->Width()/(float)attachedWindow->Height(), 0.1f, 1000.f, 70.f),
      _mouveButton(System::Mouse::Right), _pattern(p),
      _inhibitMovement(false), _drawFrustum(false),
      _angles(-90, -15)
{
	// create cursors
	if (p != StandardCamera3d::Freefly)
	{
		_cursorOpen = _window->NewCursor();
		_cursorOpen->LoadFromXpm(XpmHandOpen);
		_cursorClose = _window->NewCursor();
		_cursorClose->LoadFromXpm(XpmHandClose);
		_cursorOpen->Enable();
	}
	else
	{
		_cursorOpen = NULL;
		_cursorClose = NULL;
	}

    _moveSpeed = 1;
    _sensibilityRotate = 1;
    _sensibilityZoom = 1;
    _stateButtonRight = false;
    _stateButtonLeft = false;
    _mouseMotionAlwaysActif = false;
    _distance = 7;
    MajEye();
}

StandardCamera3d::StandardCamera3d(const StandardCamera3d &cam)
    : Camera3d(cam),
      _mouveButton(cam._mouveButton), _pattern(cam._pattern),
      _inhibitMovement(false), _drawFrustum(false),
      _angles(cam._angles)
{
	if (cam._pattern != StandardCamera3d::Freefly)
	{
		_cursorOpen = cam._cursorOpen->Clone();
		_cursorClose = cam._cursorClose->Clone();
	}
	else
	{
		_cursorOpen = NULL;
		_cursorClose = NULL;
	}

    _moveSpeed = cam._moveSpeed;
    _sensibilityRotate = cam._sensibilityRotate;
    _sensibilityZoom = cam._sensibilityZoom;
    _stateButtonRight = cam._stateButtonRight;
    _stateButtonLeft = cam._stateButtonLeft;
    _mouseMotionAlwaysActif = cam._mouseMotionAlwaysActif;
    _distance = cam._distance;
    MajEye();
}

StandardCamera3d &StandardCamera3d::operator = (const StandardCamera3d &cam)
{
    Camera3d::operator = (cam);
    if (_cursorOpen != NULL)
        delete _cursorOpen;
    if (_cursorClose != NULL)
        delete _cursorClose;

	if (cam._pattern != StandardCamera3d::Freefly)
	{
		_cursorOpen = new Graphic::Cursor(*(Cursor*)cam._cursorOpen);
		_cursorClose = new Graphic::Cursor(*(Cursor*)cam._cursorClose);
	}
	else
	{
		_cursorOpen = NULL;
		_cursorClose = NULL;
	}

    _inhibitMovement = cam._inhibitMovement;
    _mouveButton = cam._mouveButton;
    _pattern = cam._pattern;
    _angles = cam._angles;
    _moveSpeed = cam._moveSpeed;
    _sensibilityRotate = cam._sensibilityRotate;
    _sensibilityZoom = cam._sensibilityZoom;
    _stateButtonRight = cam._stateButtonRight;
    _stateButtonLeft = cam._stateButtonLeft;
    _mouseMotionAlwaysActif = cam._mouseMotionAlwaysActif;
    _distance = cam._distance;
    _drawFrustum = cam._drawFrustum;
    MajEye();
    return *this;
}

StandardCamera3d::~StandardCamera3d()
{
    if (_cursorOpen != NULL)
        delete _cursorOpen;
    if (_cursorClose != NULL)
        delete _cursorClose;
}

void StandardCamera3d::MouseMotionEvent(const System::Event &event)
{
    if (_mouseMotionAlwaysActif || (_stateButtonRight && !_inhibitMovement))
    {
        if (_pattern == Turntable || _pattern == Freefly)
        {
            _angles.data[0] += (_lastPosMouse.data[0] - event.mouseMove.x) * _sensibilityRotate;
            _angles.data[1] -= (_lastPosMouse.data[1] - event.mouseMove.y) * _sensibilityRotate;
        }
        else
        {
            MajTrackballPoint(event.mouseMove.x, event.mouseMove.y);
        }

        MajEye();

        if (_pattern == Trackball)
            _lastSpherePoint = _currentSpherePoint;
        else
        {
            _lastPosMouse.data[0] = event.mouseMove.x;
            _lastPosMouse.data[1] = event.mouseMove.y;
        }

		if (_pattern == Freefly)
		{

		}
    }
}

void StandardCamera3d::MouseButtonEvent(const System::Event &event)
{
    if (!_inhibitMovement)
    {
    // bouton gauche
        if (event.mouseButton.button == _mouveButton)
        {
            if (!_stateButtonRight && event.type == System::Event::MouseButtonPressed)
            {
                if (_cursorClose != NULL) _cursorClose->Enable();
                _stateButtonRight = true;
            }
            else if (_stateButtonRight && event.type == System::Event::MouseButtonReleased)
            {
                 if (_cursorOpen != NULL) _cursorOpen->Enable();
                _stateButtonRight = false;
            }

            if (_pattern == Trackball)
            {
                MajTrackballPoint(static_cast<WindowInput*>(event.emitter)->MousePosition().data[0],
                                  static_cast<WindowInput*>(event.emitter)->MousePosition().data[1]);
                _lastSpherePoint = _currentSpherePoint;
            }
            else
                _lastPosMouse = static_cast<WindowInput*>(event.emitter)->MousePosition();
        }
    // molette
        if (event.type == System::Event::MouseWheelMoved)
        {
            _distance -= event.mouseWheel.delta * _sensibilityZoom;
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
        bool updateEye = false;
        WindowInput *input = _window->Input();
		if (input->KeyState(System::Key::W) || input->KeyState(System::Key::Z))
        {
			if (_pattern == Freefly)
				_center += ((_center - _eye) * _moveSpeed * runningTime);
			else
				_center += ((_center - _eye) * _moveSpeed * runningTime);
            updateEye = true;
        }
        if (input->KeyState(System::Key::S))
        {
			if (_pattern == Freefly)
				_center -= ((_center - _eye) * _moveSpeed * runningTime);
			else
            _center -= ((_center - _eye) * _moveSpeed * runningTime);
            updateEye = true;
        }
        if (input->KeyState(System::Key::A) || input->KeyState(System::Key::Q))
        {
            if (_pattern == Trackball)
            {
            }
			else if (_pattern == Freefly) // strafe right
			{
				Vector3f right;
				Vector3f(0,0,1).Cross((_center - _eye), right);
				right.Normalize();

				_center += (right * _moveSpeed * runningTime);
			}
            else
                _angles.data[0] +=  _sensibilityRotate * 100 * runningTime;
            updateEye = true;
        }
        if (input->KeyState(System::Key::D))
        {
            if (_pattern == Trackball)
            {
            }
			else if (_pattern == Freefly) // strafe left
			{
				Vector3f left;
				Vector3f(0,0,1).Cross((_center - _eye), left);
				left.Normalize();

				_center -= (left * _moveSpeed * runningTime);
			}
            else
                _angles.data[0] -=  _sensibilityRotate * 100 * runningTime;
            updateEye = true;
        }

        // update l'eye
        if (updateEye)
            MajEye();
    }
}

void StandardCamera3d::MajEye()
{
// mise a jour de l'oeil
    if (_pattern == Turntable)
    {
        // reglage de l'angle
        if (_angles.data[0] > 180)
            _angles.data[0] -= 360;
        if (_angles.data[0] < -180)
            _angles.data[0] += 360;

        if (_angles.data[1] > 180)
            _angles.data[1] -= 360;
        if (_angles.data[1] < -180)
            _angles.data[1] += 360;

        _eye.Init(); // init l'oeil
        TMatrix matriceEye;
        matriceEye.TranslationX(_distance);
        matriceEye.AddRotation(Vector3f(0, 1, 0), _angles.data[1], 0);
        matriceEye.AddRotation(Vector3f(0, 0, 1), _angles.data[0], 0);
        matriceEye.AddTranslation(_center);
        matriceEye.Transform(_eye);
        _up.data[2] = (_angles.data[1] > -90 && _angles.data[1] < 90) ? 1 : -1;  // inverse le vecteur up, dans le cas ou l'on retourne la camera
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

        if (axis.data[0] != 0 || axis.data[1] != 0 || axis.data[2] != 0)
            _matrixRotationEye.AddRotation(axis, -theta);
        _matrixRotationEye.Transform(_eye);
        _matrixRotationEye.Transform(_up);

        TMatrix matrixTranslation;
        matrixTranslation.Translation(_center);
        matrixTranslation.Transform(_eye);
    }
    else
    {
        // reglage de l'angle
        if (_angles.data[0] > 180)
            _angles.data[0] -= 360;
        if (_angles.data[0] < -180)
            _angles.data[0] += 360;

        if (_angles.data[1] > 180)
            _angles.data[1] -= 360;
        if (_angles.data[1] < -180)
            _angles.data[1] += 360;

        _eye.Init(); // init l'oeil

        TMatrix matriceEye;
        matriceEye.TranslationX(_distance);
		matriceEye.AddTranslation(_center);

		float r_temp = cos((_angles.data[1] * M_PI)/180);
		float X = r_temp * cos((_angles.data[0] * M_PI) / 180);
		float Y = r_temp * sin((_angles.data[0] * M_PI) / 180);

		Vector3f forward(X, Y, sin(_angles.data[1]*M_PI/180));

        _eye = (_center + forward);

        _up.data[2] = (_angles.data[1] > -90 && _angles.data[1] < 90) ? 1 : -1;  // inverse le vecteur up, dans le cas ou l'on retourne la camera
    }
    UpdateViewFrustum();
    Matrix.Translation(_center); // positionne la matrice de la camera sur le centre

    // update the view matrix of the camera
    UpdateViewMatrix();
}

void StandardCamera3d::MajTrackballPoint(int x, int y)
{
    //set xy to -1/-1, 1/1 coord
    _currentSpherePoint.data[1] = ((float)x / (float)(_window->Width()/2.f)) - 1;
    _currentSpherePoint.data[2] = 1 - ((float)y / (float)(_window->Height()/2.f));
    _currentSpherePoint.data[0] = 1 - (_currentSpherePoint.data[1] * _currentSpherePoint.data[1]) - (_currentSpherePoint.data[2] * _currentSpherePoint.data[2]);
    _currentSpherePoint.data[0] = (_currentSpherePoint.data[0] > 0) ? sqrt(_currentSpherePoint.data[0]) : 0;
}

void StandardCamera3d::DrawFrustum(bool state)
{
    _drawFrustum = state;
    if (_drawFrustum == true)
    {
        if (_drawables.size() == 0)
        {
            GL::GeometryBuffer<Core::DefaultVertexType::Colored, false> *geometry = new GL::GeometryBuffer<Core::DefaultVertexType::Colored, false>(GL::Enum::Lines);
            geometry->VBO().Init(8, GL::Enum::DataBuffer::StreamDraw);
            _drawables.push_back(new Core::Drawable(geometry));
            GL::GeometryBuffer<Core::DefaultVertexType::Colored, false> *geometryPlan = new GL::GeometryBuffer<Core::DefaultVertexType::Colored, false>(GL::Enum::Triangles);
            geometryPlan->VBO().Init(12, GL::Enum::DataBuffer::StreamDraw);
            Core::Drawable *drawablePlan = new Core::Drawable(geometryPlan);
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
        Array<Core::DefaultVertexType::Colored, 8>   vertices;
        vertices[0].Fill(_eye[0], _eye[1], _eye[2], c);
        vertices[1].Fill(_frustumFTL[0], _frustumFTL[1], _frustumFTL[2], c);
        vertices[2].Fill(_eye[0], _eye[1], _eye[2], c);
        vertices[3].Fill(_frustumFTR[0], _frustumFTR[1], _frustumFTR[2], c);
        vertices[4].Fill(_eye[0], _eye[1], _eye[2], c);
        vertices[5].Fill(_frustumFBL[0], _frustumFBL[1], _frustumFBL[2], c);
        vertices[6].Fill(_eye[0], _eye[1], _eye[2], c);
        vertices[7].Fill(_frustumFBR[0], _frustumFBR[1], _frustumFBR[2], c);
        static_cast<GL::GeometryBuffer<Core::DefaultVertexType::Colored, false>*>(_drawables[0]->Geometry)->VBO().UpdateData(vertices.data);

        Color                                   cPlan(0.3f,0.3f,0.3f);
        Array<Core::DefaultVertexType::Colored, 12>   verticesPlan;
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
        static_cast<GL::GeometryBuffer<Core::DefaultVertexType::Colored, false>*>(_drawables[1]->Geometry)->VBO().UpdateData(verticesPlan.data);
    }
}

void     StandardCamera3d::SetPattern(Pattern p)
{
	_pattern = p;

	if (_pattern == Freefly)
	{
		if (_cursorOpen != NULL)
		{
			_cursorOpen->Disable();
			delete _cursorOpen;
			_cursorOpen = NULL;
		}

		if (_cursorClose != NULL)
		{
			_cursorClose->Disable();
			delete _cursorClose;
			_cursorClose = NULL;
		}
	}
}
