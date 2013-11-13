
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

    File Created At:        16/05/2013
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_UTILS_DAEMONIZER_H_
#define NC_CORE_UTILS_DAEMONIZER_H_

#ifdef SYSTEM_LINUX

#include "FileName.h"

namespace Nc
{
    namespace Utils
    {
        /// Allow creation of unix deamons.
        /** The daemonizer is available only on unix systems. */
        class Daemonizer
        {
            public:
                Daemonizer();
                virtual ~Daemonizer();

                /** \return true if the application is launched as root. */
                static bool     IsRoot();

                /** Set the current uid with the given username. */
                void            Setuid(const std::string &username);

                /**
                    Daemonize the program.
                    Steps to daemonize a program are:
                    - Fork the process and kill the parent.
                    - Set umask to ensure created files a readable/writable.
                    - Set a new SID for the child process to avoid orphans in the system.
                    - Optionaly change working directory to a secure one.
                    - Close standard file descriptors
                    - Optionaly create a file containing the PID of the detached process.

                    If the given \p workingDirectory is empty, the current directory won't be changed.
                    Same for the PID file, if the \p pidFile is empty we don't create a file to save the pid of the detached process.
                */
                virtual void    Fork(const FileName &pidFile = "", const FileName &workingDirectory = "/");

            protected:
        };
    }
}
#endif

#endif
