
/*--------------------------------------------------------------------------------

	3dNovac Utils
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

--------------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_SYSTEM_WINDOWS_MUTEX_H_
#define NC_CORE_UTILS_SYSTEM_WINDOWS_MUTEX_H_

#include <windows.h>
#include "../../Exception.h"
#include "../IMutex.h"

namespace Nc
{
    namespace System
    {
        class ThreadCondition;

        /// Implementation of mutex for unix system (with pthread)
        class LUTILS Mutex : public IMutex
        {
            public:
                Mutex()
                {
					static unsigned int i = 0;
					_i = i++;
                    if (!InitializeCriticalSectionAndSpinCount(&_mutexId, 0x80000400))
                        throw Exception("Failed to create the mutex.");
                }

                virtual ~Mutex()            {DeleteCriticalSection(&_mutexId);}

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
