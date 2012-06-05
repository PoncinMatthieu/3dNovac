
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

#ifndef NC_CORE_ENGINE_ENGINE_H_
#define NC_CORE_ENGINE_ENGINE_H_

#include <iostream>
#include <list>
#include "../Define.h"
#include "../System/API/API.h"
#include "../Utils/Clock.h"
#include "../Utils/Mask.h"
#include "EventManager.h"

namespace Nc
{
    namespace Engine
    {
        class Manager;

        enum Pattern
		{
			Nop = 									0,			///< do nothing
			Synchronized =                          1 << 0,     ///< define if we need to lock/unlock a mutex in the loop to synchronize the engine with an other
			HasAContext =                           1 << 1,     ///< define if we need to create and call `Active/DisableContext` at each loop
			ContextIsLoaded =                       1 << 2,     ///< informe that the context is loaded, /!\ DON'T USE OUTSIDE OF THE CLASS
			WaitingLoadContentsOfOthersEngines =    1 << 3,     ///< force the waiting of the others engines afers to have loaded the contents
			DontWaitOthersContext =                 1 << 4      ///< don't wait the others context (used by the graphic engine for displaying while the loading)
        };

        /// Abstract class to define a Threaded Engine
        /**
            Abstract class to define a Threaded Engine.
            Inherite of EventManager, so an engine has a set of routine to receive events from another Engine/Thread
            <br/>
            An engine could have a "Context". A context is created and load before to load contents.
            An engine has a set of priority in the loading and deleting process.
            So in the loading process and with highest priority, the engine will be loaded at first,
            and in the deleting process and with en highest priority, the engine will be the last one deleted
        */
        class LCORE IEngine : public EventManager, public System::Thread
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT(Nc::Engine::EventManager, Nc::Engine::IEngine);

            public:
                // constructeur et destructeur
                IEngine(const std::string &name, Manager *manager, const Utils::Mask<Pattern> &pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority);
                virtual ~IEngine();

                /** Set the manager */
                inline void             SetManager(Manager *m)          {_manager = m;}

                /** Call the methods `Loading` and `MainLoop` of the engine */
                virtual void            Run();
                /** Sleep the engine */
                void                    Sleep();
                /** Wake up the engine */
                void                    WakeUp();
                /** Stop the engine without stoping the manager */
                void                    Stop();

                /** Set the limit frame rate of the thread. If the limit is not null, the thread will be sleeped to have good frame rate */
                inline void             LimitFrameRate(unsigned int limit)     {_limitFPS = limit;}

                /** Load the contents of the thread before entering in the `MainLoop` */
                virtual inline void     LoadContent()                   {}
				/** Release the contents of the thread just after the `MainLoop` */
                virtual inline void     ReleaseContent()				{}

                /** Execute the engine, to be redefine */
                virtual void            Execute(float runningTime) = 0;

                /** \return true if the context is loaded or if there are no context */
                inline bool             ContextLoaded()                 {return (_pattern.Disabled(HasAContext) || _pattern.Enabled(ContextIsLoaded));}
                /** \return true if the engine is loaded */
                inline bool             Loaded() const                  {return _loaded;}
				/** \return true if the content of the engine has been released */
				inline bool				Released() const				{return _released;}
				/** \return the priority delete */
                unsigned char           DeletePriority()                {return _deletePriority;}
                /** \return the context priority loading */
                unsigned char           LoadingContextPriority()        {return _loadingContextPriority;}
                /** \return the priority loading */
                unsigned char           LoadingPriority()               {return _loadingPriority;}

            protected:
                /** Before to entering in the main loop, load contents and create context by using the engine manager priorities*/
                virtual void            Loading();
                /** Just after the main loop, delete the contents by using the engine manager priorities */
                virtual void            Releasing();
                /** The main loop of the thread witch call the `Process` method until the Manager will stoped */
                virtual void            MainLoop();
                /**
					Process the engine.
					Procedure :
						- Active/DisableContext
						- ExecuteEvents
						- Execute
						- Thread synchronisation with mutex
				*/
                virtual void            Process();

                /** Create the context, to redefine */
                virtual inline void     CreateContext()     {}
                /** Active the context, to redefine */
                virtual inline void     ActiveContext()     {}
                /** Disable the context, to redefine */
                virtual inline void     DisableContext()    {}

            protected:
                Manager                 *_manager;                  ///< The instance of the engine Manager
                bool                    _loaded;                    ///< true if the engine is loaded
				bool					_released;					///< true if the engine has been released

                Utils::Mask<Pattern>    _pattern;                   ///< To detemine the pattern (comportement) of the engine
                unsigned char           _deletePriority;            ///< if the priority is null, the manager will not delete the Engine but the contents will be even deleted at the end of the thread
                unsigned char           _loadingContextPriority;    ///< if the priority is null, no context loading
                unsigned char           _loadingPriority;           ///< if the priority is null, no content loading

            private:
                /** limit the fps if the limit is >0 */
                void                    LimitFrameRate();

                float                   _elapsedTime;               ///< Elapsed time between 2 frame (Execute) in second
                unsigned int            _limitFPS;                  ///< if > 0, used to sleep the thread with the good values to have the good number of seconds
                Utils::Clock            _clock;                     ///< the clock used to compute the elapsed time and the fps sleep if the limit is set
                System::Mutex           _sleepMutex;                ///< mutex used to sleep the engine
                bool                    _stop;                      ///< a boolean to stop the engine
        };
    }
}

#endif
