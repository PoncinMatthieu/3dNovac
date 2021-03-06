
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

    File Created At:        29/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "RasterEffect.h"

using namespace Nc;
using namespace Nc::Graphic;

RasterEffect::RasterEffect(Style style)
    : Effect()
{
    SetPattern(style);
}

void RasterEffect::SetPattern(Style style)
{
    System::Locker l(&_mutex);
    if (style == Points)
    {
        _mode.SetPolygonMode(GL::Enum::FrontAndBack, GL::Enum::Point);
        _mode2.InhibitPolygonMode();
        _mode2.InhibitPolygonOffset();
        _mode2.InhibitDepthTest();
    }
    else if (style == Wireframe)
    {
        _mode.SetPolygonMode(GL::Enum::FrontAndBack, GL::Enum::Line);
        _mode2.InhibitPolygonMode();
        _mode2.InhibitPolygonOffset();
        _mode2.InhibitDepthTest();
    }
    else if (style == Fill)
    {
        _mode.SetPolygonMode(GL::Enum::FrontAndBack, GL::Enum::Fill);
        _mode2.InhibitPolygonMode();
        _mode2.InhibitPolygonOffset();
        _mode2.InhibitDepthTest();
    }
    else if (style == FillWireframe)
    {
        _mode.SetPolygonMode(GL::Enum::FrontAndBack, GL::Enum::Fill);
        _mode2.SetPolygonMode(GL::Enum::FrontAndBack, GL::Enum::Line);
        _mode2.SetPolygonOffset(-1.f, -1.f);
        _mode2.InhibitDepthTest();
    }
    else if (style == FillWireframeNoDepth)
    {
        _mode.SetPolygonMode(GL::Enum::FrontAndBack, GL::Enum::Fill);
        _mode2.SetPolygonMode(GL::Enum::FrontAndBack, GL::Enum::Line);
        _mode2.SetPolygonOffset(-1.f, -1.f);
        _mode2.SetDepthTest(false);
    }
}

void RasterEffect::Render(SceneGraph *scene)
{
    bool activated = _activated;

    System::Locker l(&_mutex);
    if (activated)
        _mode.Enable();
    RenderChilds(scene);
    if (activated)
        _mode.Disable();
}

void RasterEffect::Render(Entity *node, SceneGraph *scene)
{
    bool activated = _activated;

    System::Locker l(&_mutex);
    // first pass
    if (activated)
        _mode.Enable();
    node->RenderChilds(scene);
    if (activated)
        _mode.Disable();

    // second pass
    if (_mode2.PolygonModeState() && activated)
    {
        _mode2.Enable();
        node->RenderChilds(scene);
        _mode2.Disable();
    }
}
