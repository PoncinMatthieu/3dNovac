
/*--------------------------------------------------------------------------------

	3dNovac Utils
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

--------------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_SYSTEM_UNIX_THREAD_H_
#define NC_CORE_UTILS_SYSTEM_UNIX_THREAD_H_

#include <sys/time.h>
#include <pthread.h>
#include "../IThread.h"
#include "../../../Utils/Exception.h"
#include "../../../Utils/Logger.h"

namespace Nc
{
    namespace System
    {
        /// Thread implementation for a unix system
        class Thread : public IThread
        {
            public:
                Thread()            {/*_threadId = 0*/;}
                virtual ~Thread()   {}

                void Start()
                {
                    getitimer(ITIMER_PROF, &_itimer);
                    if (pthread_create(&_threadId, NULL, Launch, this) != 0)
                        throw Utils::Exception("Thread", "The creation of the thread as failed");
                }

                void Wait()
                {
                    if (pthread_join(_threadId, NULL) != 0)
                        std::cerr << "Thread: The waiting can't be the current thread" << std::endl;
                }

            protected:
                inline void Exit()
                {
                    pthread_exit(0);
                }

            private:
                inline static void *Launch(void *t)
                {
                    setitimer(ITIMER_PROF, &reinterpret_cast<Thread*>(t)->_itimer, NULL);
                    reinterpret_cast<Thread*>(t)->Run();
                    return 0;
                }

                pthread_t   		_threadId;
                struct itimerval 	_itimer;
        };
    }
}

#endif
