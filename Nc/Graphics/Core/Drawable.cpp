
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

    File Created At:        30/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Drawable.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Graphic::Core;

Drawable::Drawable(GL::IGeometryBuffer *geometry)
{
    Config = new MaterialConfig();
    Geometry = geometry;
}

Drawable::Drawable(GL::IGeometryBuffer *geometry, MaterialConfig *config)
{
    Config = config;
    Geometry = geometry;
}

Drawable::Drawable(const Drawable &d)
{
    Config = (d.Config != NULL) ? d.Config->Clone() : NULL;
    Geometry = (d.Geometry != NULL) ? d.Geometry->Clone() : NULL;
}

Drawable &Drawable::operator = (const Drawable &d)
{
    if (Config != NULL)
        delete Config;
    if (Geometry != NULL)
        delete Geometry;
    Config = (d.Config != NULL) ? d.Config->Clone() : NULL;
    Geometry = (d.Geometry != NULL) ? d.Geometry->Clone() : NULL;
    return *this;
}

Drawable::~Drawable()
{
    if (Config != NULL)
        delete Config;
    if (Geometry != NULL)
        delete Geometry;
}
