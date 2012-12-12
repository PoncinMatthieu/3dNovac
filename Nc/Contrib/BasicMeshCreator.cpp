
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
    // creation de la box
    Box3f box(Vector3f(0.f, 0.f, -0.1f), Vector3f(size[0], size[1], 0.1f));

    // creation du mesh
    Object *mesh = new Object(box);
    mesh->Drawables().resize(1);
    mesh->Drawables()[0] = PlanDrawable(size);
    mesh->Drawables()[0]->Config->Textures.InitSize(1);
    mesh->Drawables()[0]->Config->Textures[0] = texture;
    mesh->ChooseDefaultMaterial();
    return mesh;
}

GL::Drawable        *BasicMeshCreator::PlanDrawable(const Vector2f &size)
{
    Array<GL::DefaultVertexType::Textured, 4>     vertices;
    Color color(1, 1, 1);

    // creation des vertex en fontion de la taille de la map
    vertices[0].Fill(0, 0, 0, 0, 0, 0, 0, 1, color);
    vertices[1].Fill(size[0], 0, 0, size[0], 0, 0, 0, 1, color);
    vertices[2].Fill(0, size[1], 0, 0, size[1], 0, 0, 1, color);
    vertices[3].Fill(size[0], size[1], 0, size[0], size[1], 0, 0, 1, color);

    return new GL::Drawable(vertices, GL::Enum::DataBuffer::StreamDraw, GL::Enum::TriangleStrip);
}

Object      *BasicMeshCreator::Grid(const Vector3f &size, const Color &color)
{
    // creation de la box
    Vector3f bmin(0.f, 0.f, size.data[2] - 0.1f);
    Vector3f bmax(size.data[0], size.data[1], size.data[2] + 0.1f);

    // creation du mesh
    Object *mesh = new Object(Box3f(bmin, bmax));
    mesh->Drawables().resize(1);
    mesh->Drawables()[0] = GridDrawable(size, color);
    mesh->ChooseDefaultMaterial();
    mesh->UseSceneMaterial(false);
    return mesh;
}

GL::Drawable        *BasicMeshCreator::GridDrawable(const Vector3f &size, const Color &color)
{
    // creation des vertex en fontion de la taille
    unsigned int    x = size.data[0], y = size.data[1];
    unsigned int    nbVertices = (x * 2) + (y * 2) + 4;
    unsigned int    k = 0;

    Array<GL::DefaultVertexType::Colored>     vertices(nbVertices);
    for (unsigned int i = 0; i < x + 1; ++i)
    {
        vertices[k++].Fill(i, 0, size.data[2], color);
        vertices[k++].Fill(i, size.data[1], size.data[2], color);
    }
    for (unsigned int i = 0; i < y + 1; ++i)
    {
        vertices[k++].Fill(0, i, size.data[2], color);
        vertices[k++].Fill(size.data[0], i, size.data[2], color);
    }

    return new GL::Drawable(vertices, GL::Enum::DataBuffer::StreamDraw, GL::Enum::Lines);
}

Object          *BasicMeshCreator::Circle(float radius, const Color &c, float chordError)
{
    // creation de la box
    Vector3f bmin(-radius, -radius, -0.1f);
    Vector3f bmax(radius, radius, 0.1f);

    // creation du mesh
    Object *mesh = new Object(Box3f(bmin, bmax));
    mesh->Drawables().resize(1);
    mesh->Drawables()[0] = CircleDrawable(radius, c, chordError);
    mesh->ChooseDefaultMaterial();
    mesh->UseSceneMaterial(false);
    return mesh;
}

GL::Drawable    *BasicMeshCreator::CircleDrawable(float radius, const Color &c, float chordError)
{
    unsigned int    nbMeridian = M_PI / acos((radius - chordError) / radius);
    float           alpha = 2.f * M_PI / (float)nbMeridian;

    Array<GL::DefaultVertexType::Colored2d>     vertices(nbMeridian);
    for (unsigned int i = 0; i < nbMeridian; ++i)
        vertices[i].Fill(cos(i * alpha) * radius, sin(i * alpha) * radius, c);

    return new GL::Drawable(vertices, GL::Enum::DataBuffer::StreamDraw, GL::Enum::LineLoop);
}

