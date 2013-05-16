
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

#include "../Define.h"
#ifdef SYSTEM_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include "Exception.h"
#include "Daemonizer.h"

using namespace Nc;
using namespace Nc::Utils;

Daemonizer::Daemonizer()
{
}

Daemonizer::~Daemonizer()
{
}

void    Daemonizer::Fork(const FileName &pidFile, const FileName &workingDirectory)
{
    // Our process ID and Session ID
    pid_t pid, sid;

    // Fork off the parent process
    pid = fork();
    if (pid < 0)
        throw Exception("Daemonizer", "Failed to fork the process. - " + std::string(strerror(errno)));

    // Exit the parent process.
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // Change the file mode mask, can be usefull to ensure that further files created are writable/readable.
    umask(0);

    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0)
        throw Exception("Daemonizer", "Failed to set a new SID for the child process. - " + std::string(strerror(errno)));

    // Change the current working directory
    if (!workingDirectory.empty())
    {
        if ((chdir(workingDirectory.c_str())) < 0)
            throw Exception("Daemonizer", "Failed to change working directory. - " + std::string(strerror(errno)));
    }

    // Close out the standard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // Save new SID in the given file for daemon management purpose.
    if (!pidFile.empty())
    {
        int fd = open(pidFile.c_str(), O_WRONLY | O_CREAT, S_IXUSR | S_IRUSR | S_IWUSR);
        if (fd == -1)
            throw Exception("Daemonizer", "Failed open the pid file: " + pidFile + " - " + std::string(strerror(errno)));

        std::string sidStr = Convert::ToString(sid);
        write(fd, sidStr.c_str(), sidStr.size());
        close(fd);
    }
}

#endif
