
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

    File Created At:        12/07/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/**
	\file WinMain.h
	
	Defines the WinMain entry function, 
	so that developpers can use the standard main function.
	
	This file needs to be include into your main source file and only this one.
	To be able to create a win32 application (without console).
*/

#ifndef NC_CORE_UTILS_SYSTEM_WINDOWS_WINMAIN_H_
#define NC_CORE_UTILS_SYSTEM_WINDOWS_WINMAIN_H_

#ifdef _WIN32
	#include <Windows.h>

	extern int main(int argc, char *argv[]);

	int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
	{
		return main(__argc, __argv);
	}
#endif

#endif
