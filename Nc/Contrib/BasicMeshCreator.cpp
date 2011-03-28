
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
/*
Object3d    *BasicMeshCreator::Pave(const Vector3f& center, const Vector3f &size, const Color& color, const GL::Texture &texture)
{
    Array<BasicVertexType::Textured, 12>    vertices;
    Array<unsigned int, 36>                 indices;

    vertices[0].Fill(center[0], center[1], center[2], 0, 0, color);
    vertices[1].Fill(center[0] + size[0], center[1], center[2], 1, 0, color);
    vertices[2].Fill(center[0] + size[0], center[1] + size[1], center[2], 1, 1, color);
    vertices[3].Fill(center[0], center[1] + size[1], center[2], 0, 1, color);

    vertices[4].Fill(center[0] + size[0], center[1] + size[1], center[2] + size[2], 0, 1, color);
    vertices[5].Fill(center[0] + size[0], center[1], center[2] + size[2], 0, 0, color);
    vertices[6].Fill(center[0], center[1], center[2] + size[2], 1, 0, color);
    vertices[7].Fill(center[0], center[1] + size[1], center[2] + size[2], 1, 1, color);

    vertices[8].Fill(center[0] + size[0], center[1], center[2] + size[2], 1, 1, color);
    vertices[9].Fill(center[0] + size[0], center[1], center[2], 0, 1, color);
    vertices[10].Fill(center[0], center[1] + size[1], center[2] + size[2], 0, 0, color);
    vertices[11].Fill(center[0] + size[0], center[1] + size[1], center[2] + size[2], 1, 0, color);

    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 2; indices[4] = 3; indices[5] = 0;

    indices[6] = 5; indices[7] = 1; indices[8] = 4;
    indices[9] = 2; indices[10] = 4; indices[11] = 1;

    indices[12] = 0; indices[13] = 6; indices[14] = 7;
    indices[15] = 0; indices[16] = 7; indices[17] = 3;

    indices[18] = 5; indices[19] = 6; indices[20] = 7;
    indices[21] = 5; indices[22] = 4; indices[23] = 7;

    indices[24] = 0; indices[25] = 6; indices[26] = 8;
    indices[27] = 0; indices[28] = 8; indices[29] = 9;

    indices[30] = 10; indices[31] = 11; indices[32] = 2;
    indices[33] = 10; indices[34] = 2; indices[35] = 3;

    // creation du mesh
    GL::GeometryBuffer<BasicVertexType::Textured> geometry(GL::VertexBuffer<BasicVertexType::Textured>(vertices, GL_STATIC_DRAW),
                                                          GL::IndexBuffer(indices, 3), GL_TRIANGLES);
    Mesh<BasicVertexType::Textured>     *mesh = new Mesh<BasicVertexType::Textured>(geometry, Box3f(center, center + size));
    mesh->GetMaterialConfig().Texture = texture;
    return mesh;
}
*/

Object3d    *BasicMeshCreator::Grid(const Vector3f &size, const Color &color)
{
// creation des vertex en fontion de la taille
    unsigned int    x = size.Data[0], y = size.Data[1];
    unsigned int    nbVertices = (x * 2) + (y * 2) + 4;
    unsigned int    k = 0;

    Array<BasicVertexType::Colored>     vertices(nbVertices);
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
    GL::GeometryBuffer<BasicVertexType::Colored, false> geometry(GL::VertexBuffer<BasicVertexType::Colored>(vertices, GL_STATIC_DRAW), GL_LINES);
    Mesh<BasicVertexType::Colored, false>  *mesh = new Mesh<BasicVertexType::Colored, false>(geometry, Box3f(bmin, bmax));
    return mesh;
}

