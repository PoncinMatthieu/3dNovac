
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

#include "BoundingBox.h"
#include "../Material/FactoryDefaultMaterials.h"
#include "Nc/Core/Utils/Debug/OverloadAlloc.h"

using namespace Nc;
using namespace Nc::Graphic;

Drawable            *BoundingBox::_drawable = NULL;
IDefaultMaterial    *BoundingBox::_material = NULL;

BoundingBox::BoundingBox()
    : Object(ClassName()), _color(1,1,1)
{
    CreateGeometry();

    // recup le default material
    if (_material == NULL)
    {
        _material = FactoryDefaultMaterials::Instance().GetBestMaterial(_drawable);
        if (_material != NULL)
            _material->Configure(*_drawable);
        else
        {
            delete _drawable;
            _drawable = NULL;
            throw Utils::Exception("BoundingBox", "Can't find a default Material for the bounding box.");
        }
    }
}

BoundingBox::BoundingBox(const Box3f &box)
    : Object(), Box3f(box), _color(1,1,1)
{
    CreateGeometry();
}

BoundingBox::BoundingBox(const Vector3f &min, const Vector3f &max)
    : Object(), Box3f(min, max), _color(1,1,1)
{
    CreateGeometry();
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::CreateGeometry()
{
    if (_drawable == NULL)
    {
        Array<DefaultVertexType::Colored, 8>  vertices;
        Array<unsigned int, 12*2>           indices;

        vertices[0].Fill(0, 0, 0, _color);
        vertices[1].Fill(1, 0, 0, _color);
        vertices[2].Fill(1, 1, 0, _color);
        vertices[3].Fill(0, 1, 0, _color);
        vertices[4].Fill(0, 0, 1, _color);
        vertices[5].Fill(1, 0, 1, _color);
        vertices[6].Fill(1, 1, 1, _color);
        vertices[7].Fill(0, 1, 1, _color);

        indices[0] = 0;     indices[1] = 1;
        indices[2] = 1;     indices[3] = 2;
        indices[4] = 2;     indices[5] = 3;
        indices[6] = 0;     indices[7] = 3;
        indices[8] = 4;     indices[9] = 0;
        indices[10] = 1;    indices[11] = 5;
        indices[12] = 2;    indices[13] = 6;
        indices[14] = 3;    indices[15] = 7;
        indices[16] = 4;    indices[17] = 5;
        indices[18] = 5;    indices[19] = 6;
        indices[20] = 6;    indices[21] = 7;
        indices[22] = 4;    indices[23] = 7;

        _drawable = new Drawable(vertices, GL::Enum::StaticDraw, indices, 2, GL::Enum::Lines);
    }
}

void BoundingBox::Draw(SceneGraph *scene)
{
    Matrix.Scale(_max - _min);
    Matrix.AddTranslation(_min);
    _material->Render(scene, scene->ModelMatrix() * Matrix, *_drawable);
}

