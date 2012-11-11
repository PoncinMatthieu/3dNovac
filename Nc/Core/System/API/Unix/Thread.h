
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

#ifndef NC_CORE_UTILS_SYSTEM_UNIX_THREAD_H_
#define NC_CORE_UTILS_SYSTEM_UNIX_THREAD_H_

#include <sys/time.h>
#include <pthread.h>
#include "../IThread.h"
#include "../../../Utils/Exception.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Debug/CrashReporter.h"

namespace Nc
{
    namespace System
    {
        /// Thread implementation for a unix system
        class Thread : public IThread
        {
            public:
                Thread()            {_threadId = 0;}
                virtual ~Thread()   {}

                virtual void Start()
                {
		    getitimer(ITIMER_PROF, &_itimer);
                    if (pthread_create(&_threadId, NULL, Launch, this) != 0)
                        throw Utils::Exception("Thread", "The creation of the thread as failed");
                }

                virtual void Wait()
                {
		    if (_threadId != 0)
                        if (pthread_join(_threadId, NULL) != 0)
		            std::cerr << "Thread: The waiting can't be the current thread" << std::endl;
                }

		virtual unsigned int GetThreadId() const
		{
		  return _id;
		}

            protected:
                inline void Exit()
                {
                    pthread_exit(0);
                }

            private:
                inline static void *Launch(void *t)
                {
		  Utils::CrashReporter::Instance().Init();
		  Thread *th = reinterpret_cast<Thread*>(t);
		  setitimer(ITIMER_PROF, &th->_itimer, NULL);
		  th->_id = ThreadId();
		  th->Run();
		  return 0;
                }

		unsigned int		_id;
                pthread_t   		_threadId;
                struct itimerval 	_itimer;
        };
    }
}

#endif
