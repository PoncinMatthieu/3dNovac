
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

#ifndef NC_CORE_UTILS_SYSTEM_API_H_
#define NC_CORE_UTILS_SYSTEM_API_H_

    #include "../../Define.h"

	namespace Nc
	{
	    /// This namespace provide some multiarchi function and class
		namespace System
		{
		    /** Sleep in second */
		    LCORE void            Sleep(float second);
		    /** \return time in second */
			LCORE double          Time();
			/** remalloc a null terminated string */
            LCORE char*           Strdup(const char *str);
            /** \return the current thread id */
            LCORE unsigned int    ThreadId();
        }
	}

	#if defined(SYSTEM_WINDOWS)
        #include "Windows/Mutex.h"
        #include "Windows/Thread.h"
    #elif defined(SYSTEM_LINUX)
        #include "Unix/Mutex.h"
        #include "Unix/Thread.h"
    #else
        #error "Thread API not implemented for this system."
    #endif

#endif
