
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


#include <sstream>
#include <fstream>
#include "Object.h"
#include "../Exception.h"
#include "../Convert.h"

using namespace std;
using namespace Nc::Utils;
using namespace Nc::Utils::Xml;

Object::Object(Object *parent)
    : _type(INLINE), _parent(parent), _cdata(false)
{
}

Object::Object(const std::string &name, TYPE type, Object *parent)
    : _name(name), _type(type), _parent(parent), _cdata(false)
{
}

Object::Object(const std::string &name, TYPE type, const std::map<std::string, std::string> &params, Object *parent)
    : _name(name), _type(type), _params(params), _parent(parent), _cdata(false)
{
}

Object::Object(const Object &o)
{
    _parent = o._parent;
    _name = o._name;
    _type = o._type;
    _params = o._params;
    _cdata = o._cdata;
    _data = o._data;
    for (ListObject::const_iterator it = o._content.begin(); it != o._content.end(); ++it)
    {
        Object *newObj = new Object(**it);
        newObj->_parent = this;
        _content.push_back(newObj);
    }
}

Object &Object::operator = (const Object &o)
{
    _parent = o._parent;
    _name = o._name;
    _type = o._type;
    _params = o._params;
    _cdata = o._cdata;
    _data = o._data;
    for (ListObject::const_iterator it = o._content.begin(); it != o._content.end(); ++it)
    {
        Object *newObj = new Object(**it);
        newObj->_parent = this;
        _content.push_back(newObj);
    }
    return *this;
}

Object::~Object()
{
    for (ListObject::iterator it = _content.begin(); it != _content.end(); ++it)
        delete *it;
}

unsigned int Object::GetNbParent() const
{
    unsigned int n = 0;

    for (Object *p = _parent; p != NULL; p = p->_parent)
        n++;
    return n;
}

bool Object::BlockExist(const std::string &name)
{
    for (ListObject::iterator it = _content.begin(); it != _content.end(); it++)
        if ((*it)->Type() == BLOCK && (*it)->Name() == name)
            return true;
    return false;
}

bool Object::LineExist(const std::string &name)
{
    for (ListObject::iterator it = _content.begin(); it != _content.end(); it++)
        if ((*it)->Type() == INLINE && (*it)->Name() == name)
            return true;
    return false;
}

Object *Object::Block(const std::string &name)
{
    for (ListObject::iterator it = _content.begin(); it != _content.end(); it++)
        if ((*it)->Type() == BLOCK && (*it)->Name() == name)
            return *it;
    throw Exception("Xml::Object", "The block object " + name + " don't exist");
}

Object *Object::Line(const std::string &name)
{
    for (ListObject::iterator it = _content.begin(); it != _content.end(); it++)
        if ((*it)->Type() == INLINE && (*it)->Name() == name)
            return *it;
    throw Exception("Xml::Object", "The inline object " + name + " don't exist");
}

void Object::Write(std::ostream &out, unsigned int i) const
{
    // imprime les infos des objets
    for (int j = i; j > 0; --j)
        out <<"\t";
    out << "<";
    if (_type == HEADER)
        out << "?";
    out << _name;
    if (!_params.empty())
        out << " ";
    for (MapParam::const_iterator it = _params.begin(); it != _params.end(); it++)
        out << it->first << "=\"" << it->second << "\" ";
    if (_type == HEADER)
        out << "?";
    else if (_type == INLINE)
        out << "/";
    out << ">";

    // imprime de facon recursive les childs, si on a un type block
    if (_type == BLOCK)
    {
        if (!_data.empty())
        {
            if (_cdata)
                out << "<![CDATA[";
            out << _data;
            if (_cdata)
                out << "]]>";
        }
        else if (!_content.empty())
            out << "\n";
        for (ListObject::const_iterator it = _content.begin(); it != _content.end(); it++)
            (*it)->Write(out, ((_data.empty() || it != _content.begin()) ? i + 1 : 0));
        if (!_content.empty())
            for (int j = i; j > 0; --j)
                out <<"\t";
        out << "</" << _name << ">\n";
    }
    else
        out << "\n";
}

namespace Nc
{
	namespace Utils
	{
		namespace Xml
		{
			std::ostream	&operator << (std::ostream &out, const Object &o) 
			{
				o.Write(out, 0); 
				return out;
			}
		}
	}
}
