
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_ENGINE_MANAGER_H_
#define NC_CORE_ENGINE_MANAGER_H_

#include <map>
#include <queue>
#include <string>

#include "../Define.h"
#include "../System/API/API.h"

namespace Nc
{
    /// Provide abstraction to create multithreaded engine, like a GraphicalEngine, GameEngine, AudioEngine, and NetworkEngine
    namespace Engine
    {
        class   IEngine;
        struct  IEvent;
        typedef std::map<std::string, IEngine*>  MapEngine;

        /// Manage Engines of 3dNovac
        /**
            Manage the launching of the threads (engines) and dispatch events between them
        */
        class LCORE Manager : public Utils::NonCopyable
        {
            public:
                /**
                    \param confFile to load the 3dNovac System::Config file, it will be closed in the destructor. if empty, do not load the confFile
                */
                Manager(const Utils::FileName &confFile = "");
                virtual ~Manager();

                /** Add an engine with his corresponding name */
                virtual void        AddEngine(const std::string &name, IEngine *engine);

                /** \return the corresponding engine */
                static IEngine      *GetEngine(const std::string &name);

                /** \return true if the engines is launch */
                inline bool         IsLaunched()                    {return _isLaunched;}

                /** Start the engines */
                virtual void        Start();   // demarre les threads (engine)

                /** Stop the engines */
                inline void         Stop()                          {_isLaunched = false;}    // stop les threads

                /** Wait the engines until they are stoped */
                virtual void        Wait();

                /** \return the mutex witch is used to protect and synchronize the engines */
                inline System::Mutex &MutexGlobal()                 {return _mutexGlobal;}

                void                WaitLoadingContextPriority(unsigned char priority);
                void                WaitLoadingPriority(unsigned char priority);
                void                WaitEnginesContextLoading();
                void                WaitEnginesLoading();

                /** Dispatch events to the corresponding engine */
                template<typename T>
                static void         PushEvent(const std::string &engineName, unsigned int id, const T &arg);
                static void         PushEvent(const std::string &engineName, unsigned int id, IEvent *e);
                static void         PushEvent(const std::string &engineName, const std::string &cmdName, const std::string &args);  // from console
                static void         PushEvent(const std::string &engineName, const std::string &cmdName);                           // from console

            protected:
                static MapEngine    _mapEngine;             ///< Engine map container

                #ifdef SYSTEM_LINUX
                /** this function is called when a signal SIGSEGV is catch, only in unix system. */
                static void         RecieveSegv(int);
                #endif
                static void         Terminate();

                bool                _isLaunched;            ///< true, if the engines are launched.
                System::Mutex       _mutexGlobal;           ///< global mutex used to synchronize threads if needed.

            private:
                bool                _confFileOpened;        ///< true, if the Config file of 3dNovac has been loaded by the Manager, in this case it will be closed be the Manager
        };
    }
}

#include "IEngine.h"

namespace Nc
{
    namespace Engine
    {
        template<typename T>
        void Manager::PushEvent(const std::string &engineName, unsigned int id, const T &arg)
        {
            try
            {
                GetEngine(engineName)->PushEvent(id, arg);
            }
            catch(const std::exception &e)
            {
                LOG_ERROR << "Error: " << e.what() << std::endl;
            }
        }
    }
}

#endif // GAMEMANAGER_H_INCLUDED

