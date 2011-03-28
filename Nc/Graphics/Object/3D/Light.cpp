
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

#include "Light.h"
#include "Nc/Core/Utils/Debug/OverloadAlloc.h"

using namespace Nc;
using namespace Nc::Graphic;

Light::Light() : Object3d()
{
    _positionLight = Vector3f(0, 0, 0);;
    _colorLight = Color(255, 255, 255, 0);
    _radius = 10;
}

Light::Light(const Vector3f &p, const Color &c, const float radius) : Object3d()
{
    _positionLight = p;
    _colorLight = c;
    _radius = radius;
}
/*
void Light::Draw(ISceneGraph *scene)
{
#ifdef _DEBUG_LIGHT       // deprecated
    glDisable(GL_TEXTURE_2D);
    glPointSize(5);
    _colorLight.Enable();
    glBegin(GL_POINTS);
        glVertex3f(_positionLight.Data[0], _positionLight.Data[1], _positionLight.Data[2]);
    glEnd();
    glEnable(GL_TEXTURE_2D);
#endif
}
*/
