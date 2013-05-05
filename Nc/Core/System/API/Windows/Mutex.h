
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

#ifndef NC_CORE_UTILS_SYSTEM_WINDOWS_MUTEX_H_
#define NC_CORE_UTILS_SYSTEM_WINDOWS_MUTEX_H_

#include <windows.h>
#include "../../../Utils/Exception.h"
#include "../IMutex.h"

#undef CreateWindow

namespace Nc
{
    namespace System
    {
        class ThreadCondition;

        /// Implementation of mutex for unix system (with pthread)
        class LIB_NC_CORE Mutex : public IMutex
        {
            public:
                Mutex()
                {
				  static unsigned int i = 0;
				  _i = i++;
				  if (!InitializeCriticalSectionAndSpinCount(&_mutexId, 0x80000400))
					throw Utils::Exception("Failed to create the mutex.");
                }
                Mutex(const Mutex &)
				{
				  static unsigned int i = 0;
				  _i = i++;
				  if (!InitializeCriticalSectionAndSpinCount(&_mutexId, 0x80000400))
					throw Utils::Exception("Failed to create the mutex.");
				}
                Mutex &operator = (const Mutex &)	{return *this;}
                virtual ~Mutex()			{DeleteCriticalSection(&_mutexId);}

			private:
                inline void Lock()
				{
					#ifdef _DEBUG_MUTEX
					std::cout << "Lock " << _i << " PID = " << GetCurrentThreadId() << std::endl;
					#endif
					EnterCriticalSection(&_mutexId);
					#ifdef _DEBUG_MUTEX
					std::cout << "Lock end " << _i << " PID = " << GetCurrentThreadId() << std::endl;
					#endif
				}
                inline void Unlock()
				{
					#ifdef _DEBUG_MUTEX
					std::cout << "Unlock " << _i << " PID = " << GetCurrentThreadId() << std::endl;
					#endif
					LeaveCriticalSection(&_mutexId);
					#ifdef _DEBUG_MUTEX
					std::cout << "Unlock end " << _i << " PID = " << GetCurrentThreadId() << std::endl;
					#endif
				}

            private:
                CRITICAL_SECTION    _mutexId;
				unsigned int 		_i;

            friend class ThreadCondition;
        };
    }
}

#endif
