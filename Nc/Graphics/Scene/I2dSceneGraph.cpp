
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

#include "I2dSceneGraph.h"

using namespace Nc::Graphic;

void     I2dSceneGraph::SetCurrentScene()
{
    _camera->SetProjectionMatrix(&ProjectionMatrix());
    _camera->SetViewMatrix(&ViewMatrix());
    _camera->UpdateProjection();
}

void    I2dSceneGraph::Render()
{
    _mode.Enable();

    if (!ModelMatrix().IsIdentity())
        ModelMatrix().SetIdentity();

    _camera->Fix();
    _camera->Render(this);
    glDisable(GL_DEPTH_TEST);
    RenderIn2D();
    glEnable(GL_DEPTH_TEST);
}

