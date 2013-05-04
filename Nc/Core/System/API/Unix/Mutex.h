
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

#ifndef NC_CORE_UTILS_SYSTEM_UNIX_MUTEX_H_
#define NC_CORE_UTILS_SYSTEM_UNIX_MUTEX_H_

#include <pthread.h>
#include "../IMutex.h"
#include "../../../Utils/Exception.h"

namespace Nc
{
    namespace System
    {
        class ThreadCondition;

        /// Implementation of mutex for unix system (with pthread)
        class Mutex : public IMutex
        {
            public:
                Mutex()                                 {pthread_mutex_init(&_mutexId, NULL);}
                Mutex(const Mutex &)                    {pthread_mutex_init(&_mutexId, NULL);}
                Mutex &operator = (const Mutex &)       {return *this;}
                virtual ~Mutex()                        {pthread_mutex_destroy(&_mutexId);}

			private:
                inline void Lock()                      {pthread_mutex_lock(&_mutexId);}
                inline void Unlock()                    {pthread_mutex_unlock(&_mutexId);}

            private:
                pthread_mutex_t     _mutexId;

            friend class ThreadCondition;
        };
    }
}

#endif
