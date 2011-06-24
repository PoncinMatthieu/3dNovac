
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

    File Created At:        08/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "SceneGraph.h"
#include "../Camera/Camera.h"

using namespace Nc;
using namespace Nc::Graphic;

SceneGraph::SceneGraph(bool for2dRendering)
    : Entity(ClassName()), _currentCamera(NULL)
{
    _stackProjectionMatrix.push(TMatrix::Identity);
    _stackViewMatrix.push(TMatrix::Identity);
    _stackModelMatrix.push(TMatrix::Identity);

    if (for2dRendering)
        _mode.SetDepthTest(false);
}

SceneGraph::SceneGraph(const char *className, bool for2dRendering)
    : Entity(className), _currentCamera(NULL)
{
    _stackProjectionMatrix.push(TMatrix::Identity);
    _stackViewMatrix.push(TMatrix::Identity);
    _stackModelMatrix.push(TMatrix::Identity);

    if (for2dRendering)
        _mode.SetDepthTest(false);
}


SceneGraph::~SceneGraph()
{
}

void     SceneGraph::SetupCamera()
{
    if (_currentCamera != NULL)
        _currentCamera->UpdateProjection(this);
}

void    SceneGraph::Render()
{
    if (!_enabled)
        return;

    if (!ModelMatrix().IsIdentity())
        ModelMatrix().SetIdentity();

    // Rendering
    _mode.Enable();
    RenderChilds(this);
    _mode.Disable();
}
