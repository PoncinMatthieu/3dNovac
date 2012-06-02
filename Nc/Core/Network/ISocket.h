
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

    File Created At:        13/04/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_NETWORK_ISOCKET_H_
#define NC_CORE_NETWORK_ISOCKET_H_

#include "../Define.h"
#include "../Math/Array.h"
#include "Ip.h"

namespace Nc
{
    namespace Network
    {
        class LCORE ISocket
        {
            public:
                ISocket();

                /** Create the socket descriptor */
                virtual void    Create() = 0;
                /** Close the descriptor */
                virtual void    Close() = 0;

                /** \return true if the descriptor of the socket is valid */
                inline bool     IsValid() const                     {return (_descriptor != _invalidDescriptor);}

                /** \return the invalid desriptor */
                #ifdef SYSTEM_WINDOWS
                inline SOCKET   InvalidDescriptor() const                  {return _invalidDescriptor;}
                #else
                inline int      InvalidDescriptor() const                  {return _invalidDescriptor;}
                #endif


				/** \return the descriptor of the socket */
                #ifdef SYSTEM_WINDOWS
                inline SOCKET   Descriptor() const                  {return _descriptor;}
                #else
                inline int      Descriptor() const                  {return _descriptor;}
                #endif

            protected:
                #ifdef SYSTEM_WINDOWS
                SOCKET                  _descriptor;
                #else
                int                     _descriptor;
                #endif

            private:
                #ifdef SYSTEM_WINDOWS
                static const SOCKET     _invalidDescriptor;
                #else
                static const int        _invalidDescriptor;
                #endif
        };
    }
}

#endif
