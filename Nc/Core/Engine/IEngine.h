
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
#include "IContext.h"

namespace Nc
{
    namespace Engine
    {
        class Manager;

        enum Pattern
		{
			Nop = 									0,			///< Do nothing
			Synchronized =                          1 << 0,     ///< Define if we need to lock/unlock a mutex in the loop to synchronize the engine with an other
			ContextIsLoaded =                       1 << 1,     ///< Informe that the context is loaded, /!\ DO NOT USE OUTSIDE OF THE CLASS
			WaitingLoadContentsOfOthersEngines =    1 << 2,     ///< Force the waiting of the others engines afers to have loaded the contents
        };

        /// Abstract class to define a Threaded Engine
        /**
            Abstract class to define a Threaded Engine.
            Inherite of EventManager, so an engine has a set of routine to receive events from another Engine/Thread

            An engine can have a "Context". A context is created and load before to load contents.
            To create a context you must redefine the method "CreateContext".

            An engine has a set of priority in the loading and deleting process.
            So in the loading process and with highest priority, the engine will be loaded at first,
            and in the deleting process and with en highest priority, the engine will be the last one to be deleted.
        */
        class LCORE IEngine : public EventManager, public System::Thread
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT(Nc::Engine::EventManager, Nc::Engine::IEngine);

            public:
                // constructeur et destructeur
                IEngine(Manager *manager, const Utils::Mask<Pattern> &pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority);
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

                /** \return the thread id of the engine */
                inline unsigned int     ThreadId() const                {return _threadId;}

                /** \return true if the context is loaded or if there are no context */
                inline bool             ContextLoaded()                 {return _pattern.Enabled(ContextIsLoaded);}
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

                /** \return the context of the engine */
                IContext                *Context()                      {return _context;}
                /** Called by the Manager to request the engine to disable his context */
                void                    RequestDisableContext();
                /** Called by the Manager to reactivate the context previously disabled */
                void                    RequestActiveContext();

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
						- Active/Disable Context
						- ExecuteEvents
						- Execute
						- Thread synchronisation with mutex
				*/
                virtual void            Process();

                /** Create the context, to redefine in the engine requiring a context */
                virtual void            CreateContext()             {_context = NULL;}

            protected:
                Manager                 *_manager;                  ///< The instance of the engine Manager
                bool                    _loaded;                    ///< true if the engine is loaded
				bool					_released;					///< true if the engine has been released

                Utils::Mask<Pattern>    _pattern;                   ///< To detemine the pattern (comportement) of the engine
                unsigned char           _deletePriority;            ///< if the priority is null, the manager will not delete the Engine but the contents will be even deleted at the end of the thread
                unsigned char           _loadingContextPriority;    ///< if the priority is null, no context loading
                unsigned char           _loadingPriority;           ///< if the priority is null, no content loading

                IContext                *_context;                  ///< Instance of the context bound to the engine

            private:
                /** limit the fps if the limit is > 0 */
                void                    LimitFrameRate();

                float                   _elapsedTime;               ///< Elapsed time between 2 frame (Execute) in second
                unsigned int            _limitFPS;                  ///< if > 0, used to sleep the thread with the good values to have the good number of seconds
                Utils::Clock            _clock;                     ///< the clock used to compute the elapsed time and the fps sleep if the limit is set
                System::Mutex           _sleepMutex;                ///< mutex used to sleep the engine
                bool                    _stop;                      ///< a boolean to stop the engine
                unsigned int            _threadId;                  ///< the thread id of the engine
                bool                    _requestedToDisableContext; ///< statement so that the engine know if we want him to disable his context
        };
    }
}

#endif
