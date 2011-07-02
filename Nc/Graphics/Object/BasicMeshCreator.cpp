
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

#include "BasicMeshCreator.h"
#include "Nc/Core/Utils/Debug/OverloadAlloc.h"

using namespace Nc;
using namespace Nc::Graphic;

Object *BasicMeshCreator::Repere(float scale, const Vector3f &center)
{
    return Repere(Vector3f(scale, scale, scale), center);
}

Object *BasicMeshCreator::Repere(const Vector3f &scale, const Vector3f &center)
{
    Array<DefaultVertexType::Colored, 4>  vertices;
    Array<unsigned int, 3*2>            indices;

    vertices[0].Fill(center.Data[0], center.Data[1], center.Data[2], Color(1, 1, 1));
    vertices[1].Fill(center.Data[0] + scale.Data[0], center.Data[1], center.Data[2], Color(1, 0, 0));
    vertices[2].Fill(center.Data[0], center.Data[1] + scale.Data[1], center.Data[2], Color(0, 0, 1));
    vertices[3].Fill(center.Data[0], center.Data[1], center.Data[2] + scale.Data[2], Color(0, 1, 0));

    indices[0] = 0; indices[1] = 1;
    indices[2] = 0; indices[3] = 2;
    indices[4] = 0; indices[5] = 3;

    // creation du mesh
    Object *obj = new Object(Box3f(center, center + Vector3f(scale.Data[0], scale.Data[1], scale.Data[2])));
    obj->Drawables().resize(1);
    obj->Drawables()[0] = new Drawable(vertices, GL::Enum::StaticDraw, indices, 2, GL::Enum::Lines);
    obj->ChooseDefaultMaterial();
    return obj;
}
