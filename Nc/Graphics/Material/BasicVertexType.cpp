
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

#include "BasicVertexType.h"

using namespace Nc::Graphic;
using namespace Nc::Graphic::BasicVertexType;

const GL::VertexDescriptor   Colored::GetDescriptor()
{
    static GL::VertexDescriptor desc(2);
    desc[0] = {3, GL_FLOAT, 0, -1, false};
    desc[1] = {3, GL_FLOAT, 3 * sizeof(float), -1, false};
    return desc;
}

const GL::VertexDescriptor   Colored2d::GetDescriptor()
{
    static GL::VertexDescriptor desc(2);
    desc[0] = {2, GL_FLOAT, 0, -1, false};
    desc[1] = {3, GL_FLOAT, 2 * sizeof(float), -1, false};
    return desc;
}

const GL::VertexDescriptor   &Textured::GetDescriptor()
{
    static GL::VertexDescriptor desc(4);
    desc[0] = {3, GL_FLOAT, 0, -1, false};
    desc[1] = {2, GL_FLOAT, 3 * sizeof(float), -1, false};
    desc[2] = {3, GL_FLOAT, 5 * sizeof(float), -1, false};
    desc[3] = {3, GL_FLOAT, 8 * sizeof(float), -1, false};
    return desc;
}

const GL::VertexDescriptor   &Textured2d::GetDescriptor()
{
    static GL::VertexDescriptor desc(3);
    desc[0] = {2, GL_FLOAT, 0, -1, false};
    desc[1] = {2, GL_FLOAT, 2 * sizeof(float), -1, false};
    desc[2] = {3, GL_FLOAT, 4 * sizeof(float), -1, false};
    return desc;
}

const GL::VertexDescriptor   &Textured3d::GetDescriptor()
{
    static GL::VertexDescriptor desc(3);
    desc[0] = {3, GL_FLOAT, 0, -1, false};
    desc[1] = {3, GL_FLOAT, 3 * sizeof(float), -1, false};
    desc[2] = {3, GL_FLOAT, 6 * sizeof(float), -1, false};
    return desc;
}

void Colored::Fill(float coordX, float coordY, float coordZ, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    coord[2] = coordZ;
    color[0] = c.R;
    color[1] = c.G;
    color[2] = c.B;
}

void Colored2d::Fill(float coordX, float coordY, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    color[0] = c.R;
    color[1] = c.G;
    color[2] = c.B;
}

void Textured::Fill(float coordX, float coordY, float coordZ, float texCoordX, float texCoordY, float normalX, float normalY, float normalZ, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    coord[2] = coordZ;
    texCoord[0] = texCoordX;
    texCoord[1] = texCoordY;
    color[0] = c.R;
    color[1] = c.G;
    color[2] = c.B;
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
    color[0] = c.R;
    color[1] = c.G;
    color[2] = c.B;
}

void Textured3d::Fill(float coordX, float coordY, float coordZ, float texCoordX, float texCoordY, float texCoordZ, const Color &c)
{
    coord[0] = coordX;
    coord[1] = coordY;
    coord[2] = coordZ;
    texCoord[0] = texCoordX;
    texCoord[1] = texCoordY;
    texCoord[2] = texCoordZ;
    color[0] = c.R;
    color[1] = c.G;
    color[2] = c.B;
}
