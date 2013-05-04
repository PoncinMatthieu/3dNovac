
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

    File Created At:        26/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_API_LOCKER_H_
#define NC_CORE_SYSTEM_API_LOCKER_H_

#include "../../Define.h"

namespace Nc
{
    namespace System
    {
        class IMutex;

		/// Used to lock/unlock a mutex safely.
		/**
			The locker lock/unlock the mutex into it's constructor and destructor.
			By following the RAII principle (Resource Acquisition Is Initialization),
			the locker ensure that the mutex will always be unlock even if an exception is thrown.

			<div class="title"> Here is a sample to use a Locker: </div>
 \code
	{
		// create and lock the mutex
		System::Locker l(&_mutex);
		// do stuff to protect against a concurential access
		...
	} // the mutex is unlocked automatically in the destructor of the Locker at the end of the scope, even if an exception has been thrown.
 \endcode
			*/
        class LCORE Locker
        {
            public:
				/** \param mutex is a pointer to the mutex to lock, you can give a null pointer and use the method Lock to lock a new mutex. */
                Locker(IMutex *mutex);
                ~Locker();

				/** Use this method if you gived a null pointer to the constructor. */
                void Lock(IMutex *mutex);
				/** Unlock the mutex, you don't need to call that method if you use the destructor to unlock it. */
                void Unlock();

            private:
                void Ctor(IMutex *mutex);
                void Dtor();

            private:
                IMutex  *_mutex;		///< pointer to the mutex to lock/unlock.
        };
    }
}

#endif
