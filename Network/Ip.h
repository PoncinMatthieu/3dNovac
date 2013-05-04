
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

#ifndef NC_CORE_NETWORK_IP_H_
#define NC_CORE_NETWORK_IP_H_

#ifdef SYSTEM_WINDOWS

#else
    #include <netinet/in.h>
#endif

#include "../Define.h"

namespace Nc
{
    namespace Network
    {
        /// Define an ip address.
        /**
            Allows to manipulate IP v4 addresses.
        */
        class LCORE Ip
        {
            public:
                Ip();
                Ip(UInt32 inetaddr);
                Ip(const std::string &ip);
                Ip(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);

                bool    operator == (const Ip &ip) const        {return _addr == ip._addr;}

                /** Init the address with the given \p ip address. */
                void                Init(const std::string &ip);
                /** Init the address with the given 4 byte which describ the ip address. */
                void                Init(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);

                /** \return true if the address is valid. */
                inline bool         IsValid() const             {return _addr != INADDR_NONE;}

                /** \return the address string in dotted-decimal notation. */
                std::string         ToString() const;

            public:
                static const Ip     localHost;      ///< static ip used to define the localhost.

            private:
                UInt32              _addr;          ///< the address of the ip.
        };
    }
}

#endif
