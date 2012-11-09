
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

    File Created At:        24/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "../API.h"

using namespace Nc;
using namespace Nc::Utils;

void    System::Sleep(float second)
{
    usleep(static_cast<unsigned long>(second * 1000000));
}

double  System::Time()
{
    struct timeval  t = {0, 0};

    if (gettimeofday(&t, NULL) == -1)
        throw Exception("System::Time", strerror(errno));
	return t.tv_sec + (((double)t.tv_usec)*1.e-6);
}

char* System::Strdup(const char *str)
{
    return strdup(str);
}

unsigned int System::ThreadId()
{
	return pthread_self();
}
