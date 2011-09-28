
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
            _viewportWidth = Window::Width();
            _viewportHeight = Window::Height();
        }
        _resized = false;
        UpdateProjection(scene);
    }
    GL::State::Current().Viewport(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
}

bool    Camera::InViewport(const Vector2ui pos)
{
    return (pos.Data[0] >= _viewportX && pos.Data[1] >= _viewportY &&
            pos.Data[0] < (_viewportX + _viewportWidth) && pos.Data[1] < (_viewportY + _viewportHeight));
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
