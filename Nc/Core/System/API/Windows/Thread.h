
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

#ifndef NC_CORE_UTILS_SYSTEM_WINDOWS_THREAD_H_
#define NC_CORE_UTILS_SYSTEM_WINDOWS_THREAD_H_

#include <Windows.h>
#include <iostream>
#include <exception>

#undef CreateWindow

#include "../IThread.h"
#include "../../../Utils/Exception.h"

namespace Nc
{
    namespace System
    {
        /// Thread implementation for a unix system
        class LIB_NC_CORE Thread : public IThread
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

                virtual void Start()
                {
                    _handle = CreateThread( NULL,                               // default security attributes
                                            0,                                  // use default stack size
                                            (LPTHREAD_START_ROUTINE)&Launch,    // thread function name
                                            this,           // argument to thread function
                                            0,              // use default creation flags
                                            &_threadId);    // returns the thread identifier
                    if (_handle == NULL)
                        throw Utils::Exception("The creation of the thread as failed");
				}

                virtual void Wait()
                {
                    if (_handle != NULL)
                        WaitForSingleObject(_handle, INFINITE);
                }

		virtual unsigned int GetThreadId() const
		{
		  return _id;
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
		  Thread *th = reinterpret_cast<Thread*>(p);
		  th->_id = ThreadId();
		  th->Run();
		  return 0;
                }

		unsigned int	_id;
                DWORD		_threadId;
                HANDLE		_handle;
        };
    }
}

#endif
