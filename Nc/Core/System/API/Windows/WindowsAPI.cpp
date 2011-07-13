
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

#include <Windows.h>

#include "../API.h"

using namespace Nc;

void    System::Sleep(float second)
{ 
    ::Sleep(static_cast<DWORD>(1000 * second));
}

double  System::Time()
{
	static LARGE_INTEGER Frequency;
    static BOOL          UseHighPerformanceTimer = QueryPerformanceFrequency(&Frequency);

    if (UseHighPerformanceTimer)
    {
        // High performance counter available : use it
        LARGE_INTEGER CurrentTime;
        QueryPerformanceCounter(&CurrentTime);
        return static_cast<double>(CurrentTime.QuadPart) / Frequency.QuadPart;
    }
    else // High performance counter not available
        return GetTickCount() * 0.001;        
}

char* System::Strdup(const char *str)
{
	unsigned int len = strlen(str) + 1;
	#ifdef __BORLANDC__
	#define malloc	std::malloc
	#endif
	char *r = (char*)malloc(sizeof(char) * len);
	memcpy(r, str, len);
    return r;
}

unsigned int System::ThreadId()
{
	return GetCurrentThreadId();
}
