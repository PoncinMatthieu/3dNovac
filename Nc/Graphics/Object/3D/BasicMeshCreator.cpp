
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

Object3d *BasicMeshCreator::Repere(float echelle, const Vector3f &center)
{
    return Repere(Vector3f(echelle, echelle, echelle), center);
}

Object3d *BasicMeshCreator::Repere(const Vector3f &echelle, const Vector3f &center)
{
    Array<BasicVertexType::Colored, 4>  vertices;
    Array<unsigned int, 3*2>            indices;

    vertices[0].Fill(center.Data[0], center.Data[1], center.Data[2], Color(1, 1, 1));
    vertices[1].Fill(center.Data[0] + echelle.Data[0], center.Data[1], center.Data[2], Color(1, 0, 0));
    vertices[2].Fill(center.Data[0], center.Data[1] + echelle.Data[1], center.Data[2], Color(0, 0, 1));
    vertices[3].Fill(center.Data[0], center.Data[1], center.Data[2] + echelle.Data[2], Color(0, 1, 0));

    indices[0] = 0; indices[1] = 1;
    indices[2] = 0; indices[3] = 2;
    indices[4] = 0; indices[5] = 3;

    // creation du mesh
    GL::GeometryBuffer<BasicVertexType::Colored> geometry(GL::VertexBuffer<BasicVertexType::Colored>(vertices, GL_STATIC_DRAW),
                                                          GL::IndexBuffer(indices, 2), GL_LINES);
    return new Mesh<BasicVertexType::Colored>(geometry, Box3f(center, center + Vector3f(echelle.Data[0], echelle.Data[1], echelle.Data[2])));
}
