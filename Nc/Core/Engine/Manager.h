
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
/*-----------------------------------------------------------------------------

                    Implementation de la classe "Manager"

                     gestion des differrents modules du jeux
                Les modules du jeu sont represente par des Engine
                    chaque engine et execute par un thread

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
    namespace Engine
    {
        class   IEngine;
        struct  IEvent;
        typedef std::map<std::string, IEngine*>  MapEngine;

        class LCORE Manager : public Utils::NonCopyable
        {
            public:
                Manager();
                virtual ~Manager();

                virtual void        AddEngine(const std::string &name, IEngine *engine);
                static IEngine      *GetEngine(const std::string &name);

                inline bool         IsLaunched()                    {return _isLaunched;}
                virtual void        Start();   // demarre les threads (engine)
                inline void         Stop(int retVal)                {_retVal = retVal; _isLaunched = false;}    // stop les threads
                virtual int         WaitEngines();
                inline int          RetVal()                        {return _retVal;}
                inline System::Mutex &MutexGlobal()                 {return _mutexGlobal;}

                void                WaitLoadingContextPriority(unsigned char priority);
                void                WaitLoadingPriority(unsigned char priority);
                void                WaitEnginesContextLoading();
                void                WaitEnginesLoading();

                // send un event a l'engine (thread) correspondant
                template<typename T>
                static void         PushEvent(const std::string &engineName, unsigned int id, const T &arg);
                static void         PushEvent(const std::string &engineName, unsigned int id, IEvent *e);
                static void         PushEvent(const std::string &engineName, const std::string &cmdName, const std::string &args);  // from console
                static void         PushEvent(const std::string &engineName, const std::string &cmdName);                           // from console

            protected:
                static MapEngine    _mapEngine;     // map d'engine

                #ifdef SYSTEM_LINUX
                static void         RecieveSegv(int);   // this function is called when a signal SIGSEGV is catch
                #endif
                static void         Terminate();

                bool                _isLaunched;
                int                 _retVal;
                System::Mutex       _mutexGlobal; // to synchronize threads if needed
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

