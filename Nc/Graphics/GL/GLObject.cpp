
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

#include "GLObject.h"

using namespace Nc;
using namespace Nc::Graphic::GL;

Object::Object()
{
    _nbRef = NULL;
}

Object::Object(const Object &sp)
{
    _nbRef = sp._nbRef;
    if (_nbRef != NULL)
        ++(*_nbRef);
    //std::cout << "construction\tshared : " << _nbRef << "\tNB Ref = " << *_nbRef << std::endl;
}

Object::~Object()
{
}

Object &Object::operator = (const Object &sp)
{
    AddRef(sp._nbRef);
    return *this;
}

void Object::AddRef(unsigned int *nbRef)
{
    if ((nbRef != NULL) && (_nbRef != nbRef))
    {
        ReleaseRef();
        _nbRef = nbRef;
        ++(*_nbRef);
        //std::cout << "construction\tshared : " << _nbRef << "\tNB Ref = " << *_nbRef << std::endl;
    }
}

void Object::NewRef()
{
    ReleaseRef();
    _nbRef = new unsigned int(1);
}

void Object::ReleaseRef()
{
    if (_nbRef != NULL)
    {
        --(*_nbRef);
        //std::cout << "destruction\tshared : n = "<< _n << "\t" << _nbRef << "\tNB Ref = " << *_nbRef << std::endl;
        if (*_nbRef == 0)
        {
            if (IsValid())
                Release();
            delete _nbRef;
            _nbRef = NULL;
        }
    }
}


