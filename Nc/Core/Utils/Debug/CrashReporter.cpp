
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2012, The 3dNovac Team

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

    File Created At:        09/11/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <signal.h>
#include "../../Define.h"
#ifdef SYSTEM_LINUX
	#include <execinfo.h>
#endif
#include "CrashReporter.h"
#include "CallStack.h"

using namespace Nc;
using namespace Nc::Utils;

CrashReporter::CrashReporter()
{
    // redirect signal SIGSEGV functions.
    #ifdef SYSTEM_LINUX
    signal(SIGSEGV, &CrashReporter::SignalHandler);
    signal(SIGILL, &CrashReporter::SignalHandler);
    signal(SIGFPE, &CrashReporter::SignalHandler);
    signal(SIGBUS, &CrashReporter::SignalHandler);
    signal(SIGPIPE, &CrashReporter::SignalHandler);
    #endif
}

CrashReporter::~CrashReporter()
{

}

void CrashReporter::Init()
{
    // redirect unhandled exceptions.
    std::set_terminate(Terminate);
}

#ifdef SYSTEM_LINUX
void CrashReporter::SignalHandler(int signal)
{
    CALLSTACK_INFO("CrashReporter::SignalHandler");
    switch (signal)
    {
        case SIGSEGV:   LOG_ERROR << "Fatal error: " << "Received SIGSEGV." << std::endl << std::endl; break;
        case SIGILL:    LOG_ERROR << "Fatal error: " << "Received SIGILL." << std::endl << std::endl; break;
        case SIGFPE:    LOG_ERROR << "Fatal error: " << "Received SIGFPE." << std::endl << std::endl; break;
        case SIGBUS:    LOG_ERROR << "Fatal error: " << "Received SIGBUS." << std::endl << std::endl; break;
        case SIGPIPE:   LOG_ERROR << "Fatal error: " << "Received SIGPIPE." << std::endl << std::endl; break;
    }
    Abort();
}
#endif

//reference: http://www.ibm.com/developerworks/linux/library/l-cppexcep.html
void    CrashReporter::Terminate()
{
    CALLSTACK_INFO("CrashReporter::Terminate");

    try
    {
        throw; // re-throw
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "Fatal error: " << e.what() << std::endl;
    }
    catch (...)
    {
        LOG_ERROR << "Unknown fatal error." << std::endl;
    }
    LOG_ERROR << std::endl;

    Abort();
}

void CrashReporter::Abort()
{
    // print the available backtrace
    LOG_ERROR << "Backtrace: " << std::endl;
    void    *array[25];
    int     nSize = backtrace(array, 25);
    char    **symbols = backtrace_symbols(array, nSize);
    for (int i = 0; i < nSize; i++)
        LOG_ERROR << symbols[i] << std::endl;
    LOG_ERROR << std::endl;
    free(symbols);

    // print the available callstack infos
    const CallStack::MapInstance &instances = CallStack::Instances();
    for (CallStack::MapInstance::const_iterator it = instances.begin(); it != instances.end(); ++it)
    {
        if (!it->second->Empty())
        {
            LOG_ERROR << "CallStack info Thread: " << it->first << std::endl;
            it->second->Print();
            LOG_ERROR << std::endl;
        }
    }
    CallStack::DeleteInstances();

    //if this is a thread performing some core activity
    LOG_ERROR << "Aborting..." << std::endl;
    abort();
    // else if this is a thread used to service requests
    // pthread_exit();
}
