
/*-----------------------------------------------------------------------------

	3dNovac Contrib
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/


#include "BasicMeshCreator.h"

using namespace Nc;
using namespace Nc::Graphic;

Object      *BasicMeshCreator::Plan(const Vector2f &size, const GL::Texture &texture)
{
    Array<Core::DefaultVertexType::Textured, 4>     vertices;
    Color color(1, 1, 1);

// creation des vertex en fontion de la taille de la map
    vertices[0].Fill(0, 0, 0, 0, 0, 0, 0, 1, color);
    vertices[1].Fill(size[0], 0, 0, size[0], 0, 0, 0, 1, color);
    vertices[2].Fill(0, size[1], 0, 0, size[1], 0, 0, 1, color);
    vertices[3].Fill(size[0], size[1], 0, size[0], size[1], 0, 0, 1, color);

// creation de la box
    Box3f box(Vector3f(0.f, 0.f, -0.1f), Vector3f(size[0], size[1], 0.1f));

// creation du mesh
    Object *mesh = new Object(box);
    mesh->Drawables().resize(1);
    mesh->Drawables()[0] = new Core::Drawable(vertices, GL::Enum::DataBuffer::StreamDraw, GL::Enum::TriangleStrip);
    mesh->Drawables()[0]->Config->Textures.InitSize(1);
    mesh->Drawables()[0]->Config->Textures[0] = texture;
    mesh->ChooseDefaultMaterial();
    return mesh;
}

Object      *BasicMeshCreator::Grid(const Vector3f &size, const Color &color)
{
// creation des vertex en fontion de la taille
    unsigned int    x = size.Data[0], y = size.Data[1];
    unsigned int    nbVertices = (x * 2) + (y * 2) + 4;
    unsigned int    k = 0;

    Array<Core::DefaultVertexType::Colored>     vertices(nbVertices);
    for (unsigned int i = 0; i < x + 1; i++)
    {
        vertices[k++].Fill(i, 0, size.Data[2], color);
        vertices[k++].Fill(i, size.Data[1], size.Data[2], color);
    }
    for (unsigned int i = 0; i < y + 1; i++)
    {
        vertices[k++].Fill(0, i, size.Data[2], color);
        vertices[k++].Fill(size.Data[0], i, size.Data[2], color);
    }

// creation de la box
    Vector3f bmin(0.f, 0.f, size.Data[2] - 0.1f);
    Vector3f bmax(size.Data[0], size.Data[1], size.Data[2] + 0.1f);

// creation du mesh
    Object *mesh = new Object(Box3f(bmin, bmax));
    mesh->Drawables().resize(1);
    mesh->Drawables()[0] = new Core::Drawable(vertices, GL::Enum::DataBuffer::StreamDraw, GL::Enum::Lines);
    mesh->ChooseDefaultMaterial();
    mesh->UseSceneMaterial(false);
    return mesh;
}

