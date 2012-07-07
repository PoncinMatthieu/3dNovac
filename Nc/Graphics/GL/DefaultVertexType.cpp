
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

#include "DefaultVertexType.h"

using namespace Nc::Graphic;
using namespace Nc::Graphic::GL;
using namespace Nc::Graphic::GL::DefaultVertexType;

const GL::VertexDescriptor   &Colored::GetDescriptor()
{
    static GL::VertexDescriptor desc("Colored", sizeof(Colored), 2, GetDescriptorPriority());
    desc[0].Init<float>(ComponentsName::Coord, 3, Enum::Float, 0);
    desc[1].Init<float>(ComponentsName::Color, 3, Enum::Float, 3 * sizeof(float));
    return desc;
}

const GL::VertexDescriptor   &Colored2d::GetDescriptor()
{
    static GL::VertexDescriptor desc("Colored2d", sizeof(Colored2d), 2, GetDescriptorPriority());
    desc[0].Init<float>(ComponentsName::Coord, 2, Enum::Float, 0);
    desc[1].Init<float>(ComponentsName::Color, 3, Enum::Float, 2 * sizeof(float));
    return desc;
}

const GL::VertexDescriptor   &Textured::GetDescriptor()
{
    static GL::VertexDescriptor desc("Textured", sizeof(Textured), 4, GetDescriptorPriority());
    desc[0].Init<float>(ComponentsName::Coord, 3, Enum::Float, 0);
    desc[1].Init<float>(ComponentsName::TexCoord, 2, Enum::Float, 3 * sizeof(float));
    desc[2].Init<float>(ComponentsName::Color, 3, Enum::Float, 5 * sizeof(float));
    desc[3].Init<float>(ComponentsName::Normal, 3, Enum::Float, 8 * sizeof(float));
    return desc;
}

const GL::VertexDescriptor   &Textured2d::GetDescriptor()
{
    static GL::VertexDescriptor desc("Textured2d", sizeof(Textured2d), 3, GetDescriptorPriority());
    desc[0].Init<float>(ComponentsName::Coord, 2, Enum::Float, 0);
    desc[1].Init<float>(ComponentsName::TexCoord, 2, Enum::Float, 2 * sizeof(float));
    desc[2].Init<float>(ComponentsName::Color, 3, Enum::Float, 4 * sizeof(float));
    return desc;
}

const GL::VertexDescriptor   &Textured3d::GetDescriptor()
{
    static GL::VertexDescriptor desc("Textured3d", sizeof(Textured3d), 3, GetDescriptorPriority());
    desc[0].Init<float>(ComponentsName::Coord, 3, Enum::Float, 0);
    desc[1].Init<float>(ComponentsName::TexCoord, 3, Enum::Float, 3 * sizeof(float));
    desc[2].Init<float>(ComponentsName::Color, 3, Enum::Float, 6 * sizeof(float));
    return desc;
}

void Colored::Fill(float coordX, float coordY, float coordZ, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    coord[2] = coordZ;
    color[0] = c.r;
    color[1] = c.g;
    color[2] = c.b;
}

void Colored2d::Fill(float coordX, float coordY, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    color[0] = c.r;
    color[1] = c.g;
    color[2] = c.b;
}

void Textured::Fill(float coordX, float coordY, float coordZ, float texCoordX, float texCoordY, float normalX, float normalY, float normalZ, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    coord[2] = coordZ;
    texCoord[0] = texCoordX;
    texCoord[1] = texCoordY;
    color[0] = c.r;
    color[1] = c.g;
    color[2] = c.b;
    normal[0] = normalX;
    normal[1] = normalY;
    normal[2] = normalZ;
}

void Textured2d::Fill(float coordX, float coordY, float texCoordX, float texCoordY, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    texCoord[0] = texCoordX;
    texCoord[1] = texCoordY;
    color[0] = c.r;
    color[1] = c.g;
    color[2] = c.b;
}

void Textured3d::Fill(float coordX, float coordY, float coordZ, float texCoordX, float texCoordY, float texCoordZ, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    coord[2] = coordZ;
    texCoord[0] = texCoordX;
    texCoord[1] = texCoordY;
    texCoord[2] = texCoordZ;
    color[0] = c.r;
    color[1] = c.g;
    color[2] = c.b;
}
