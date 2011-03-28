
/*-----------------------------------------------------------------------------

	3dNovac Core
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

#include <iostream>
#include <string>
#include "Object.h"

using namespace Nc::System;

unsigned int Object::_nbObject = 0;

Object::Object()
{
    _id = _nbObject++;
}

Object::Object(const std::string &name) : _name(name)
{
    _id = _nbObject++;
}

Object::~Object()
{
}

namespace Nc
{
    namespace System
    {
        std::ostream& operator << (std::ostream& Out, const Object& o)
        {
            Out << "id: " << o._id << "  name:" << o._name << "\n";
            return Out;
        }
    }
}
