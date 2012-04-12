
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

    File Created At:        21/11/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "../Scene/SceneGraph.h"
#include "CameraEffect.h"

using namespace Nc;
using namespace Nc::Graphic;

CameraEffect::CameraEffect()
    :   Effect(),
        _updateViewport(false), _viewportX(0), _viewportY(0), _viewportWidth(0), _viewportHeight(0),
        _updateProjection(false),
        _updateView(false)
{

}

CameraEffect::~CameraEffect()
{

}

void    CameraEffect::Viewport(unsigned int viewportX, unsigned int viewportY, unsigned int viewportWidth, unsigned int viewportHeight)
{
    _viewportX = viewportX;
    _viewportY = viewportY;
    _viewportWidth = viewportWidth;
    _viewportHeight = viewportHeight;
    _updateViewport = true;
}

void    CameraEffect::Projection(float ratioAspect, float nearf, float farf, float fieldOfView)
{
    _projectionMatrix.SetProjection(ratioAspect, nearf, farf, fieldOfView);
    _updateProjection = true;
}

void    CameraEffect::Projection2d(unsigned int viewportX, unsigned int viewportY, unsigned int viewportWidth, unsigned int viewportHeight)
{
    _projectionMatrix.SetOrtho(viewportX, viewportWidth, viewportY, viewportHeight, -1, 1);
    _updateProjection = true;
}

void    CameraEffect::Projection2d()
{
    _projectionMatrix.SetOrtho(_viewportX, _viewportWidth, _viewportY, _viewportHeight, -1, 1);
    _updateProjection = true;
}

void    CameraEffect::View(Vector3f eye, Vector3f center, Vector3f up)
{
    _viewMatrix.SetLookAt(eye, center, up);
    _updateView = true;
}

void    CameraEffect::Render(SceneGraph *scene)
{
    if (_activated)
    {
        unsigned int oldValues[4];
        if (_updateViewport)
        {
            oldValues[0] = scene->GLState()->ViewportX();
            oldValues[1] = scene->GLState()->ViewportY();
            oldValues[2] = scene->GLState()->ViewportWidth();
            oldValues[3] = scene->GLState()->ViewportHeight();
            scene->GLState()->Viewport(_viewportX, _viewportY, _viewportWidth, _viewportHeight);
        }
        if (_updateProjection)
        {
            scene->PushProjectionMatrix();
            scene->ProjectionMatrix() = _projectionMatrix;
        }
        if (_updateView)
        {
            scene->PushViewMatrix();
            scene->ViewMatrix() = _viewMatrix;
        }

        RenderChilds(scene);

        if (_updateViewport)
            scene->GLState()->Viewport(oldValues[0], oldValues[1], oldValues[2], oldValues[3]);
        if (_updateProjection)
            scene->PopProjectionMatrix();
        if (_updateView)
            scene->PopViewMatrix();
    }
    else
        RenderChilds(scene);
}
