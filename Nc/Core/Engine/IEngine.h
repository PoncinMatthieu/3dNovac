
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

    File Created At:        07/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                    Implementation de la classe "Engine"

                     Classe mere de base pour un moteur

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_ENGINE_ENGINE_H_
#define NC_CORE_ENGINE_ENGINE_H_

#include <iostream>
#include <list>
#include "../Define.h"
#include "../System/API/API.h"
#include "../Utils/Clock.h"
#include "EventManager.h"

namespace Nc
{
    namespace Engine
    {
        class Manager;

        enum PATTERN
        {
            Synchronize =                           1 << 0,    ///< define if we need to lock/unlock a mutex in the loop to synchronize the engine with an other
            HasAContext =                           1 << 1,    ///< define if we need to create and call `Active/DisableContext` at each loop
            ContextIsLoaded =                       1 << 2,    ///< informe that the context is loaded, /!\ DON'T USE OUTSIDE OF THE CLASS
            WaitingLoadContentsOfOthersEngines =    1 << 3,    ///< force the waiting of the others engines afers to have loaded the contents
            DontWaitOthersContext =                 1 << 4     ///< don't wait the others context (used by the graphic engine for displaying while the loading)
        };

        class LCORE IEngine : public EventManager, public System::Thread
        {
            public:
                // constructeur et destructeur
                IEngine(const std::string &name, Manager *manager, unsigned int pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority);
                virtual ~IEngine();

                virtual void            Run();                         // execute the Loading and MainLoop of the thread

                inline void             LimitFrameRate(float limit)     {_limitFPS = limit;}
                virtual inline void     LoadContent()                   {}
                virtual void            Execute(float runningTime) = 0;

                inline bool             ContextLoaded()                 {return (!(_pattern & HasAContext) || (_pattern & ContextIsLoaded));}
                inline bool             Loaded() const                  {return _loaded;}
                unsigned char           DeletePriority()                {return _deletePriority;}
                unsigned char           LoadingContextPriority()        {return _loadingContextPriority;}
                unsigned char           LoadingPriority()               {return _loadingPriority;}

            protected:
                virtual void            Loading();                      // before to entering in the main loop, load contents and create context
                virtual void            MainLoop();                     // mainLoop of the thread

                virtual void            Process();                      // process the engine, call functions : Active/DisableContext, ExecuteEvents, Execute, thread synchronisation with mutex
                virtual inline void     CreateContext()     {}
                virtual inline void     ActiveContext()     {}
                virtual inline void     DisableContext()    {}


                Manager*        _manager;   // instance of gameManager
                bool            _loaded;

            private:
                void            LimitFrameRate();

                float           _elapsedTime;
                unsigned int    _limitFPS;
                Utils::Clock    _clock;
                unsigned int    _pattern;
                unsigned char   _deletePriority;            // if the priority is null, the manager will not delete the Engine
                unsigned char   _loadingContextPriority;    // if the priority is null, no context loading
                unsigned char   _loadingPriority;           // if the priority is null, no content loading
        };
    }
}

#endif
