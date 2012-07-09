
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

    File Created At:        12/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_NETWORK_SELECT_H_
#define NC_CORE_NETWORK_SELECT_H_

#include "../Define.h"
#include "ISocket.h"

namespace Nc
{
	/// To Abstract some usefull class to manipulate network connections.
    namespace Network
    {
        /// Define a select class used to determine which socket can be read or write.
		/**
			<div class="title">Full sample to show how to use a select in a Tcp client application:</div>
                \p Examples/Tuto_NcCore_Client.cpp
                \include Tuto_NcCore_Client.cpp
		*/
        class LCORE Select
        {
            public:
                Select();

                /** Clear the set of read and write. */
                void            Clear();
                /** Set the given \p socket for reading. */
                void            SetForReading(const ISocket &socket);
                /** Set the given \p socket for writing. */
                void            SetForWriting(const ISocket &socket);

                /** Wait until one of the sockets is ready for reading/writing, or \p timeout is reached. */
                unsigned int    Wait(float timeout = 0.f);

                /** \return true if the given \p socket is ready for reading. */
                bool            IsReadyForReading(const ISocket &socket);
                /** \return true if the given \p socket is ready for writing. */
                bool            IsReadyForWriting(const ISocket &socket);

            private:
                fd_set                  _readSet;           ///< Set of socket to watch.
                fd_set                  _writeSet;          ///< Set of socket to write.
                int                     _max;               ///< Maximum socket index.
        };
    }
}

#endif
