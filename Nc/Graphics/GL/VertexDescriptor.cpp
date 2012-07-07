
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

    File Created At:        20/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "VertexDescriptor.h"

using namespace Nc;
using namespace Nc::Graphic::GL;

DataVertexDescriptor::DataVertexDescriptor()
	: Size(0), Type(0), PointerOffset(0), IndexAttrib(-1), Normalized(false)
{}

DataVertexDescriptor::~DataVertexDescriptor()
{}

VertexDescriptor::VertexDescriptor(const std::string &name, unsigned int Sizeof, unsigned int nbComponent, unsigned int priority)
	: Array<DataVertexDescriptor,0>(nbComponent),
      _name(name), _sizeof(Sizeof), _priority(priority)
{}

VertexDescriptor::~VertexDescriptor()
{}