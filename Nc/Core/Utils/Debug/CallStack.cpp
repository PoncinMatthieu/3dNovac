
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

    File Created At:        10/11/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "CallStack.h"

using namespace Nc;
using namespace Nc::Utils;

CallStackInfo::CallStackInfo(const std::string &info)
{
    CallStack::Instance().Push(info);
}

CallStackInfo::~CallStackInfo()
{
    CallStack::Instance().Pop();
}

CallStack::CallStack()
{
    _stack.reserve(255);
}

CallStack::~CallStack()
{
}

void    CallStack::Push(const std::string &info)
{
    _stack.push_back(info);
}

void    CallStack::Pop()
{
    _stack.pop_back();
}

void    CallStack::Print() const
{
    if (!_stack.empty())
    {
        for (int i = _stack.size() - 1; i >= 0; --i)
        {
            LOG << i << ": " << _stack[i] << std::endl;
        }
    }
}

