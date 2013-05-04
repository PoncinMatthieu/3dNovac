
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

    File Created At:        26/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Locker.h"
#include "IMutex.h"

using namespace Nc;
using namespace Nc::System;

Locker::Locker(IMutex *mutex)
{
    Ctor(mutex);
}

Locker::~Locker()
{
    Dtor();
}

void Locker::Lock(IMutex *mutex)
{
    Dtor();
    Ctor(mutex);
}

void Locker::Unlock()
{
    Dtor();
}

void Locker::Ctor(IMutex *mutex)
{
    _mutex = mutex;
    if (_mutex != NULL)
        _mutex->Lock();
}

void Locker::Dtor()
{
    if (_mutex != NULL)
        _mutex->Unlock();
    _mutex = NULL;
}
