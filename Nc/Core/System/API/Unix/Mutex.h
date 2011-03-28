
/*--------------------------------------------------------------------------------

	3dNovac Utils
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

--------------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_SYSTEM_UNIX_MUTEX_H_
#define NC_CORE_UTILS_SYSTEM_UNIX_MUTEX_H_

#include <pthread.h>
#include "../IMutex.h"

namespace Nc
{
    namespace System
    {
        class ThreadCondition;

        /// Implementation of mutex for unix system (with pthread)
        class Mutex : public IMutex
        {
            public:
                Mutex()                     {pthread_mutex_init(&_mutexId, NULL);}
                virtual ~Mutex()            {pthread_mutex_destroy(&_mutexId);}

                inline void Lock()          {pthread_mutex_lock(&_mutexId);}
                inline void Unlock()        {pthread_mutex_unlock(&_mutexId);}

            private:
                pthread_mutex_t     _mutexId;

            friend class ThreadCondition;
        };
    }
}

#endif
