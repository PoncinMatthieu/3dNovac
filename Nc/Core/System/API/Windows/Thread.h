
/*--------------------------------------------------------------------------------

	3dNovac Utils
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

--------------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_SYSTEM_WINDOWS_THREAD_H_
#define NC_CORE_UTILS_SYSTEM_WINDOWS_THREAD_H_

#include <Windows.h>
#include <iostream>
#include <exception>
#include "../IThread.h"
#include "../../Exception.h"

namespace Nc
{
    namespace System
    {
        /// Thread implementation for a unix system
        class LUTILS Thread : public IThread
        {
            public:
                Thread()
                {
                    _handle = NULL;
                }
                virtual ~Thread()
                {
                    if (_handle != NULL)
                        CloseHandle(_handle);
                }

                void Start()
                {
                    _handle = CreateThread( NULL,                               // default security attributes
                                            0,                                  // use default stack size
                                            (LPTHREAD_START_ROUTINE)&Launch,    // thread function name
                                            this,           // argument to thread function
                                            0,              // use default creation flags
                                            &_threadId);    // returns the thread identifier
                    if (_handle == NULL)
                        throw Exception("The creation of the thread as failed");
                }

                void Wait()
                {
                    if (_handle != NULL)
                        WaitForSingleObject(_handle, INFINITE);
                }

            protected:
                inline void Exit()
                {
                    if (_handle != NULL)
                        ExitThread(0);
                }

            private:
                inline static DWORD Launch(void *p)
                {
                    reinterpret_cast<Thread*>(p)->Run();
                    return 0;
                }

                DWORD       _threadId;
                HANDLE      _handle;
        };
    }
}

#endif
