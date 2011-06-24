
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

    File Created At:        23/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Camera2d.h"

using namespace Nc;
using namespace Nc::Graphic;

void    Camera2d::UpdateProjection(SceneGraph *scene)
{
    scene->ProjectionMatrix().SetOrtho(_viewportX, _viewportWidth, _viewportY, _viewportHeight, -1, 1);
}

void    Camera2d::Fix(SceneGraph *scene)
{
    Camera::Fix(scene);
    // init the view matrix
    if (!scene->ViewMatrix().IsIdentity())
        scene->ViewMatrix().SetIdentity();
}

bool    Camera2d::PointInFrustum(const Vector3f &point)
{
    // in 2 dimensions, we have just to test the collision with the viewport
    return (point.Data[0] >= _viewportX &&
            point.Data[1] >= _viewportY &&
            point.Data[0] < _viewportWidth &&
            point.Data[1] < _viewportHeight);
}

bool    Camera2d::BoxInFrustum(const Vector3f &center, float size)
{
    // in 2 dimensions, we have just to test the collision with the viewport
    return (center.Data[0] - size >= _viewportX &&
            center.Data[1] - size >= _viewportY &&
            center.Data[0] - size < _viewportWidth &&
            center.Data[1] - size < _viewportHeight);
}
