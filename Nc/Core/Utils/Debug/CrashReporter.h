
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

#ifndef NC_CORE_UTILS_DEBUG_CRASHREPORTER_H_
#define NC_CORE_UTILS_DEBUG_CRASHREPORTER_H_

#include "../Logger.h"

namespace Nc
{
    namespace Utils
    {
        /// Handle crash of the application.
        /**
            You can redefine your own crash reporter if needed.
        */
        class CrashReporter : public Singleton<CrashReporter>, NonCopyable
        {
            public:
                /**
                    Initialize the crash reporter for the current thread.
                    This method is called by every thread launched by the Nc::System::Thread class and by the Nc::Engine::Manager class.
                */
                virtual void    Init();

                /** Print information about the callstack and then abort the program. */
                static void     Abort();

            protected:
                CrashReporter();
                virtual ~CrashReporter();

                #ifdef SYSTEM_LINUX
                /** this function is called when a signal is catch. But only on unix like operating system. */
                static void     SignalHandler(int);
                #endif
                /** Called in case of uncaught exception. Get info about the exeption and the call Abort function. */
                static void     Terminate();

            protected:
                friend /*LCORE*/ class Singleton<CrashReporter>; // pour avoir acces a l'instance du singleton
        };

        // explicit instanciation
        template class Singleton<CrashReporter>;
    }
}

#endif
