
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        04/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Camera.h"

using namespace Nc;
using namespace Nc::Graphic;

Camera::Camera(Window *attachedWindow, bool is2d)
    : Object(), _window(attachedWindow), _resized(true), _setAsCurrentCamera(true), _fix(true), _is2d(is2d), _useWindowSizeForViewport(true)
{
}


void    Camera::SetViewport(unsigned int viewportX, unsigned int viewportY, unsigned int viewportWidth, unsigned int viewportHeight)
{
    _useWindowSizeForViewport = false;
    _viewportX = viewportX;
    _viewportY = viewportY;
    _viewportWidth = viewportWidth;
    _viewportHeight = viewportHeight;
    _resized = true;
}

void    Camera::Fix(SceneGraph *scene)
{
    if (_resized)
    {
        if (_useWindowSizeForViewport)
        {
            _viewportX = 0;
            _viewportY = 0;
            _viewportWidth = _window->Width();
            _viewportHeight = _window->Height();
        }
        _resized = false;
        UpdateProjection(scene);
    }
    scene->GLState()->Viewport(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
}

bool    Camera::InViewport(const Vector2ui pos)
{
    return (pos.data[0] >= _viewportX && pos.data[1] >= _viewportY &&
            pos.data[0] < (_viewportX + _viewportWidth) && pos.data[1] < (_viewportY + _viewportHeight));
}

void    Camera::Render(SceneGraph *scene)
{
    if (_setAsCurrentCamera && _fix)
    {
        if (scene->CurrentCamera() != this)
        {
            scene->CurrentCamera(this);
            UpdateProjection(scene);
        }
        Fix(scene);
    }
    else if (_setAsCurrentCamera && scene->CurrentCamera() != this)
        scene->CurrentCamera(this);
    else if (_fix)
    {
        if (scene->CurrentCamera() != this)
            UpdateProjection(scene);
        Fix(scene);
    }
    Object::Render(scene);
}


bool    Camera::Project(const TMatrix &modelMatrix, const TMatrix &viewMatrix, const TMatrix &projectionMatrix, const Vector3f &objCoords, Vector3f &winCoords) const
{
    Vector<float,4>  in;
    in[0] = objCoords[0];
    in[1] = objCoords[1];
    in[2] = objCoords[2];
    in[3] = 1.0;

    modelMatrix.Transform(in);
    viewMatrix.Transform(in);
    projectionMatrix.Transform(in);


    if (in[3] == 0.0)
        return false;

    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];

    /* Map x, y and z to range 0-1 */
    in[0] = in[0] * 0.5 + 0.5;
    in[1] = in[1] * 0.5 + 0.5;
    in[2] = in[2] * 0.5 + 0.5;

    /* Map x,y to viewport */
    in[0] = in[0] * _viewportWidth + _viewportX;
    in[1] = in[1] * _viewportHeight + _viewportY;

    winCoords[0] = in[0];
    winCoords[1] = in[1];
    winCoords[2] = in[2];
    return true;
}

void    Camera::TransformModelMatrixToRender(SceneGraph *scene)
{
    scene->ModelMatrix().AddTransformation(Matrix);
}

void    Camera::Resized(const Vector2ui &)
{
    _resized = true;
}
