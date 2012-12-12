
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

#include "VertexArray.h"

using namespace Nc;
using namespace Nc::Graphic::GL;

VertexArray::VertexArray() : _index(0)
{
    CALLSTACK_INFO("VertexArray::Constructor() ")
    NewRef();
    glGenVertexArrays(1, &_index);
    if (_index == 0)
        throw Utils::Exception("VertexArray", "Failed to create the VertexArrayObject.");
    CALLSTACK_APPEND_INFO(Utils::Convert::ToString(_index));
}

VertexArray::~VertexArray()
{
    ReleaseRef();
}

void     VertexArray::Release()
{
    CALLSTACK_INFO("VertexArray::Release() " + Utils::Convert::ToString(_index));
    glDeleteVertexArrays(1, &_index);
}

void     VertexArray::Enable() const
{
    glBindVertexArray(_index);
}

void     VertexArray::Disable() const
{
    glBindVertexArray(0);
}
